/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__vulkan__vk_instance_h__
#define __TL__internal__vulkan__vk_instance_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/fwd.h"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#include <cutils/carray/carray.h>

/**
 * @brief Create a Vulkan instance object.
 *
 * This function creates a Vulkan instance object to support the renderer features in the given `requirements` struct.
 *
 * Note that the availability of these features is also implicitly checked, and extensions/layers that fit unavailable features will not be enabled.
 *
 * The names of the successfully enabled Vulkan layers is returned into the `out_layer_names` array, as with the successfully enabled extension names
 * into `out_extension_names`.
 *
 * @param application_info Vulkan application info struct
 * @param requirements Renderer feature requirements for the instance to support.
 * @param out_layer_names Pointer to the dynamic array into which names of successfully enabled layers will be output.
 * @param out_extension_names Pointer to the dynamic array into which names of successfully enabled instance-level extensions will be output.
 * @param debugger NULL or a debugger for function debugging
 *
 * @return The new Vulkan instance
 */
VkInstance TLVK_CreateInstance(
    const VkApplicationInfo application_info,
    const TL_RendererFeatures_t requirements,
    carray_t *const out_layer_names,
    carray_t *const out_extension_names,
    const TL_Debugger_t *const debugger
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
