/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file renderer.h
 * @brief Renderer structure(s) and related functions.
 *
 * @defgroup renderer Managing renderer objects
 * @brief Creating and managing renderer objects
 *
 * Renderer structure(s) and related functions.
 *
 * ### Explanation of terminology
 *
 * In Thallium, a **graphics API** such as OpenGL or Vulkan is represented by a structure called a
 * **renderer**. When building Thallium you need to make sure the appropriate **module** is compiled
 * for the desired renderer; otherwise, you will be attempting to use undefined functions.
 *
 * To summarise:
 *  - **API**: OpenGL, Vulkan, Metal, etc
 *  - **renderer**: user-created structure
 *  - **module**: section of the library source code to optionally be compiled
 */

#pragma once
#ifndef _THALLIUM_PUB_RENDERER_H
#define _THALLIUM_PUB_RENDERER_H
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/debug.h"

/**
 * @ingroup renderer
 * @brief An enumeration to represent options of graphics apis
 *
 * An enumeration to represent options of graphics APIs.
 */
typedef enum th_ApiId_t {
    /// @brief Null API
    THALLIUM_API_ID_NULL = 0x00,
    /// @brief The [Vulkan API](https://www.vulkan.org/)
    THALLIUM_API_ID_VULKAN = 0x01
} th_ApiId_t;

/**
 * @ingroup renderer
 * @brief A structure to represent a graphics API.
 *
 * This structure provides a representation of a certain graphics API.
 *
 * @warning Setting the contents of the structure at runtime will almost certainly result in an
 * exception. If you want to use a different API for some functions, **create another renderer.**
 */
typedef struct th_Renderer_t {
    /// @brief The ID of the graphics API specified to the renderer.
    const th_ApiId_t apiId;
    /// @brief The name of the graphics API specified to the renderer.
    const char *const apiName;
} th_Renderer_t;

/**
 * @ingroup renderer
 * @brief Create, populate, and return a new renderer struct.
 *
 * This function constructs and returns a renderer struct.
 *
 * If `apiName` is invalid, then an 'empty' struct is returned (e.g. apiId set to NULL). Note that
 * this includes API modules that were not included when Thallium was compiled.
 *
 * @param apiName name of the graphics API to represent
 * @return The new renderer object
 */
const th_Renderer_t th_CreateRenderer(
    const char *const apiName
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_PUB_RENDERER_H
