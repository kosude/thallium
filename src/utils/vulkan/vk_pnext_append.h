/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__utils__vk_pnext_append_h__
#define __TL__internal__utils__vk_pnext_append_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @brief Append `obj` onto the end of the Vulkan pNext chain based from `p_next`.
 *
 * This function appends `obj` onto the end of the Vulkan pNext chain based from `p_next`.
 *
 * This function depends on unused pNext members to be NULL (which should really be the case as Vulkan depends on this as well) so make sure the final
 * struct in the chain has a pNext of NULL.
 *
 * @warning This function is recursive and depends on the memory alignment of the structs - this means it assumes that the first two members of
 * `obj` are `sType` and `pNext`, in that order. Use with caution!
 *
 * @param p_next pointer to a pNext member that is part of the chain
 * @param obj Vulkan object to set to the pNext in `base`
 */
void TLVK_AppendPNext(
    const void **const p_next,
    const void *const obj
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif //
