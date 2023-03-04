/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file fwd.h
 * @brief Thallium library structure forward declarations
 *
 * Include this file to recieve forward declarations of public Thallium types. This may be useful if
 * you want to avoid including @ref thallium.h for whatever reason.
 */

#pragma once
#ifndef __thallium_fwd_h__
#define __thallium_fwd_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/** @cond */

typedef struct th_Version_t th_Version_t;

typedef struct th_Renderer_t th_Renderer_t;
typedef struct th_RendererDescriptor_t th_RendererDescriptor_t;

typedef struct th_Debugger_t th_Debugger_t;

/** @endcond */

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_fwd_h__
