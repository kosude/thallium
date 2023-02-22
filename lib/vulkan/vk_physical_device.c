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

#define MAX_PHYSICAL_DEVICE_PAIRS_COUNT 128

// A struct used for sorting list of physical devices by their score.
typedef struct _PhysicalDeviceScorePair {
    const VkPhysicalDevice *physicalDevice;
    uint64_t score;
} _PhysicalDeviceScorePair;

// Compare function for qsort (descending order).
static const int _PhysicalDeviceScorePairSortCompareFun(const void *a, const void *b) {
    _PhysicalDeviceScorePair *pairA = (_PhysicalDeviceScorePair *) a;
    _PhysicalDeviceScorePair *pairB = (_PhysicalDeviceScorePair *) b;

    return pairB->score - pairA->score;
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
    // NOTE: reminder to print a log message if the deivce is not suitable.

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

    th_Log(debugger, "Given score %ld to Vulkan device \"%s\"", score, deviceProperties.deviceName);

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

VkPhysicalDevice *thvk_RankPhysicalDevices(const thvk_RenderSystem_t *renderSystem, const VkPhysicalDevice *physicalDevices,
    const unsigned int physicalDeviceCount, unsigned int *out_count)
{
    _PhysicalDeviceScorePair pairs[MAX_PHYSICAL_DEVICE_PAIRS_COUNT];

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
        pairs[i] = (_PhysicalDeviceScorePair) {
            &(physicalDevices[i]),
            score
        };

        realCount++;
    }

    // sort device-score pair array
    qsort(pairs, realCount, sizeof(_PhysicalDeviceScorePair), _PhysicalDeviceScorePairSortCompareFun);

    // return real count
    if (out_count) {
        *out_count = realCount;
    }

    if (realCount == 0) {
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
