/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium.h"
#include "thallium_vulkan.h"

#include "glfw/include/GLFW/glfw3.h"

// TODO: cross-platform test
#define GLFW_EXPOSE_NATIVE_X11
#include "glfw/include/GLFW/glfw3native.h"

#include <X11/Xlib-xcb.h>

#include <iostream>
#include <vector>

TL_Debugger_t *DEBUGGER = nullptr;

TL_Context_t *CONTEXT = nullptr;

GLFWwindow *WINDOW1 = nullptr;

TL_Renderer_t *VULKAN_RENDERER = nullptr;

int main() {
#   if defined(DEBUG)
        // ..:: Create a debugger ::..

        TL_DebuggerDescriptor_t debugger_descriptor = {};

        (int &) debugger_descriptor.severities = TL_DEBUG_SEVERITY_ALL_BIT;// & ~TL_DEBUG_SEVERITY_VERBOSE_BIT & ~TL_DEBUG_SEVERITY_NOTIF_BIT;
        (int &) debugger_descriptor.sources = TL_DEBUG_SOURCE_THALLIUM_BIT | TL_DEBUG_SOURCE_VULKAN_BIT;

        DEBUGGER = TL_CreateDebugger(debugger_descriptor);
        if (!DEBUGGER) {
            std::cerr << "FAILED TO CREATE THALLIUM DEBUGGER" << std::endl;
            return 1;
        }
#   endif


    // ..:: Create the Thallium context ::..

    TL_DebuggerAttachmentDescriptor_t debugger_attachment_descriptor = {};
    debugger_attachment_descriptor.debugger = DEBUGGER;

    TL_ContextDescriptor_t context_descriptor = {};
    context_descriptor.debug_attachment_descriptor = &debugger_attachment_descriptor;

    CONTEXT = TL_CreateContext(context_descriptor, DEBUGGER);
    if (!CONTEXT) {
        std::cerr << "FAILED TO CREATE THALLIUM CONTEXT" << std::endl;
        return 1;
    }


    // ..:: Create the GLFW windows(s) ::..

    if (!glfwInit()) {
        std::cerr << "FAILED TO INIT GLFW" << std::endl;
        return 1;
    }

    WINDOW1 = glfwCreateWindow(640, 480, "Thallium test", nullptr, nullptr);

    if (!WINDOW1) {
        std::cerr << "FAILED TO CREATE GLFW WINDOW(S)" << std::endl;
        return 1;
    }

    TL_WindowSurface_t *window1_surface = TL_CreateXCBWindowSurface(XGetXCBConnection(glfwGetX11Display()), glfwGetX11Window(WINDOW1));


    // ..:: Create the renderer(s) ::..

    TLVK_RenderSystemDescriptor_t render_system_descriptor = {};

    TL_RendererDescriptor_t vulkan_renderer_descriptor = {};
    vulkan_renderer_descriptor.api = TL_RENDERER_API_VULKAN_BIT;
    vulkan_renderer_descriptor.api_version = { 1, 3, 0 };
    vulkan_renderer_descriptor.render_system_descriptor = &render_system_descriptor;
    vulkan_renderer_descriptor.requirements.presentation = true;

    // TL_Renderer_t *extra_vulkan_renderer = nullptr;

    // TL_RendererDescriptor_t extra_vulkan_renderer_descriptor = {};
    // extra_vulkan_renderer_descriptor.api = TL_RENDERER_API_VULKAN_BIT;
    // extra_vulkan_renderer_descriptor.api_version = { 1, 3, 0 };

    std::vector<TL_RendererDescriptor_t> renderer_descriptors = {
        vulkan_renderer_descriptor,
        // extra_vulkan_renderer_descriptor,
    };
    std::vector<TL_Renderer_t **> renderer_ptrs = {
        &VULKAN_RENDERER,
        // &extra_vulkan_renderer
    };

    if (TL_CreateRenderers(CONTEXT, renderer_ptrs.size(), renderer_descriptors.data(), renderer_ptrs.data(), DEBUGGER) < renderer_ptrs.size()) {
        std::cerr << "FAILED TO CREATE THALLIUM RENDERER(S)" << std::endl;
        return 1;
    }


    // ..:: Termination ::..

    TL_DestroyRenderer(VULKAN_RENDERER);
    // TL_DestroyRenderer(extra_vulkan_renderer);

    TL_DestroyContext(CONTEXT);

    TL_DestroyDebugger(DEBUGGER);

    glfwDestroyWindow(WINDOW1);
    glfwTerminate();

    return 0;
}
