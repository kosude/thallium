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

int thvk_GetRequiredLayers(const thvk_RenderSystem_t *const render_system, unsigned int *out_count, const char **out_layer_names) {
    if (!out_count || !render_system) {
        return 0;
    }

    if (out_layer_names) {
        memcpy(out_layer_names, render_system->config.extension_config.layer_names, render_system->config.extension_config.layer_count
            * sizeof(char *));
    }
    *out_count = render_system->config.extension_config.layer_count;

    return 1;
}

int thvk_GetRequiredInstanceExtensions(const thvk_RenderSystem_t *const render_system, const int debug_utils_enabled, unsigned int *out_count,
    const char **out_extension_names)
{
    if (!out_count) {
        return 0;
    }

    unsigned int count = 0;

    // constant required extensions:
    // ----

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

    // user-specified required extensions:
    // ----

    if (render_system && render_system->config_specified) {
        // this array also contains device extensions but these should be filtered out separately from this function.
        for (unsigned int i = 0; i < (unsigned int) render_system->config.extension_config.extension_count; i++) {
            DEFINE_REQUIRED_EXTENSION(render_system->config.extension_config.extension_names[i]);
        }
    }

    // return required extension count
    *out_count = count;

    return 1;
}

int thvk_GetRequiredDeviceExtensions(const thvk_RenderSystem_t *const render_system, unsigned int *out_count, const char **out_extension_names) {
    if (!out_count) {
        return 0;
    }

    unsigned int count = 0;

    // constant required extensions:
    // ----

    DEFINE_REQUIRED_EXTENSION("VK_KHR_swapchain");

    // user-specified required extensions:
    // ----

    // this array also contains instance extensions but these should be filtered out separately from this function.
    if (render_system && render_system->config_specified) {
        for (unsigned int i = 0; i < (unsigned int) render_system->config.extension_config.extension_count; i++) {
            DEFINE_REQUIRED_EXTENSION(render_system->config.extension_config.extension_names[i]);
        }
    }

    // return required extension count
    *out_count = count;

    return 1;
}

int thvk_GetRequiredDeviceFeatures(const thvk_RenderSystem_t *const render_system, VkPhysicalDeviceFeatures *out_features) {
    if (!out_features) {
        return 0;
    }

    // user-specified required features:
    // ----

    if (render_system && render_system->config_specified) {
        *out_features = render_system->config.device_config.features;
    }

    return 1;
}
