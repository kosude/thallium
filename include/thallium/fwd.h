/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__thallium__fwd_h__
#define __TL__thallium__fwd_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

typedef struct TL_Context_t TL_Context_t;
typedef struct TL_ContextDescriptor_t TL_ContextDescriptor_t;

typedef struct TL_Debugger_t TL_Debugger_t;
typedef struct TL_DebuggerDescriptor_t TL_DebuggerDescriptor_t;
typedef struct TL_DebuggerAttachmentDescriptor_t TL_DebuggerAttachmentDescriptor_t;

typedef struct TL_Renderer_t TL_Renderer_t;
typedef struct TL_RendererDescriptor_t TL_RendererDescriptor_t;
typedef struct TL_RendererFeatures_t TL_RendererFeatures_t;

typedef struct TL_Version_t TL_Version_t;

typedef struct TL_WindowSurface_t TL_WindowSurface_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
