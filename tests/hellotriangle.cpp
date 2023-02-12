/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"

#include <iostream>
#include <vector>

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

    // debugger for thallium functions
    th_Debugger_t *debugger = th_CreateDebugger(THALLIUM_DEBUG_SEVERITY_ALL_BIT);

    th_RendererExtensionDescriptor_t extensionDescr = {
        // vulkan renderers
        {
            // extensions
            VulkanExtensions.data(),
            (int) VulkanExtensions.size(),
            // layers
            VulkanLayers.data(),
            (int) VulkanLayers.size(),
            // flags
            THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_ERROR_BIT | THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_WARNING_BIT
            | THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_TYPE_ALL_BIT
        }
    };
    if (!th_ValidateRendererExtensionDescriptor(extensionDescr, debugger)) {
        std::cout << "Problem with renderer extension descriptor" << std::endl;
        return 1;
    }

    th_Renderer_t *vulkan = th_CreateRenderer({
        "vulkan",           // api name
        { 1, 3, 0 },        // api version
        "Hello triangle",   // application name
        { 0, 1, 0 },        // application version
        "No Engine",        // engine name
        { 0, 0, 0 },        // engine version
        extensionDescr      // extension descriptor
    }, debugger);

    if (!vulkan) {
        printf("Could not create renderer\n");
        return 0;
    }

    th_DestroyRenderer(vulkan);

    return 0;
}
