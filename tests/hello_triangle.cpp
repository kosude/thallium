/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium.h"

#include <iostream>
#include <vector>

TL_Context_t *CONTEXT = nullptr;

TL_Debugger_t *DEBUGGER = nullptr;

TL_Renderer_t *VULKAN_RENDERER = nullptr;

int main() {
#   if defined(DEBUG)
        // ..:: Create a debugger ::..

        TL_DebuggerDescriptor_t debugger_descriptor = {};
        (int &) debugger_descriptor.severities = TL_DEBUG_SEVERITY_ALL_BIT;
        (int &) debugger_descriptor.sources = TL_DEBUG_SOURCE_THALLIUM_BIT | TL_DEBUG_SOURCE_VULKAN_BIT;

        DEBUGGER = TL_CreateDebugger(debugger_descriptor);
        if (!DEBUGGER) {
            throw std::runtime_error("Failed to create debugger");
        }
#   endif


    // ..:: Create the Thallium context ::..

    TL_DebuggerAttachmentDescriptor_t debugger_attachment_descriptor = {};
    debugger_attachment_descriptor.debugger = DEBUGGER;

    TL_ContextDescriptor_t context_descriptor = {};
    context_descriptor.debug_attachment_descriptor = &debugger_attachment_descriptor;

    CONTEXT = TL_CreateContext(context_descriptor, DEBUGGER);
    if (!CONTEXT) {
        throw std::runtime_error("Failed to create Thallium context");
    }


    // ..:: Create the renderer(s) ::..

    TL_RendererDescriptor_t vulkan_renderer_descriptor = {};
    vulkan_renderer_descriptor.api = TL_RENDERER_API_VULKAN_BIT;
    vulkan_renderer_descriptor.api_version = { 1, 2, 0 };

    TL_Renderer_t *extra_vulkan_renderer = nullptr;

    TL_RendererDescriptor_t extra_vulkan_renderer_descriptor = {};
    extra_vulkan_renderer_descriptor.api = TL_RENDERER_API_VULKAN_BIT;
    extra_vulkan_renderer_descriptor.api_version = { 1, 3, 0 };
    extra_vulkan_renderer_descriptor.requirements.placeholder = true;

    std::vector<TL_RendererDescriptor_t> renderer_descriptors = {
        vulkan_renderer_descriptor,
        extra_vulkan_renderer_descriptor,
    };
    std::vector<TL_Renderer_t **> renderer_ptrs = {
        &VULKAN_RENDERER,
        &extra_vulkan_renderer
    };

    if (!TL_CreateRenderers(CONTEXT, renderer_ptrs.size(), renderer_descriptors.data(), renderer_ptrs.data(), DEBUGGER)) {
        throw std::runtime_error("Failed to create Thallium renderers");
    }


    // ..:: Termination ::..

    TL_DestroyRenderer(VULKAN_RENDERER);
    TL_DestroyRenderer(extra_vulkan_renderer);

    TL_DestroyContext(CONTEXT);

    TL_DestroyDebugger(DEBUGGER);

    return 0;
}
