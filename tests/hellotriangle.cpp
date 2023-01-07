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
    th_ConfigureDebugMessageFilter(THALLIUM_DEBUG_SEVERITY_ALL_BIT);

    th_RendererExtensionDescriptor_t extensionDescr = {
        // vulkan extension
        {
            // extensions
            VulkanExtensions.data(),
            (int) VulkanExtensions.size(),
            // layers
            VulkanLayers.data(),
            (int) VulkanLayers.size()
        }
    };

    th_ValidateRendererExtensionDescriptor(extensionDescr);

    const th_Renderer_t *vulkan = th_CreateRenderer({
        "vulkan",           // api name
        { 1, 3, 0 },        // api version
        "Hello triangle",   // application name
        { 0, 1, 0 },        // application version
        "No Engine",        // engine name
        { 0, 0, 0 },        // engine version
        extensionDescr      // extension descriptor
    });

    std::cout << vulkan << std::endl;

    return 0;
}
