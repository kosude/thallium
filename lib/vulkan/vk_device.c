/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium_vulkan.h"

#include "assert.h"
#include "vk_log.h"
#include "utils/log.h"
#include "utils/primitive.h"

#include <stdlib.h>

// A large portion of this file contains *modified* portions of code written by Travis Vroman.
// Source repository can be found at https://github.com/travisvroman/kohi.
// Original code is licensed under the Apache License 2.0.

#define CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(feature) \
{ \
    if (required_features.feature && !available_features.feature) { \
        th_Warn(render_system->debugger, "Vulkan device \"%s\" doesn't meet requirements (missing feature: \"%s\")", device_properties.deviceName, \
            #feature); \
        return 0; \
    } \
}

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

// Structure to contain queue family indices
typedef struct _QueueFamilyInfo_t {
    int graphics_family_index;
    int compute_family_index;
    int transfer_family_index;
} _QueueFamilyInfo_t;

// Get queue family indices
_QueueFamilyInfo_t _GetQueueFamilyInfo(const VkPhysicalDevice physical_device) {
    _QueueFamilyInfo_t r;

    // -1 indicates not found
    r.graphics_family_index = -1;
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
    }

    return r;
}

// Will return 1 if the given physical device is suitable for the application, 0 if not.
// (make sure it supports required extensions for example)
// NOTE: present queue support is not checked here.
static int _PhysicalDeviceIsSuitable(const thvk_RenderSystem_t *render_system, const VkPhysicalDevice physical_device) {
    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceProperties(physical_device, &device_properties);
    vkGetPhysicalDeviceFeatures(physical_device, &device_features);

    // check device extensions

    // get required device extensions
    unsigned int required_extension_count;
    thassert(thvk_GetRequiredDeviceExtensions(render_system, &required_extension_count, NULL));
    const char *required_extension_names[required_extension_count];
    thassert(thvk_GetRequiredDeviceExtensions(render_system, &required_extension_count, required_extension_names));

    // get available device extensions
    unsigned int available_extension_count;
    thassert_vk(vkEnumerateDeviceExtensionProperties(physical_device, NULL, &available_extension_count, NULL));
    VkExtensionProperties available_extension_properties[available_extension_count];
    thassert_vk(vkEnumerateDeviceExtensionProperties(physical_device, NULL, &available_extension_count, available_extension_properties));

    // convert available extension properties to their extracted names
    char *available_extension_names[available_extension_count];
    for (unsigned int i = 0; i < available_extension_count; i++) {
        available_extension_names[i] = available_extension_properties[i].extensionName;
    }

    // also get available INSTANCE extensions so as to filter them out of the extensions list
    unsigned int available_instance_extension_count;
    thassert_vk(vkEnumerateInstanceExtensionProperties(NULL, &available_instance_extension_count, NULL));
    VkExtensionProperties available_instance_extension_properties[available_instance_extension_count];
    thassert_vk(vkEnumerateInstanceExtensionProperties(NULL, &available_instance_extension_count, available_instance_extension_properties));

    // convert available INSTANCE extension properties to their extracted names
    char *available_instance_extension_names[available_instance_extension_count];
    for (unsigned int i = 0; i < available_instance_extension_count; i++) {
        available_instance_extension_names[i] = available_instance_extension_properties[i].extensionName;
    }

    // assert all required extensions are available to the device
    for (unsigned int i = 0; i < required_extension_count; i++) {
        // ensure the extension is not intended for instances
        if (th_StringValueInArray(required_extension_names[i], (const char *const *) available_instance_extension_names,
            available_instance_extension_count))
        {
            // set the extension name to NULL so it is not picked up when the extensions are parsed later on when we enable them
            required_extension_names[i] = NULL;
            continue;
        }

        // if required extension is not available
        if (!th_StringValueInArray(required_extension_names[i], (const char *const *) available_extension_names, available_extension_count)) {
            th_Warn(render_system->debugger, "Vulkan device \"%s\" doesn't meet requirements (missing extension \"%s\")",
                device_properties.deviceName, required_extension_names[i]);
            return 0;
        }
    }

    // check queue families

    // 0 indicates don't care
    int graphics = 0;
    int compute = 0;
    int transfer = 0;
    if (render_system->config_specified) {
        graphics = render_system->config.device_config.graphics_queue_exists;
        compute = render_system->config.device_config.compute_queue_exists;
        transfer = render_system->config.device_config.transfer_queue_exists;
    }

    // get queue family info (indices)
    _QueueFamilyInfo_t queue_family_info = _GetQueueFamilyInfo(physical_device);

    // check available queue families against requirements
    if (graphics && queue_family_info.graphics_family_index <= -1) {
        th_Warn(render_system->debugger, "Vulkan device \"%s\" doesn't meet requirements (missing GRAPHICS queue family)", device_properties.deviceName);
        return 0;
    }
    if (compute && queue_family_info.compute_family_index <= -1) {
        th_Warn(render_system->debugger, "Vulkan device \"%s\" doesn't meet requirements (missing COMPUTE queue family)", device_properties.deviceName);
        return 0;
    }
    if (transfer && queue_family_info.transfer_family_index <= -1) {
        th_Warn(render_system->debugger, "Vulkan device \"%s\" doesn't meet requirements (missing TRANSFER queue family)", device_properties.deviceName);
        return 0;
    }

    // check features

    if (render_system->config_specified) {
        VkPhysicalDeviceFeatures required_features = render_system->config.device_config.features;

        VkPhysicalDeviceFeatures available_features;
        vkGetPhysicalDeviceFeatures(physical_device, &available_features);

        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(robustBufferAccess);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(fullDrawIndexUint32);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(imageCubeArray);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(independentBlend);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(geometryShader);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(tessellationShader);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sampleRateShading);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(dualSrcBlend);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(logicOp);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(multiDrawIndirect);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(drawIndirectFirstInstance);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(depthClamp);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(depthBiasClamp);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(fillModeNonSolid);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(depthBounds);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(wideLines);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(largePoints);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(alphaToOne);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(multiViewport);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(samplerAnisotropy);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(textureCompressionETC2);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(textureCompressionASTC_LDR);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(textureCompressionBC);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(occlusionQueryPrecise);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(pipelineStatisticsQuery);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(vertexPipelineStoresAndAtomics);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(fragmentStoresAndAtomics);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderTessellationAndGeometryPointSize);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderImageGatherExtended);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageImageExtendedFormats);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageImageMultisample);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageImageReadWithoutFormat);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageImageWriteWithoutFormat);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderUniformBufferArrayDynamicIndexing);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderSampledImageArrayDynamicIndexing);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageBufferArrayDynamicIndexing);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageImageArrayDynamicIndexing);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderClipDistance);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderCullDistance);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderFloat64);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderInt64);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderInt16);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderResourceResidency);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderResourceMinLod);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseBinding);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidencyBuffer);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidencyImage2D);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidencyImage3D);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidency2Samples);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidency4Samples);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidency8Samples);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidency16Samples);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidencyAliased);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(variableMultisampleRate);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(inheritedQueries);
    }

    // device meets all requirements!
    return 1;
}

// Get score for the given physical device based on device type, memory size, etc.
static uint64_t _ScorePhysicalDevice(const VkPhysicalDevice physical_device) {
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

    return score;
}


// ===========================================================================
//                           THALLIUM PUBLIC API
// ===========================================================================

int thvk_EnumerateRankedPhysicalDevices(const thvk_RenderSystem_t *render_system, const VkPhysicalDevice *physical_devices,
    const unsigned int physical_device_count, const VkPhysicalDevice **out_physical_devices)
{
    thassert(out_physical_devices);

    _PhysicalDeviceScorePair_t pairs[physical_device_count];

    // if this is the same as physical_device_count by the end, then there are no suitable devices.
    unsigned int unsuitable_device_count = 0;

    // convert list of devices to list of scored pairs...

    for (unsigned int i = 0; i < physical_device_count; i++) {
        uint64_t score = _ScorePhysicalDevice(physical_devices[i]);

        // assert device is considerable
        if (!_PhysicalDeviceIsSuitable(render_system, physical_devices[i])) {
            score = 0;
            unsuitable_device_count++;
        }

        // combine device and its score and add them to the pairs array.
        pairs[i] = (_PhysicalDeviceScorePair_t) {
            &(physical_devices[i]),
            score
        };

        // printing score and device info
#       ifdef THALLIUM_DEBUG_LAYER
            VkPhysicalDeviceProperties device_properties;
            vkGetPhysicalDeviceProperties(physical_devices[i], &device_properties);

            th_Log(render_system->debugger, "Score of \"%s\" = %ld", device_properties.deviceName, score);
#       endif
    }

    // sort filtered device-score pair array
    qsort(pairs, physical_device_count, sizeof(_PhysicalDeviceScorePair_t), _PhysicalDeviceScorePairSortCompareFun);

    if (unsuitable_device_count >= physical_device_count) {
        th_Error(render_system->debugger, "No suitable Vulkan devices found!");
        return 0;
    }

    // populate resultant array - extract physical device pointers from pairs
    // out_physical_devices already asserted
    for (unsigned int i = 0; i < physical_device_count; i++) {
        out_physical_devices[i] = pairs[i].physical_device;
    }

    return 1;
}

int thvk_CreateLogicalDevice(thvk_RenderSystem_t *render_system, const VkPhysicalDevice physical_device) {
    VkDeviceCreateInfo device_info;

    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.pNext = NULL;
    device_info.flags = 0;

    // TODO queues
    device_info.queueCreateInfoCount = 0;
    device_info.pQueueCreateInfos = NULL;

    // deprecated and ignored
    device_info.enabledLayerCount = 0;
    device_info.ppEnabledLayerNames = NULL;

    // request extensions (these should have already been checked via thvk_EnumerateRankedPhysicalDevices())
    unsigned int extension_count;
    thvk_GetRequiredDeviceExtensions(render_system, &extension_count, NULL);
    const char *extension_names[extension_count];
    thvk_GetRequiredDeviceExtensions(render_system, &extension_count, extension_names);

    device_info.enabledExtensionCount = extension_count;
    device_info.ppEnabledExtensionNames = extension_names;

    // request features (these should have already been checked via thvk_EnumerateRankedPhysicalDevices())
    VkPhysicalDeviceFeatures features;
    thvk_GetRequiredDeviceFeatures(render_system, &features);
    device_info.pEnabledFeatures = &features;

    thassert_vk(vkCreateDevice(physical_device, &device_info, NULL, &(render_system->device)));
    th_Note(render_system->debugger, "Created Vulkan logical device at %p", &(render_system->device));

#   ifdef THALLIUM_DEBUG_LAYER
        // print info to stdout
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physical_device, &properties);

        thvk_PrintVkDeviceInfo(render_system->debugger, properties.deviceName, extension_count, extension_names);
#   endif

    return 1;
}
