/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "vk_loader.h"

#include <volk/volk.h>

#include <stdbool.h>
#include <string.h>

// flag to only load vulkan loader once
static bool __VOLK_INITIALISED = false;

void TLVK_LoaderInit(void) {

    if (!__VOLK_INITIALISED) {
        volkInitialize(); // seemed to have spelt initialise wrong
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
