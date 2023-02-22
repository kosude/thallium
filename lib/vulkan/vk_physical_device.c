/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium_vulkan.h"

#include "utils/log.h"

#include <stdlib.h>
#include <stdint.h>

// A large portion of this file contains *modified* portions of code written by Travis Vroman.
// Source repository can be found at https://github.com/travisvroman/kohi.
// Original code is licensed under the Apache License 2.0.

#define MAX_PHYSICAL_DEVICE_PAIRS_COUNT 128
#define MAX_PHYSICAL_DEVICE_EXTENSION_NAME_COUNT 128

// A struct used for sorting list of physical devices by their score.
typedef struct _PhysicalDeviceScorePair_t {
    const VkPhysicalDevice *physicalDevice;
    uint64_t score;
} _PhysicalDeviceScorePair_t;

// Compare function for qsort (descending order).
static const int _PhysicalDeviceScorePairSortCompareFun(const void *a, const void *b) {
    _PhysicalDeviceScorePair_t *pairA = (_PhysicalDeviceScorePair_t *) a;
    _PhysicalDeviceScorePair_t *pairB = (_PhysicalDeviceScorePair_t *) b;

    return pairB->score - pairA->score;
}

// Structure containing requirement information for a physical device.
typedef struct _PhysicalDeviceRequirements_t {
    // booleans determining whether or not certain queue types need to be supported.
    int graphics;
    int present;
    int compute;
    int transfer;

    // array of names of extensions to be required
    char **extensionNames;
    unsigned int extensionCount;

    // set to -1 to not require a type (otherwise set to a VkPhysicalDeviceType enum.)
    int type;
} _PhysicalDeviceRequirements_t;

// Will return 1 if the given physical device is suitable for the application, 0 if not.
// (make sure it supports required extensions for example)
static const int _PhysicalDeviceIsSuitable(const VkPhysicalDevice physicalDevice, const th_Debugger_t *debugger) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    VkPhysicalDeviceMemoryProperties memoryProperties;

    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

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
    requirements.extensionNames = thvk_GetRequiredPhysicalDeviceExtensions(&requirements.extensionCount, debugger);

    // check if the device meets requirements...

    // assert type meets requirement
    if (requirements.type != -1) {
        if ((int) deviceProperties.deviceType != requirements.type) {
            th_Warn(debugger, "Vulkan device determined unsuitable (device type mismatch)");
            th_Hint(debugger, "Required type %d, found type %d instead", requirements.type, deviceProperties.deviceType);

            return 0;
        }
    }

    // get queue family info (indices)
    thvk_QueueFamilyInfo_t queueFamilyInfo = thvk_GetQueueFamilyInfo(physicalDevice, debugger);

    // check available queue families against requirements
    if (requirements.graphics && queueFamilyInfo.graphicsFamilyIndex <= -1) {
        th_Warn(debugger, "Vulkan device \"%s\" doesn't meet requirements (missing GRAPHICS queue family)", deviceProperties.deviceName);
        return 0;
    }
    if (requirements.present && queueFamilyInfo.presentFamilyIndex <= -1) {
        th_Warn(debugger, "Vulkan device \"%s\" doesn't meet requirements (missing PRESENT queue family)", deviceProperties.deviceName);
        return 0;
    }
    if (requirements.compute && queueFamilyInfo.computeFamilyIndex <= -1) {
        th_Warn(debugger, "Vulkan device \"%s\" doesn't meet requirements (missing COMPUTE queue family)", deviceProperties.deviceName);
        return 0;
    }
    if (requirements.transfer && queueFamilyInfo.transferFamilyIndex <= -1) {
        th_Warn(debugger, "Vulkan device \"%s\" doesn't meet requirements (missing TRANSFER queue family)", deviceProperties.deviceName);
        return 0;
    }

    return 1;
}

// Get score for the given physical device based on device type, memory size, etc.
static const uint64_t _ScorePhysicalDevice(const VkPhysicalDevice physicalDevice, const th_Debugger_t *debugger) {
    uint64_t score = 0;

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceMemoryProperties memoryProperties;

    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

    // increase score for typically discrete GPUs
    // (significant performance advantage over integrated ofc)
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    // get memory size
    for (unsigned int i = 0; i < memoryProperties.memoryHeapCount; i++) {
        if (memoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            // device local heap: .size will be size of VRAM in bytes.
            // we add this size to the score (converted to smaller number)
            score += memoryProperties.memoryHeaps[i].size / (1024 * 1024);
        }
    }

    th_Log(debugger, "Considering Vulkan device \"%s\" (met requirements, scored %ld)", deviceProperties.deviceName, score);

    return score;
}


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

VkPhysicalDevice *thvk_GetAvailablePhysicalDevices(const thvk_RenderSystem_t *renderSystem, unsigned int *out_count) {
    unsigned int physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(renderSystem->instance, &physicalDeviceCount, NULL);

    if (physicalDeviceCount == 0) {
        th_Warn(renderSystem->debugger, "No devices that support Vulkan were found.");
    }

    VkPhysicalDevice *r = malloc(physicalDeviceCount * sizeof(VkPhysicalDevice));
    if (!r) {
        th_Fatal(renderSystem->debugger, "MALLOC fault in thvk_GetAvailablePhysicalDevices!");
        return NULL;
    }
    vkEnumeratePhysicalDevices(renderSystem->instance, &physicalDeviceCount, r);

    // return physical device count
    if (out_count) {
        *out_count = physicalDeviceCount;
    }

    return r;
}

VkPhysicalDevice *thvk_RankPhysicalDevices(const VkPhysicalDevice *physicalDevices, const unsigned int physicalDeviceCount,
    const thvk_RenderSystem_t *renderSystem, unsigned int *out_count)
{
    _PhysicalDeviceScorePair_t pairs[MAX_PHYSICAL_DEVICE_PAIRS_COUNT];

    // amount of devices minus ones that don't support required extensions/features
    // this will be the amount of set items in pairs[], and thus that of the returned array.
    unsigned int realCount = 0;

    // convert list of devices to list of scored pairs...

    for (unsigned int i = 0; i < physicalDeviceCount; i++) {
        // assert device is considerable
        if (!_PhysicalDeviceIsSuitable(physicalDevices[i], renderSystem->debugger)) {
            continue;
        }

        uint64_t score = _ScorePhysicalDevice(physicalDevices[i], renderSystem->debugger);

        // combine device and its score and add them to the pairs array.
        pairs[i] = (_PhysicalDeviceScorePair_t) {
            &(physicalDevices[i]),
            score
        };

        realCount++;
    }

    // sort filtered device-score pair array
    qsort(pairs, realCount, sizeof(_PhysicalDeviceScorePair_t), _PhysicalDeviceScorePairSortCompareFun);

    // return real count
    if (out_count) {
        *out_count = realCount;
    }

    if (realCount == 0) {
        th_Error(renderSystem->debugger, "No suitable Vulkan devices found!");
        // we return NULL if there are no suitable devices
        return NULL;
    }

    // allocate...
    VkPhysicalDevice *r = malloc(realCount * sizeof(VkPhysicalDevice));
    if (!r) {
        th_Fatal(renderSystem->debugger, "MALLOC fault in thvk_RankPhysicalDevices!");
        return NULL;
    }

    /// ...and populate resultant array - extract physical devices from pairs
    for (unsigned int i = 0; i < realCount; i++) {
        r[i] = *(pairs[i].physicalDevice);
    }

    return r;
}

const thvk_QueueFamilyInfo_t thvk_GetQueueFamilyInfo(const VkPhysicalDevice physicalDevice, const th_Debugger_t *debugger) {
    thvk_QueueFamilyInfo_t r;

    // -1 indicates not found
    r.graphicsFamilyIndex = -1;
    r.presentFamilyIndex = -1;
    r.computeFamilyIndex = -1;
    r.transferFamilyIndex = -1;

    // get queue families
    unsigned int queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);
    VkQueueFamilyProperties queueFamilies[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);

    // this score is reset after each queue family, and is incremented each time one has a flag other than
    // VK_QUEUE_TRANSFER_BIT. finally, the family with the lowest score is identified as the transfer queue family,
    // as it is the most likely one to be a dedicated transfer queue.
    unsigned char minTransferScore = 255;

    for (unsigned int i = 0; i < queueFamilyCount; i++) {
        unsigned int transferScore = 0;

        // if it is a graphics queue
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            r.graphicsFamilyIndex = i;

            transferScore++;
        }

        // if it is a compute queue
        if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            r.computeFamilyIndex = i;

            transferScore++;
        }

        // if it is a transfer queue (ideally dedicated)
        if (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
            // take index if current lowest transfer score to aim for a dedicated transfer queue.
            if (transferScore <= minTransferScore) {
                r.transferFamilyIndex = i;

                // update the minimum transfer score
                minTransferScore = transferScore;
            }
        }

        // TODO: this (requires surfaces + swapchains)
        // if it is a present queue
        // VkBool32 supportsPresent = VK_FALSE;
        // vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, XXX, &supportsPresent);
        // if (supportsPresent) {
        //     r.presentFamilyIndex = i;
        // }
    }

    // we only do this whole section to otherwise avoid the vkGet* call.
#   ifdef THALLIUM_DEBUG_LAYER
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(physicalDevice, &props);

        // print queue family info
        th_Log(debugger,
            "Queue family indices for \"%s\"...\n"
            "     GRAPHICS PRESENT COMPUTE TRANSFER (-1 means not found)\n"
            "         %d       %d       %d        %d",
            props.deviceName,
            r.graphicsFamilyIndex,
            r.presentFamilyIndex,
            r.computeFamilyIndex,
            r.transferFamilyIndex
        );
#   endif

    return r;
}
