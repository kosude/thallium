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
    const th_RendererConfig_Vulkan_t vulkan_config = {
        "Hello triangle",                       // app name
        { 0, 1, 0 },                            // app version
        "No Engine",                            // engine name
        { 0, 0, 0, },                           // engine version
        (const char **) vulkan_layers.data(),    // optional layers
        (int) vulkan_layers.size()               // optional layer count
    };

    th_Renderer_t *vulkan = th_CreateRenderer({
        "vulkan",           // api name
        { 1, 3, 0 },        // api version
        &vulkan_config       // vulkan api configuration
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
