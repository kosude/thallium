/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__thallium__enumsvk_h__
#define __TL__thallium__enumsvk_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @brief Enumeration containing modes of physical device selection in a Vulkan renderer system.
 *
 * This enumeration contains modes of physical device selection available to be used when creating a Vulkan renderer system.
 */
typedef enum TLVK_PhysicalDeviceSelectionMode_t {
    /// @brief Use the most **optimal** physical device for the renderer system, based on queue families, and available extensions, device features,
    /// memory size, device type, etc. This mode **may** result in more processing but will usually result in the best GPU being used.
    /// **This is the default behaviour.**
    TLVK_PHYSICAL_DEVICE_SELECTION_MODE_OPTIMAL,
    /// @brief Use the **first** possible physical device for the renderer system. This could result in a device that does not support the required
    /// Vulkan extensions or features being used, leading to some missing features. Queue families will still be validated.
    TLVK_PHYSICAL_DEVICE_SELECTION_MODE_FIRST,
} TLVK_PhysicalDeviceSelectionMode_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
