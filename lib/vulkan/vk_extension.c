/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"
#include "thallium_vulkan.h"

#include "utils/log.h"

#include <stdlib.h>
#include <string.h>

const int thvk_ValidateInstanceExtension(const char *const extensionName, const char *const layerName) {
    if (!extensionName) {
        th_Error("Extension name parameter NULL in thvk_ValidateInstanceExtension");
        return 0;
    }

    unsigned int extCount = 0;
    vkEnumerateInstanceExtensionProperties(layerName, &extCount, NULL);

    VkExtensionProperties *availableExts = malloc(extCount * sizeof(VkExtensionProperties));
    if (!availableExts) {
        th_Fatal("MALLOC fault in thvk_ValidateInstanceExtension!");
        return 0;
    }
    vkEnumerateInstanceExtensionProperties(layerName, &extCount, availableExts);

    // returned at end
    int found = 0;

    for (unsigned int i = 0; i < extCount; i++) {
        if (!strcmp(availableExts[i].extensionName, extensionName)) {
            found = 1;
            break;
        }
    }

    free(availableExts);
    availableExts = NULL;

    return found;
}

const int thvk_ValidateLayer(const char *const layerName) {
    if (!layerName) {
        th_Error("Layer name parameter NULL in thvk_ValidateLayer");
        return 0;
    }

    unsigned int layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties *availableLayers = malloc(layerCount * sizeof(VkLayerProperties));
    if (!availableLayers) {
        th_Fatal("MALLOC fault in thvk_ValidateLayer!");
        return 0;
    }
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    // returned at end
    int found = 0;

    for (unsigned int i = 0; i < layerCount; i++) {
        if (!strcmp(availableLayers[i].layerName, layerName)) {
            found = 1;
            break;
        }
    }

    free(availableLayers);
    availableLayers = NULL;

    return found;
}
