/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__core__context_api_h__
#define __TL__internal__core__context_api_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium_decl/enums.h"
#include "thallium_decl/fwd.h"

#include "thallium/core/version.h"

// SIZE_MAX means data block is not initialised
#define TL_CONTEXT_API_OBJECT_UNINITIALISED SIZE_MAX

/**
 * @brief A struct to hold a version struct for each abstracted API.
 *
 * This struct holds a version struct for each Thallium-abstracted API.
 */
typedef struct TL_ContextAPIVersions_t {
    /// @brief Vulkan API version
    TL_Version_t vulkan_version;
} TL_ContextAPIVersions_t;

/**
 * @brief Create the API objects needed to support the specified APIs and features. These objects are stored in the given context.
 *
 * This function creates the context-scope API objects needed to support the specified APIs and features, which are then stored in the given Thallium
 * context object at `context`.
 *
 * An example of an 'API object' is the `VkInstance` object, which will be created by this function if `apis` includes TL_RENDERER_API_VULKAN_BIT
 * (and if the Vulkan module was compiled).
 *
 * @note This function can only be called **once** per Thallium context object!
 *
 * @param context Context object into which the context-scope API objects will be stored
 * @param apis Bit field of all APIs to support
 * @param versions Minimum versions to request for each API
 * @param features Struct of combined renderer features to support
 * @param debugger NULL or Thallium debugger object
 *
 * @return False if error
 */
bool TL_ContextBlocksCreate(
    TL_Context_t *const context,
    const TL_RendererAPIFlags_t apis,
    const TL_ContextAPIVersions_t versions,
    const TL_RendererFeatures_t features,
    const TL_Debugger_t *const debugger
);

/**
 * @brief Destroy the API objects stored in the given Thallium context object.
 *
 * This function destroys the API objects stored in the given Thallium context object, using the appropriate API-specific functions to do so.
 *
 * @param context Context object to depopulate
 */
void TL_ContextBlocksDestroy(
    TL_Context_t *const context
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
