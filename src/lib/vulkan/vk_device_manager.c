/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium_vulkan.h"

#include "utils/io/log.h"

#include "types/vulkan/vk_device_manager.h"
#include "types/core/renderer.h"

#include <stdlib.h>

TLVK_DeviceManager_t *TLVK_CreateDeviceManager(const TL_Renderer_t *const renderer, const TLVK_DeviceManagerDescriptor_t descriptor) {
    if (!renderer) {
        return NULL;
    }

    const TL_Debugger_t *debugger = renderer->debugger;

    TLVK_DeviceManager_t *device_manager = malloc(sizeof(TLVK_DeviceManager_t));
    if (!device_manager) {
        TL_Fatal(renderer->debugger, "MALLOC fault in call to TLVK_CreateDeviceManager");
        return NULL;
    }

    TL_Log(debugger, "Allocated memory for Vulkan device manager at %p", device_manager);

    // TODO: vulkan device manager
    device_manager->placeholder = descriptor.placeholder;

    return device_manager;
}

void TLVK_DestroyDeviceManager(TLVK_DeviceManager_t *const device_manager) {
    if (!device_manager) {
        return;
    }

    free(device_manager);
}
