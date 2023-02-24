/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium_vulkan.h"

#include "utils/log.h"

#include <stdlib.h>

// A large portion of this file contains *modified* portions of code written by Travis Vroman.
// Source repository can be found at https://github.com/travisvroman/kohi.
// Original code is licensed under the Apache License 2.0.

#define MAX_PHYSICAL_DEVICE_PAIRS_COUNT 128
#define MAX_PHYSICAL_DEVICE_EXTENSION_NAME_COUNT 128

// A struct used for sorting list of physical devices by their score.
typedef struct _PhysicalDeviceScorePair_t {
    const VkPhysicalDevice *physical_device;
    uint64_t score;
} _PhysicalDeviceScorePair_t;

// Compare function for qsort (descending order).
static int _PhysicalDeviceScorePairSortCompareFun(const void *a, const void *b) {
    _PhysicalDeviceScorePair_t *pair_a = (_PhysicalDeviceScorePair_t *) a;
    _PhysicalDeviceScorePair_t *pair_b = (_PhysicalDeviceScorePair_t *) b;

    return pair_b->score - pair_a->score;
}

// Structure containing requirement information for a physical device.
typedef struct _PhysicalDeviceRequirements_t {
    // booleans determining whether or not certain queue types need to be supported.
    int graphics;
    int present;
    int compute;
    int transfer;

    // array of names of extensions to be required
    const char **extension_names;
    unsigned int extension_count;

    // set to -1 to not require a type (otherwise set to a VkPhysicalDeviceType enum.)
    int type;
} _PhysicalDeviceRequirements_t;

// Will return 1 if the given physical device is suitable for the application, 0 if not.
// (make sure it supports required extensions for example)
static int _PhysicalDeviceIsSuitable(const thvk_RenderSystem_t *render_system, const VkPhysicalDevice physical_device,
    thvk_QueueFamilyInfo_t *out_info
) {
    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceFeatures device_features;
    VkPhysicalDeviceMemoryProperties memory_properties;

    vkGetPhysicalDeviceProperties(physical_device, &device_properties);
    vkGetPhysicalDeviceFeatures(physical_device, &device_features);
    vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

    // TODO: checking if physical device supports everything needed.
    // TODO2: set up a system where device extensions and properties can be specified!!

    // TODO: allow user-specified configuration of physical device requirements
    _PhysicalDeviceRequirements_t requirements = { 0 };
    requirements.graphics = 1;
    requirements.present = 0;
    requirements.transfer = 1;
    // NOTE: enable if compute will be needed
    // requirements.compute = 0;
    requirements.type = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;

    // get globally required device extensions
    thvk_GetRequiredDeviceExtensions(&requirements.extension_count, NULL);
    requirements.extension_names = malloc(requirements.extension_count * sizeof(char *));
    thvk_GetRequiredDeviceExtensions(&requirements.extension_count, requirements.extension_names);

    // check if the device meets requirements...

    // assert type meets requirement
    if (requirements.type != -1) {
        if ((int) device_properties.deviceType != requirements.type) {
            th_Warn(render_system->debugger, "Vulkan device determined unsuitable (device type mismatch)");
            th_Hint(render_system->debugger, "Required type %d, found type %d instead", requirements.type, device_properties.deviceType);

            return 0;
        }
    }

    // get queue family info (indices)
    thvk_QueueFamilyInfo_t queue_family_info = thvk_GetQueueFamilyInfo(physical_device);

    // check available queue families against requirements
    if (requirements.graphics && queue_family_info.graphics_family_index <= -1) {
        th_Warn(render_system->debugger, "Vulkan device \"%s\" doesn't meet requirements (missing GRAPHICS queue family)", device_properties.deviceName);
        return 0;
    }
    if (requirements.present && queue_family_info.present_family_index <= -1) {
        th_Warn(render_system->debugger, "Vulkan device \"%s\" doesn't meet requirements (missing PRESENT queue family)", device_properties.deviceName);
        return 0;
    }
    if (requirements.compute && queue_family_info.compute_family_index <= -1) {
        th_Warn(render_system->debugger, "Vulkan device \"%s\" doesn't meet requirements (missing COMPUTE queue family)", device_properties.deviceName);
        return 0;
    }
    if (requirements.transfer && queue_family_info.transfer_family_index <= -1) {
        th_Warn(render_system->debugger, "Vulkan device \"%s\" doesn't meet requirements (missing TRANSFER queue family)", device_properties.deviceName);
        return 0;
    }

    if (out_info) {
        *out_info = queue_family_info;
    }

    free(requirements.extension_names);

    return 1;
}

// Get score for the given physical device based on device type, memory size, etc.
static uint64_t _ScorePhysicalDevice(const VkPhysicalDevice physical_device, const th_Debugger_t *debugger) {
    unsigned long int score = 0;

    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceMemoryProperties memory_properties;

    vkGetPhysicalDeviceProperties(physical_device, &device_properties);
    vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

    // increase score for typically discrete GPUs
    // (significant performance advantage over integrated ofc)
    if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 2000;
    }

    // get memory size
    for (unsigned int i = 0; i < memory_properties.memoryHeapCount; i++) {
        if (memory_properties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            // device local heap: .size will be size of VRAM in bytes.
            // we add this size to the score (converted to smaller number)
            score += memory_properties.memoryHeaps[i].size / (1024 * 1024);
        }
    }

    th_Log(debugger, "Considering Vulkan device \"%s\" (met requirements, scored %ld)", device_properties.deviceName, score);

    return score;
}


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

int thvk_EnumerateRankedPhysicalDevices(const thvk_RenderSystem_t *render_system, const VkPhysicalDevice *physical_devices,
    const unsigned int physical_device_count, unsigned int *const out_count, const VkPhysicalDevice **const out_physical_devices)
{
    if (!out_count) {
        return 0;
    }

    _PhysicalDeviceScorePair_t pairs[MAX_PHYSICAL_DEVICE_PAIRS_COUNT];

    // amount of devices minus ones that don't support required extensions/features
    // this will be the amount of set items in pairs[], and thus that of the returned array.
    unsigned int real_count = 0;

    // convert list of devices to list of scored pairs...

    for (unsigned int i = 0; i < physical_device_count; i++) {
        thvk_QueueFamilyInfo_t queue_family_info;

        // assert device is considerable
        if (!_PhysicalDeviceIsSuitable(render_system, physical_devices[i], &queue_family_info)) {
            continue;
        }

        uint64_t score = _ScorePhysicalDevice(physical_devices[i], (out_physical_devices) ? render_system->debugger : NULL);

        // combine device and its score and add them to the pairs array.
        pairs[i] = (_PhysicalDeviceScorePair_t) {
            &(physical_devices[i]),
            score
        };

        real_count++;

        // print queue information (we only do this when out_physical_devices is not NULL so as to avoid duplicate messages)
        // this is also wrapped in a THALLIUM_DEBUG_LAYER condition to otherwise avoid the vkGet call.
#       ifdef THALLIUM_DEBUG_LAYER
            if (out_physical_devices) {
                VkPhysicalDeviceProperties props;
                vkGetPhysicalDeviceProperties(physical_devices[i], &props);

                // print queue family info
                th_Log(render_system->debugger,
                    "Queue family indices for \"%s\"...\n"
                    "     GRAPHICS PRESENT COMPUTE TRANSFER (-1 means not found)\n"
                    "         %d       %d       %d        %d",
                    props.deviceName,
                    queue_family_info.graphics_family_index,
                    queue_family_info.present_family_index,
                    queue_family_info.compute_family_index,
                    queue_family_info.transfer_family_index
                );
            }
#       endif
    }

    // sort filtered device-score pair array
    qsort(pairs, real_count, sizeof(_PhysicalDeviceScorePair_t), _PhysicalDeviceScorePairSortCompareFun);

    // return real count
    if (out_count) {
        *out_count = real_count;
    }

    if (real_count == 0) {
        th_Error(render_system->debugger, "No suitable Vulkan devices found!");
        // we return 0 if there are no suitable devices
        return 0;
    }

    // populate resultant array - extract physical device pointers from pairs
    if (out_physical_devices) {
        for (unsigned int i = 0; i < real_count; i++) {
            out_physical_devices[i] = pairs[i].physical_device;
        }
    }

    return 1;
}

thvk_QueueFamilyInfo_t thvk_GetQueueFamilyInfo(const VkPhysicalDevice physical_device) {
    thvk_QueueFamilyInfo_t r;

    // -1 indicates not found
    r.graphics_family_index = -1;
    r.present_family_index = -1;
    r.compute_family_index = -1;
    r.transfer_family_index = -1;

    // get queue families
    unsigned int queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, NULL);
    VkQueueFamilyProperties queue_families[queue_family_count];
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families);

    // this score is reset after each queue family, and is incremented each time one has a flag other than
    // VK_QUEUE_TRANSFER_BIT. finally, the family with the lowest score is identified as the transfer queue family,
    // as it is the most likely one to be a dedicated transfer queue.
    unsigned char min_transfer_score = 255;

    for (unsigned int i = 0; i < queue_family_count; i++) {
        unsigned int transfer_score = 0;

        // if it is a graphics queue
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            r.graphics_family_index = i;

            transfer_score++;
        }

        // if it is a compute queue
        if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            r.compute_family_index = i;

            transfer_score++;
        }

        // if it is a transfer queue (ideally dedicated)
        if (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
            // take index if current lowest transfer score to aim for a dedicated transfer queue.
            if (transfer_score <= min_transfer_score) {
                r.transfer_family_index = i;

                // update the minimum transfer score
                min_transfer_score = transfer_score;
            }
        }

        // TODO: this (requires surfaces + swapchains)
        // if it is a present queue
        // VkBool32 supportsPresent = VK_FALSE;
        // vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, XXX, &supportsPresent);
        // if (supportsPresent) {
        //     r.present_family_index = i;
        // }
    }

    return r;
}
