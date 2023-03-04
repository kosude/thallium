/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "test_base.hpp"

#include "thallium.h"

#ifdef THALLIUM_VULKAN_INCL
#   include "thallium_vulkan.h"
#endif

#include <vector>

namespace thallium_tests {
    #ifdef THALLIUM_VULKAN_INCL
        template <>
        void TestBase<TestApi::Vulkan>::_CreateRenderer() {
            // NOTE: we require the same configuration for all Vulkan tests; this might not be ideal once
            // tests that require more 'obscure' properties start being developed.

            const std::vector<const char *> layer_names = {
                "VK_LAYER_KHRONOS_validation"
            };
            const std::vector<const char *> extension_names = {};

            thvk_VulkanRendererApplicationConfig_t app_config = {};
            app_config.application_name = (char *) _test_name.c_str();

            thvk_VulkanRendererExtensionConfig_t extension_config = {};
            extension_config.layer_names = (char **) layer_names.data();
            extension_config.layer_count = (int) layer_names.size();

            thvk_VulkanRendererConfig_t config = {};
            config.application_config = app_config;
            config.extension_config = extension_config;

            th_RendererDescriptor_t renderer_info = {};
            renderer_info.api_name = "vulkan";
            renderer_info.api_version = { 1, 3, 0 };
            renderer_info.renderer_config = &config;

            _renderer = th_CreateRenderer(renderer_info, _debugger);

            if (!_renderer) {
                Print("Failed to create renderer!");
            } else {
                Print("Created renderer");
            }
        }
    #endif
}
