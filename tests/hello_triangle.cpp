/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"

#include <iostream>
#include <vector>

// Print a formatted message to stdout.
template <typename... Args>
void Print(Args... args) {
    ((std::cout << "hellotriangle >>> ") << ... << args) << std::endl;
}

int main() {
    Print("Thallium ", th_GetThalliumVersionString());

    th_Debugger_t *debugger = th_CreateDebugger(
        (th_DebugSeverity_t) (
            THALLIUM_DEBUG_SEVERITY_VERBOSE_BIT |
            THALLIUM_DEBUG_SEVERITY_NOTIF_BIT |
            THALLIUM_DEBUG_SEVERITY_WARNING_BIT |
            THALLIUM_DEBUG_SEVERITY_ERROR_BIT |
            THALLIUM_DEBUG_SEVERITY_FATAL_BIT
        ),
        (th_DebugType_t) (
            THALLIUM_DEBUG_TYPE_CORE_BIT |
            THALLIUM_DEBUG_TYPE_VULKAN_BIT
        )
    );

    const std::vector<const char *> vulkan_layers {
        "VK_LAYER_KHRONOS_validation"
    };
    const std::vector<const char *> vulkan_extensions {
        "VK_KHR_external_fence",
        "VK_EXT_debug_report",
        "VK_KHR_display",
        "VK_EXT_acquire_drm_display",
        "VK_KHR_surface_protected_capabilities",
        "VK_KHR_xlib_surface",
        "VK_KHR_external_fence_capabilities",
        "VK_GOOGLE_hlsl_functionality1",
        "VK_GOOGLE_decorate_string",
        "VK_AMD_shader_core_properties2",
        "VK_VALVE_mutable_descriptor_type",
        "VK_EXT_direct_mode_display",
        "VK_KHR_get_surface_capabilities2"
    };
    const th_RendererConfig_Vulkan_t vulkan_config = {
        (char *) "Hello triangle",          // app name
        { 0, 1, 0 },                        // app version
        (char *) "No Engine",               // engine name
        { 0, 0, 0, },                       // engine version
        (char **) vulkan_layers.data(),     // specified layers
        (int) vulkan_layers.size(),         // specified layer count
        (char **) vulkan_extensions.data(), // specified extensions
        (int) vulkan_extensions.size()      // specified extension count
    };

    th_Renderer_t *vulkan = th_CreateRenderer({
        "vulkan",           // api name
        { 1, 3, 0 },        // api version
        &vulkan_config      // vulkan api configuration
    }, debugger);

    if (!vulkan) {
        Print("Could not create renderer");
        return 0;
    } else {
        Print("Created and initialised renderer");
    }

    Print("Hello triangle!");

    th_DestroyRenderer(vulkan);
    th_DestroyDebugger(debugger);
    return 0;
}
