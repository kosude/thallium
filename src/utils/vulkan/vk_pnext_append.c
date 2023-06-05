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

    // we are assuming next_obj is a Vulkan struct and there is no padding (!)
    // offset assumes pNext is the second member of the struct following sType.
    // FIXME: I don't know why, but the size of the enum is reported as half of what it seems to actually be, so I'm multiplying it
    //        by 2. This solution works, but it is obviously idiotic and probably not portable. FIX IT YOU IDIOT! (i'm the idiot)
    size_t offset = sizeof(VkStructureType) * 2;

    // get the pNext member of the next object to traverse
    const void **next_obj_pnext = (const void **) ((char *) next_obj + offset);
    TLVK_AppendPNext(next_obj_pnext, obj);
}
