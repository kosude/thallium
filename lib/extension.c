/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"

#include "cmake_modules.h"

#include "utils/log.h"
#include "utils/primitive.h"

#ifdef THALLIUM_VULKAN_INCL
    static const int _ValidateVulkanLayers(const th_RendererExtensionDescriptor_t descriptor) {
        for (unsigned int i = 0; i < (unsigned int) descriptor.vulkan.layerCount; i++) {
            if (!thvk_ValidateLayer(descriptor.vulkan.layerNames[i])) {
                th_Warn("When validating Vulkan layers: layer \"%s\" not valid", descriptor.vulkan.layerNames[i]);
                return 0;
            }
        }

        return 1;
    }

    static const int _ValidateVulkanIExtensions(const th_RendererExtensionDescriptor_t descriptor) {
        for (unsigned int i = 0; i < (unsigned int) descriptor.vulkan.extensionCount; i++) {
            if (thvk_ValidateInstanceExtension(descriptor.vulkan.extensionNames[i], NULL)) {
                return 1;
            }

            // check extension against each layer if applicable
            if (descriptor.vulkan.layerNames) {
                for (unsigned int j = 0; j < (unsigned int) descriptor.vulkan.layerCount; j++) {
                    if (thvk_ValidateInstanceExtension(descriptor.vulkan.extensionNames[i], descriptor.vulkan.layerNames[j])) {
                        return 1;
                    }
                }
            }

            // if control reaches this point then the extension has not been found in core or any layers
            th_Warn("When validating Vulkan instance extensions: ext \"%s\" not valid", descriptor.vulkan.extensionNames[i]);
            return 0;
        }

        return 1;
    }
#endif


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

const int th_ValidateRendererExtensionDescriptor(const th_RendererExtensionDescriptor_t descriptor) {
#   ifdef THALLIUM_VULKAN_INCL
        if (descriptor.vulkan.layerNames && !ValidateVulkanLayers(descriptor)) {
            return 0;
        }
        if (descriptor.vulkan.extensionNames && !ValidateVulkanIExtensions(descriptor)) {
            return 0;
        }

        // if vulkan instance debug messenger is requested but the required extension is not specified then uh oh
        if (
            (descriptor.vulkan.flags & (THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_ALL_BIT | THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_TYPE_ALL_BIT))
            && !th_StringValueInArray(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, descriptor.vulkan.extensionNames, descriptor.vulkan.extensionCount)
        ) {
            return 0;
        }
#   endif

    return 1;
}
