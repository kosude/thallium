/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"

#include <iostream>
#include <vector>

// TODO: currently redoing this, layers and extensions not working as of current
static std::vector<const char *> VulkanExtensions {
    "VK_KHR_surface",
    "VK_KHR_xcb_surface",
    "VK_EXT_debug_utils"
};
static std::vector<const char *> VulkanLayers {
    "VK_LAYER_KHRONOS_validation"
};

int main() {
    printf("-- Thallium %s\n\n", th_GetThalliumVersion());

    th_Debugger_t *debugger = th_CreateDebugger((th_DebugSeverity_t)
        (
            THALLIUM_DEBUG_SEVERITY_VERBOSE_BIT |
            THALLIUM_DEBUG_SEVERITY_NOTIF_BIT |
            THALLIUM_DEBUG_SEVERITY_WARNING_BIT |
            THALLIUM_DEBUG_SEVERITY_ERROR_BIT |
            THALLIUM_DEBUG_SEVERITY_FATAL_BIT
        )
    );

    th_RendererConfigVulkan_t vulkanConfig = {
        "Hello triangle",   // app name
        { 0, 1, 0 },        // app version
        "No Engine",        // engine name
        { 0, 0, 0, },       // engine version
        NULL,               // optional layers
        0                   // optional layer count
    };

    th_Renderer_t *vulkan = th_CreateRenderer({
        "vulkan",           // api name
        { 1, 3, 0 },        // api version
        &vulkanConfig       // vulkan api configuration
    }, debugger);

    if (!vulkan) {
        printf("Could not create renderer\n");
        return 0;
    }

    th_DestroyRenderer(vulkan);

    th_DestroyDebugger(debugger);

    return 0;
}
