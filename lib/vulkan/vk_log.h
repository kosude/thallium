/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file vk_log.h
 * @brief Some internal functions for displaying information about Vulkan structures.
 *
 * @defgroup vk_log Vulkan-specific logging functions
 * @brief Internally used functions for printing Vulkan information.
 * @ingroup vulkan
 *
 * Some internally functions for displaying information about Vulkan structures.
 *
 * Normally, the information must be supplied - these functions just provide ways of printing them
 * as formatted strings.
 *
 * @note The majority of the functions in this group only have any effect if `THALLIUM_DEBUG_LAYER`
 * is enabled due to the high frequency of heavy string operations.
 */

#pragma once
#ifndef __thallium_vk_log_h__
#define __thallium_vk_log_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/fwd.h"

#include <volk/volk.h>

/**
 * @ingroup vk_log
 * @brief Print a formatted string containing the supplied Vulkan instance information.
 *
 * This function prints a formatted string containing the supplied Vulkan instance information.
 *
 * @param dbg Debugger to report message(s) to.
 * @param layer_count Amount of elements in `layer_names`
 * @param layer_names Array of layer names
 * @param extension_count Amount of elements in `extension_names`
 * @param extension_names Array of instance extension names
 */
void thvk_PrintVkInstanceInfo(
    const th_Debugger_t *dbg,
    const unsigned int layer_count,
    const char **layer_names,
    const unsigned int extension_count,
    const char **extension_names
);

/**
 * @ingroup vk_log
 * @brief Print a formatted string containing the supplied Vulkan logical device information.
 *
 * This function prints a formatted string containing the supplied Vulkan logical device information.
 *
 * @param dbg Debugger to report message(s) to.
 * @param physical_device_name Name of the physical device used to create the logical device
 * @param extension_count Amount of elements in `extension_names`
 * @param extension_names Array of enabled device extension names
 */
void thvk_PrintVkDeviceInfo(
    const th_Debugger_t *dbg,
    const char *physical_device_name,
    const unsigned int extension_count,
    const char **extension_names
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_vk_log_h__
