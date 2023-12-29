/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__vulkan__vk_pipeline_system_h__
#define __TL__vulkan__vk_pipeline_system_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/core/pipeline.h"
#include "thallium_decl/fwdvk.h"

/**
 * @brief A pipeline system (AKA 'pipeline state object system') to hold Vulkan pipelining data.
 *
 * This opaque struct represents per-[pipeline](@ref TL_Pipeline_t) data for Vulkan pipeline objects.
 *
 * @sa @ref TLVK_PipelineSystemCreate()
 * @sa @ref TLVK_PipelineSystemDestroy()
 */
typedef struct TLVK_PipelineSystem_t TLVK_PipelineSystem_t;

// TODO: needed?
// /**
//  * @brief Descriptor struct to configure the creation of a Thallium pipeline system for Vulkan.
//  *
//  * This descriptor structure provides options for the creation of Thallium pipeline systems.
//  */
// typedef struct TLVK_PipelineSystemDescriptor_t {
// } TLVK_PipelineSystemDescriptor_t;

/**
 * @brief Create a heap-allocated Vulkan pipeline (PSO) system with either graphics, compute, or ray tracing capabilities.
 *
 * This function creates a new heap-allocated Vulkan pipeline system, including its
 * [Vulkan pipeline state object](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPipeline.html), and returns a handle to it. NULL
 * will be returned instead if there were any errors in pipeline creation.
 *
 * @param renderer_system A valid Thallium Vulkan renderer system object
 * @param descriptor a Thallium pipeline descriptor
 * @return The new Vulkan PSO system
 */
TLVK_PipelineSystem_t *TLVK_PipelineSystemCreate(
    const TLVK_RendererSystem_t *const renderer_system,
    const TL_PipelineDescriptor_t descriptor
);

/**
 * @brief Free the given Thallium Vulkan pipeline state system object.
 *
 * This function frees the specified pipeline system object.
 *
 * @param pipeline_system Pointer to the Thallium Vulkan pipeline system to free.
 *
 * @sa @ref TLVK_PipelineSystem_t
 * @sa @ref TLVK_PipelineSystemCreate()
 */
void TLVK_PipelineSystemDestroy(
    TLVK_PipelineSystem_t *const pipeline_system
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
