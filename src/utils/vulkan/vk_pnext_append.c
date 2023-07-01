/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "vk_pnext_append.h"

#include "thallium/platform.h"

#include <volk/volk.h>

void TLVK_AppendPNext(const void **const p_next, const void *const obj) {
    const void *next_obj = *p_next;

    if (!next_obj) {
        // at this point we have reached the end of the chain and can append obj.
        *p_next = obj;

        return;
    }

    // if the pNext member is not NULL then we are still traversing through the chain...

    // offset assumes pNext is the second member of the struct following sType.
    // we are also assuming next_obj is a Vulkan struct, multiplying by 2 because of memory padding added prior to pNext as it is a void ptr.
    // NOTE: this may bork on random platforms if the struct's padding is wacky, I'm not really sure how to fix that.
    size_t offset = sizeof(VkStructureType) * 2;

    // get the pNext member of the next object to traverse
    const void **next_obj_pnext = (const void **) ((char *) next_obj + offset);
    TLVK_AppendPNext(next_obj_pnext, obj);
}
