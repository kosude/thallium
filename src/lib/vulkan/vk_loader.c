/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "vk_loader.h"

#include "thallium/platform.h"

#include "utils/utils.h"

#include <volk/volk.h>

#include <string.h>

// flag to only load vulkan loader once
static bool __VOLK_INITIALISED = false;


void TLVK_LoaderInit(const TL_Debugger_t *const debugger) {
    if (!__VOLK_INITIALISED) {
        if (volkInitialize()) {
            TL_ReportMessage(debugger, TL_DEBUG_SEVERITY_FATAL_BIT, TL_DEBUG_SOURCE_THALLIUM_BIT, "Failed to load Vulkan");
            return;
        }
        __VOLK_INITIALISED = true;
    }
}

TLVK_FuncSet_t TLVK_LoaderFuncSetLoad(const VkDevice device) {
    // volk does all the heavy lifting
    struct VolkDeviceTable dtable;
    volkLoadDeviceTable(&dtable, device);

    TLVK_FuncSet_t funcset;
    memset(&funcset, 0, sizeof(TLVK_FuncSet_t)); // ensure unloaded functions are null

    // sizeof VolkDeviceTable must be equal to sizeof TLVK_FuncSet_t.
    memcpy(&funcset, &dtable, sizeof(TLVK_FuncSet_t));

    return funcset;
}

void TLVK_LoaderInstanceLoad(const VkInstance instance) {
    volkLoadInstanceOnly(instance);
}
