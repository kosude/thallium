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

typedef struct TL_WindowSurface_t TL_WindowSurface_t;

typedef struct TL_Context_t TL_Context_t;
typedef struct TL_ContextDescriptor_t TL_ContextDescriptor_t;

typedef struct TL_Debugger_t TL_Debugger_t;
typedef struct TL_DebuggerDescriptor_t TL_DebuggerDescriptor_t;
typedef struct TL_DebuggerAttachmentDescriptor_t TL_DebuggerAttachmentDescriptor_t;

typedef struct TL_Pipeline_t TL_Pipeline_t;
typedef struct TL_PipelineDescriptor_t TL_PipelineDescriptor_t;
typedef struct TL_PipelineDepthTestDescriptor_t TL_PipelineDepthTestDescriptor_t;
typedef struct TL_PipelineRasterizerDescriptor_t TL_PipelineRasterizerDescriptor_t;

typedef struct TL_Renderer_t TL_Renderer_t;
typedef struct TL_RendererDescriptor_t TL_RendererDescriptor_t;
typedef struct TL_RendererFeatures_t TL_RendererFeatures_t;

typedef struct TL_Swapchain_t TL_Swapchain_t;
typedef struct TL_SwapchainDescriptor_t TL_SwapchainDescriptor_t;

typedef struct TL_Version_t TL_Version_t;

typedef struct TL_Viewport_t TL_Viewport_t;
typedef struct TL_Extent2D_t TL_Extent2D_t;
typedef struct TL_Offset2D_t TL_Offset2D_t;
typedef struct TL_Rect2D_t TL_Rect2D_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
