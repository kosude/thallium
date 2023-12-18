/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium.h"

#include "lib/core/context_block.h"
#if defined(_THALLIUM_VULKAN_INCL)
#   include "lib/vulkan/vk_context_block.h"
#endif

#include "types/core/context.h"

#include "utils/utils.h"

#include <stdlib.h>
#include <string.h>

// static context pointer singleton
static TL_Context_t *__CONTEXT_PTR = NULL;


TL_Context_t *TL_ContextCreate(const TL_ContextDescriptor_t context_descriptor, const TL_Debugger_t *const debugger) {
    if (__CONTEXT_PTR) {
        TL_Warn(debugger, "Attempted to create context (one already exists at %p) - creating multiple contexts is illegal. Existing context was "
            "returned", __CONTEXT_PTR);

        return __CONTEXT_PTR;
    }

    TL_Context_t *context = malloc(sizeof(TL_Context_t));
    if (!context) {
        TL_Fatal(debugger, "MALLOC fault in call to TL_ContextCreate");
        return NULL;
    }

    TL_Log(debugger, "Allocated blank context at %p", context);

    context->state.api_objects_init = false;
    context->state.renderers_init = false;

    // attach debugger if specified in descriptor
    const TL_DebuggerAttachmentDescriptor_t *dbgattachment = context_descriptor.debug_attachment_descriptor;
    if (dbgattachment) {
        const TL_Debugger_t *dbgptr = dbgattachment->debugger;
        context->attached_debugger = dbgptr;

        TL_Log(debugger, "  Attached debugger %p", dbgptr);
    } else {
        context->attached_debugger = NULL;
    }

    // initialise to NULL as the data depends on which APIs are used.
    context->data_size = 0;
    context->data = NULL;

    __CONTEXT_PTR = context;

    return context;
}

void TL_ContextDestroy(TL_Context_t *const context) {
    if (!context) {
        return;
    }

    size_t data_size = context->data_size;

    // destroy API objects stored in the context if created
    if (data_size > 0) {
        TL_ContextBlocksDestroy(context);
    }

    memset(context, 0, sizeof(TL_Context_t));

    free(context);
}

bool TL_ContextBlocksCreate(TL_Context_t *const context, const TL_RendererAPIFlags_t apis, const TL_ContextAPIVersions_t versions,
    TL_RendererFeatures_t *const features, const TL_Debugger_t *const debugger)
{
    if (!context) {
        return false;
    }

    bool api_objects_init = context->state.api_objects_init;

    if (api_objects_init) {
        TL_Warn(debugger, "Attempted to create context API objects multiple times on the same context, which is illegal behaviour");
        return true;
    }

    // running total of size requirements for `data`
    size_t data_size = 0;

    // api object block offsets
    size_t vkoffset = TL_CONTEXT_BLOCK_UNINITIALISED;

    // get the size of the data that needs to be allocated as well as setting each block offset
    if (apis & TL_RENDERER_API_VULKAN_BIT) {
#       if defined(_THALLIUM_VULKAN_INCL)
            vkoffset = data_size;

            size_t size_add = sizeof(TLVK_ContextBlock_t);
            data_size += size_add;

            TL_Log(debugger, "%d bytes allocated for Vulkan context data block (may be padded) - offset %d (total data size now %d)", size_add,
                vkoffset, data_size);
#       else
            return false;
#       endif
    }

    context->data_size = data_size;
    context->vulkan_offset = vkoffset;

    // allocate space for the context data
    void *dataptr = malloc(data_size);
    if (!dataptr) {
        TL_Fatal(debugger, "MALLOC fault in call to TL_ContextBlocksCreate");
        return false;
    }
    context->data = dataptr;

    // we initialise the data to 0 so any .initialised booleans in the API-specific data blocks are interpreted as false. This is checked when
    // populating this data blocks.
    memset(dataptr, 0, data_size);

    TL_Log(debugger, "Allocated %d bytes of space for data blocks in Thallium context %p", data_size, context);
    TL_Log(debugger, "  Pointer to allocated data space is at %p", dataptr);

    // supporting Vulkan renderers
    if (apis & TL_RENDERER_API_VULKAN_BIT) {
#       if defined(_THALLIUM_VULKAN_INCL)
            TL_Note(debugger, "Requesting the Vulkan API at version %d.%d.%d", versions.vulkan_version.major, versions.vulkan_version.minor,
                versions.vulkan_version.patch);

            if (!TLVK_ContextBlockCreate(context, versions.vulkan_version, features, debugger)) {
                TL_Error(debugger, "Failed to populate Vulkan-specific context data block (%p + offset %d)", dataptr, vkoffset);
                return false;
            }

            TL_Log(debugger, "Vulkan context data block in context %p is located at %p (%p + offset %d)", context, (char *) dataptr + vkoffset,
                dataptr, vkoffset);
#       else
            return false;
#       endif
    }

    context->state.api_objects_init = true;

    return true;
}

void TL_ContextBlocksDestroy(TL_Context_t *const context) {
    if (!context) {
        return;
    }

    if (context->vulkan_offset != TL_CONTEXT_BLOCK_UNINITIALISED) {
#       if defined(_THALLIUM_VULKAN_INCL)
            TLVK_ContextBlockDestroy(context);
#       endif
    }

    free(context->data);
}
