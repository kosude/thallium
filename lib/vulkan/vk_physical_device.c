/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium_vulkan.h"

#include "utils/log.h"

#include <stdlib.h>

// Get score for the given physical device based on device type, memory size, etc.
// -1 is returned if it is not suitable at all.
static const int _ScorePhysicalDevice(const VkPhysicalDevice physicalDevice, const th_Debugger_t *debugger) {
    int score = 0;

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceMemoryProperties memoryProperties;

    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

    // increase score for typically discrete GPUs
    // (significant performance advantage over integrated ofc)
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 50;
    }

    // get memory size
    unsigned int memorySize = 0;
    for (unsigned int i = 0; i < memoryProperties.memoryHeapCount; i++) {
        memorySize += memoryProperties.memoryHeaps[i].size / (1024 * 1024 * 1024);
    }

    // increase score by (relative) memory size
    score += memorySize;

    th_Log(debugger, "Given score %d to device \"%s\"", score, deviceProperties.deviceName);

    return score;
}

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

    return 1;
}


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

const VkPhysicalDevice *thvk_GetAvailablePhysicalDevices(const thvk_RenderSystem_t *renderSystem, unsigned int *out_count) {
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

const VkPhysicalDevice *thvk_EnumerateRankedPhysicalDevices(const thvk_RenderSystem_t *renderSystem, const VkPhysicalDevice *physicalDevices,
    const unsigned int physicalDeviceCount)
{
    for (unsigned int i = 0; i < physicalDeviceCount; i++) {
        // if device does not support requirements then we don't consider it
        if (!_PhysicalDeviceIsSuitable(physicalDevices[i], renderSystem->debugger)) {
            continue;
        }

        // TODO: some more of this rubbish (see TODOs above)

        _ScorePhysicalDevice(physicalDevices[i], renderSystem->debugger);
    }

    return NULL;
}
