/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium.h"
#include "lib/core/context_api.h"

#include "types/core/context.h"

#include "utils/utils.h"

#include <stdlib.h>
#include <string.h>

#if defined(THALLIUM_VULKAN_INCL)
#   include "lib/vulkan/vk_context_api_object.h"
#endif

// static context pointer singleton
static TL_Context_t *__CONTEXT_PTR = NULL;

TL_Context_t *TL_CreateContext(const TL_ContextDescriptor_t context_descriptor, const TL_Debugger_t *const debugger) {
    if (__CONTEXT_PTR) {
        TL_Warn(debugger, "Attempted to create context (one already exists at %p) - creating multiple contexts is illegal. Existing context was "
            "returned", __CONTEXT_PTR);

        return __CONTEXT_PTR;
    }

    TL_Context_t *context = malloc(sizeof(TL_Context_t));
    if (!context) {
        TL_Fatal(debugger, "MALLOC fault in call to TL_CreateContext");
        return NULL;
    }

    TL_Log(debugger, "Allocated blank context at %p", context);

    context->api_objects_init = false;
    context->renderers_init = false;

    // attach debugger if specified in descriptor
    if (context_descriptor.debug_attachment_descriptor) {
        context->attached_debugger = context_descriptor.debug_attachment_descriptor->debugger;

        TL_Log(debugger, "  Attached debugger %p", context->attached_debugger);
    } else {
        context->attached_debugger = NULL;
    }

    // the handles of any data blocks that are initialised will be set accordingly in TL_CreateContextAPIObjects
    context->vulkan_offset = TL_CONTEXT_API_OBJECT_UNINITIALISED;

    // initialise to NULL as the data depends on which APIs are used.
    context->data_size = 0;
    context->data = NULL;

    __CONTEXT_PTR = context;

    return context;
}

void TL_DestroyContext(TL_Context_t *const context) {
    if (!context) {
        return;
    }

    // destroy API objects stored in the context if created
    if (context->data_size > 0) {
        TL_DestroyContextAPIObjects(context);
    }

    memset(context, 0, sizeof(TL_Context_t));

    free(context);
}

bool TL_CreateContextAPIObjects(TL_Context_t *const context, const TL_RendererAPIFlags_t apis, const TL_ContextAPIVersions_t versions,
    const TL_RendererFeatures_t features, const TL_Debugger_t *const debugger)
{
    if (context->api_objects_init) {
        TL_Warn(debugger, "Attempted to create context API objects multiple times on the same context, which is illegal behaviour");
        return true;
    }

    size_t data_size = 0;

    // get the size of the data that needs to be allocated as well as setting each block offset
    if (apis & TL_RENDERER_API_VULKAN_BIT) {
#       if defined(THALLIUM_VULKAN_INCL)
            context->vulkan_offset = data_size;

            size_t size_add = sizeof(TLVK_ContextVulkanBlock_t);
            data_size += size_add;

            TL_Log(debugger, "%d bytes allocated for Vulkan context data block (may be padded) - offset %d (total data size now %d)", size_add,
                context->vulkan_offset, data_size);
#       else
            return false;
#       endif
    }

    context->data_size = data_size;

    // allocate space the context data
    context->data = malloc(data_size);
    if (!context->data) {
        TL_Fatal(debugger, "MALLOC fault in call to TL_CreateContextAPIObjects");
        return false;
    }

    // we initialise the data to 0 so any .initialised booleans in the API-specific data blocks are interpreted as false. This is checked when
    // populating this data blocks.
    memset(context->data, 0, data_size);

    TL_Log(debugger, "Allocated %d bytes of space for data blocks in Thallium context %p", data_size, context);
    TL_Log(debugger, "  Pointer to allocated data space is at %p", context->data);

    // supporting Vulkan renderers
    if (apis & TL_RENDERER_API_VULKAN_BIT) {
#       if defined(THALLIUM_VULKAN_INCL)
            TL_Note(debugger, "Requesting the Vulkan API at version %d.%d.%d", versions.vulkan_version.major, versions.vulkan_version.minor,
                versions.vulkan_version.patch);

            if (!TLVK_CreateContextVulkanBlock(context, versions.vulkan_version, features, debugger)) {
                TL_Error(debugger, "Failed to populate Vulkan-specific context data block (%p + offset %d)", context->data, context->vulkan_offset);
                return false;
            }

            TL_Log(debugger, "Vulkan context data block in context %p is located at %p (%p + offset %d)", context,
                (char *) context->data + context->vulkan_offset, context->data, context->vulkan_offset);
#       else
            return false;
#       endif
    }

    context->api_objects_init = true;

    return true;
}

void TL_DestroyContextAPIObjects(TL_Context_t *const context) {
    if (!context) {
        return;
    }

    if (context->vulkan_offset != TL_CONTEXT_API_OBJECT_UNINITIALISED) {
#       if defined(THALLIUM_VULKAN_INCL)
            TLVK_DestroyContextVulkanBlock(context);
#       endif
    }

    free(context->data);
}
