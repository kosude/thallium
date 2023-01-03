/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"

#include "utils/log.h"

#include <stdio.h>
#include <string.h>

#define EMPTY_RENDERER (th_Renderer_t) { \
    .apiId = THALLIUM_API_ID_NULL, \
    .apiName = NULL \
}

const th_Renderer_t th_CreateRenderer(const char *const apiName) {
    // get ID from the given name
    th_ApiId_t id;

    if (!strcmp(apiName, "vulkan")) {
#       ifdef THALLIUM_VULKAN_INCL
            id = THALLIUM_API_ID_VULKAN;
#       else
            th_Error("th_CreateRenderer() attempted to create renderer for API which was not compiled (\"%s\")", apiName);
            th_Hint("Recompile Thallium with the -DTHALLIUM_BUILD_MODULE_VULKAN=ON flag!");
            return EMPTY_RENDERER;
#       endif
    } else {
        th_Error("Invalid graphics API \"%s\" specified in th_CreateRenderer()", apiName);
        return EMPTY_RENDERER;
    }

    return (th_Renderer_t) {
        .apiId = id,
        .apiName = apiName
    };
}
