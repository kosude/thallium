/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"
#include "thallium_vulkan.h"

#include "utils/log.h"
#include "vk_instance.h"

#include <stdlib.h>


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

thvk_RenderSystem_t *thvk_CreateRenderSystem(const thvk_RenderSystemDescriptor_t descriptor, const th_Debugger_t *debugger) {
    thvk_RenderSystem_t *r = malloc(sizeof(thvk_RenderSystem_t));
    if (!r) {
        th_Fatal(debugger, "MALLOC fault in th_CreateRenderer!");
        return NULL;
    }

    if (!thvk_CreateInstance(&r->instance, descriptor)) {
        return NULL;
    }

    th_Note(debugger, "Created Vulkan render system at %p", r);

    VkDebugUtilsMessengerCallbackDataEXT data = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT,
        .pNext = NULL,
        .flags = 0,
        .pMessageIdName = "Hello",
        .messageIdNumber = 0,
        .pMessage = "Hello world",
        .queueLabelCount = 0,
        .pQueueLabels = NULL,
        .cmdBufLabelCount = 0,
        .pCmdBufLabels = NULL,
        .objectCount = 0,
        .pObjects = NULL
    };

    PFN_vkSubmitDebugUtilsMessageEXT func = (PFN_vkSubmitDebugUtilsMessageEXT) vkGetInstanceProcAddr(r->instance, "vkSubmitDebugUtilsMessageEXT");

    func(
        r->instance,
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
        &data);

    return r;
}

const int thvk_DestroyRenderSystem(thvk_RenderSystem_t *renderSystem) {
    vkDestroyInstance(renderSystem->instance, NULL);

    return 1;
}
