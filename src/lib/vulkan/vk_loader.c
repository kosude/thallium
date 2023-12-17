/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "vk_loader.h"

#include <volk/volk.h>

#include <string.h>

TLVK_FuncSet_t TLVK_FuncSetLoad(const VkDevice device) {
    // volk does all the heavy lifting
    struct VolkDeviceTable dtable;
    volkLoadDeviceTable(&dtable, device);

    TLVK_FuncSet_t funcset;
    memset(&funcset, 0, sizeof(TLVK_FuncSet_t)); // ensure unloaded functions are null

    // sizeof VolkDeviceTable must be equal to sizeof TLVK_FuncSet_t.
    memcpy(&funcset, &dtable, sizeof(TLVK_FuncSet_t));

    return funcset;
}
