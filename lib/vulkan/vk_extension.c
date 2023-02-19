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

#define MAX_REQUIRED_INSTANCE_EXTENSION_COUNT 128

#define LAYER_MAX_NAME_LEN 128
#define EXTENSION_MAX_NAME_LEN 128


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

char **thvk_GetRequiredLayers(unsigned int *out_count, const th_RendererConfigVulkan_t *config) {
    if (out_count) {
        *out_count = config->layerCount;
    }

    return (char **) config->layerNames;
}

char **thvk_GetRequiredInstanceExtensions(unsigned int *out_count, const int debugUtilsEnabled, const th_Debugger_t *debugger) {
    char **r = malloc(MAX_REQUIRED_INSTANCE_EXTENSION_COUNT * sizeof(char *));
    if (!r) {
        th_Fatal(debugger, "MALLOC fault in thvk_GetRequiredInstanceExtensions!");
        return 0;
    }

    unsigned int count = 1; // starts at 1 as VK_KHR_surface is always required

    //
    // forming a list of extensions based on compilation settings and renderer configuration...

    // platform-agnostic surface extension
    r[count - 1] = "VK_KHR_surface";
#   if defined(_WIN32)
        // windows-specific surfaces
        count++;
        r[count - 1] = "VK_KHR_win32_surface";
#   endif
#if defined(__unix__) || defined(__unix)
        // unix/linux-specific surfaces
        count++;
        r[count - 1] = "VK_KHR_xcb_surface";
#   endif

    // debug utils
    if (debugUtilsEnabled) {
        count++;
        r[count - 1] = "VK_EXT_debug_utils";
    }

    // return required extension count
    if (out_count) {
        *out_count = count;
    }

    return r;
}

char **thvk_GetAvailableLayers(unsigned int *out_count, const th_Debugger_t *debugger) {
    unsigned int layerPropertyCount = 0;
    vkEnumerateInstanceLayerProperties(&layerPropertyCount, NULL);

    VkLayerProperties *layerProperties = malloc(layerPropertyCount * sizeof(VkLayerProperties));
    if (!layerProperties) {
        th_Fatal(debugger, "MALLOC fault in thvk_GetAvailableLayers!");
        return 0;
    }
    vkEnumerateInstanceLayerProperties(&layerPropertyCount, layerProperties);

    // convert layer properties to layer names (strings)
    char **r = malloc(layerPropertyCount * sizeof(char *));
    if (!r) {
        th_Fatal(debugger, "MALLOC fault in thvk_GetAvailableLayers!");
        return 0;
    }
    for (unsigned int i = 0; i < layerPropertyCount; i++) {
        r[i] = malloc(LAYER_MAX_NAME_LEN * sizeof(char));
        strncpy(r[i], layerProperties[i].layerName, LAYER_MAX_NAME_LEN);
    }

    // return count
    if (out_count) {
        *out_count = layerPropertyCount;
    }

    free(layerProperties);

    return r;
}

char **thvk_GetAvailableInstanceExtensions(unsigned int *out_count, const char *const layerName, const th_Debugger_t *debugger) {
    unsigned int extensionPropertyCount = 0;
    vkEnumerateInstanceExtensionProperties(layerName, &extensionPropertyCount, NULL);

    VkExtensionProperties *extensionProperties = malloc(extensionPropertyCount * sizeof(VkExtensionProperties));
    if (!extensionProperties) {
        th_Fatal(debugger, "MALLOC fault in thvk_GetAvailableInstanceExtensions!");
        return 0;
    }
    vkEnumerateInstanceExtensionProperties(layerName, &extensionPropertyCount, extensionProperties);

    // convert extension properties to extension names (strings)
    char **r = malloc(extensionPropertyCount * sizeof(char *));
    if (!r) {
        th_Fatal(debugger, "MALLOC fault in thvk_GetAvailableLayers!");
        return 0;
    }
    for (unsigned int i = 0; i < extensionPropertyCount; i++) {
        r[i] = malloc(EXTENSION_MAX_NAME_LEN * sizeof(char));
        strncpy(r[i], extensionProperties[i].extensionName, EXTENSION_MAX_NAME_LEN);
    }

    // return count
    if (out_count) {
        *out_count = extensionPropertyCount;
    }

    free(extensionProperties);

    return r;
}
