/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"

#include "utils/log.h"

#include <string.h>

#define EMPTY_RENDERER (th_Renderer_t) { \
    .apiId = THALLIUM_API_ID_NULL, \
    .apiName = NULL \
}

const th_Renderer_t th_CreateRenderer(const char *const apiName) {
    // get ID from the given name

    th_ApiId_t id;
    if (!strcmp(apiName, "vulkan")) {
        id = THALLIUM_API_ID_VULKAN;
    } else {
        th_Error("Invalid graphics API specified in th_CreateRenderer()");
        return EMPTY_RENDERER;
    }

    return (th_Renderer_t) {
        .apiId = id,
        .apiName = apiName
    };
}
