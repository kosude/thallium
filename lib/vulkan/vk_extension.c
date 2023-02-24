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

#define DEFINE_REQUIRED_EXTENSION(name) \
{ \
    count++; \
    if (out_extension_names) out_extension_names[count - 1] = name; \
}


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

int thvk_GetRequiredLayers(const th_RendererConfig_Vulkan_t *config, unsigned int *const out_count, const char **out_layer_names) {
    if (!out_count) {
        return 0;
    }

    if (out_layer_names) {
        memcpy(out_layer_names, config->layer_names, config->layer_count * sizeof(char *));
    }
    *out_count = config->layer_count;

    return 1;
}

int thvk_GetRequiredInstanceExtensions(const int debug_utils_enabled, unsigned int *const out_count, const char **const out_extension_names) {
    if (!out_count) {
        return 0;
    }

    unsigned int count = 0;

    // require platform-agnostic surface extension
    DEFINE_REQUIRED_EXTENSION("VK_KHR_surface");

#   if defined(_WIN32)
        // require windows-specific surface extension
        DEFINE_REQUIRED_EXTENSION("VK_KHR_win32_surface");
#   endif
#if defined(__unix__) || defined(__unix)
        // require unix/linux-specific surface extension
        DEFINE_REQUIRED_EXTENSION("VK_KHR_xcb_surface");
#   endif

    // require debug utils extension (if specified with `debug_utils_enabled`)
    if (debug_utils_enabled) {
        DEFINE_REQUIRED_EXTENSION("VK_EXT_debug_utils");
    }

    // return required extension count
    *out_count = count;

    return 1;
}

int thvk_GetRequiredDeviceExtensions(unsigned int *const out_count, const char **const out_extension_names) {
    if (!out_count) {
        return 0;
    }

    unsigned int count = 0;

    // TODO: allow user-specified device extensions.
    DEFINE_REQUIRED_EXTENSION("VK_KHR_swapchain");

    // return required extension count
    *out_count = count;

    return 1;
}
