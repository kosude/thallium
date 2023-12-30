/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium.h"

#if defined(__linux__)
#   include <xcb/xcb.h>
#   include <X11/Xlib-xcb.h>

#   define GLFW_EXPOSE_NATIVE_X11
#elif defined(__APPLE__)
#   if defined(__OBJC__)
#       import <Cocoa/Cocoa.h>
#   else
#       include <ApplicationServices/ApplicationServices.h>
#       include <objc/objc.h>
#   endif

#   define GLFW_EXPOSE_NATIVE_COCOA
#elif defined(_WIN32)
#   error Useless operating systems are not yet supported by the Thallium WSI abstraction set
#endif

#include "glfw/include/GLFW/glfw3.h"
#include "glfw/include/GLFW/glfw3native.h"

#include "cutils/cio/cio.h"
#include "cutils/cio/cioenum.h"

#include <iostream>
#include <ctime>

#if defined(_THALLIUM_VULKAN_INCL)
#   include "thallium_vulkan.h"
#endif

static std::string __GetCurrentDateTime(const bool &with_date = false);

static void __DebugCallback(char *msg, TL_DebugSeverityFlags_t sev, TL_DebugSourceFlags_t src, void *ptr);


TL_Debugger_t *debugger;
TL_Context_t *context;
TL_Renderer_t *renderer;
TL_Swapchain_t *swapchain;

GLFWwindow *window;
TL_WindowSurface_t *window_surface;

int main() {
    // Create the debugger

    TL_DebuggerDescriptor_t debuggerdesc = {};
    (int &) debuggerdesc.severities = TL_DEBUG_SEVERITY_ALL_BIT;// TL_DEBUG_SEVERITY_WARNING_BIT | TL_DEBUG_SEVERITY_ERROR_BIT | TL_DEBUG_SEVERITY_FATAL_BIT;
    (int &) debuggerdesc.sources = TL_DEBUG_SOURCE_ALL_BIT;
    debuggerdesc.callback = __DebugCallback;

    debugger = TL_DebuggerCreate(debuggerdesc);
    if (!debugger) {
        std::cerr << "Failed to create debugger" << std::endl;
        return 1;
    }

    // debugger attachment will be used to attach debugger to context
    TL_DebuggerAttachmentDescriptor_t debugger_attachment = {};
    debugger_attachment.debugger = debugger;


    // Create the context

    TL_ContextDescriptor_t contextdesc = {};
    contextdesc.debug_attachment_descriptor = &debugger_attachment;

    context = TL_ContextCreate(contextdesc, debugger);
    if (!context) {
        std::cerr << "Failed to create context" << std::endl;
        return 1;
    }


    // Create the renderer

    TL_RendererFeatures_t rendererfeats = {};
    rendererfeats.presentation = true;

    TL_RendererDescriptor_t rendererdesc = {};
    rendererdesc.api = TL_RENDERER_API_VULKAN_BIT;
    rendererdesc.api_version = { 1, 3, 0 };
    rendererdesc.requirements = rendererfeats;

    TL_Renderer_t **rendererptr = &renderer;

    uint32_t renderersucc = TL_RendererCreate(
        context, 1,
        &rendererdesc,
        &rendererptr,
        debugger);
    if (renderersucc < 1) {
        std::cerr << "Failed to create renderer" << std::endl;
        return 1;
    }


    // Create the window

    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW" << std::endl;
        return 1;
    }

    window = glfwCreateWindow(640, 480, "Standalone test", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        return 1;
    }

    // window surface for interfacing with Thallium swapchains
#   if defined(GLFW_EXPOSE_NATIVE_COCOA)
        window_surface = TL_WindowSurfaceCreateCocoa(glfwGetCocoaWindow(window), debugger);
#   elif defined(GLFW_EXPOSE_NATIVE_X11)
        window_surface = TL_WindowSurfaceCreateXlib(glfwGetX11Display(), glfwGetX11Window(window), debugger);
#   endif
    if (!window_surface) {
        std::cerr << "Failed to create window" << std::endl;
        return 1;
    }


    // Initially create the swapchain

    TL_SwapchainDescriptor_t swapchaindesc = {};
    swapchaindesc.resolution = { 640, 480 };
    swapchaindesc.window_surface = window_surface;

    swapchain = TL_SwapchainCreate(renderer, swapchaindesc);
    if (!swapchain) {
        std::cerr << "Failed to create swapchain" << std::endl;
        return 1;
    }


    // Create the pipeline

    TL_Viewport_t viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) TL_SwapchainGetExtent(swapchain).width;
    viewport.height = (float) TL_SwapchainGetExtent(swapchain).height;
    viewport.min_depth = 0.0f;
    viewport.max_depth = 1.0f;

    TL_PipelineDescriptor_t pipelinedesc = {};
    pipelinedesc.type = TL_PIPELINE_TYPE_GRAPHICS;
    pipelinedesc.viewport_count = 1;
    pipelinedesc.viewports = &viewport;

    TL_Pipeline_t *pipeline = TL_PipelineCreate(renderer, pipelinedesc);
    if (!pipeline) {
        std::cerr << "Failed to create pipeline" << std::endl;
        return 1;
    }


    // Main loop

    std::cout << "-- MAIN LOOP BEGIN --" << std::endl;

    for (; !glfwWindowShouldClose(window);) {
        glfwPollEvents();
    }

    std::cout << "-- MAIN LOOP END --" << std::endl;


    // Cleanup

    TL_PipelineDestroy(pipeline);

    TL_SwapchainDestroy(swapchain);

    TL_RendererDestroy(renderer);
    TL_ContextDestroy(context);
    TL_DebuggerDestroy(debugger);

    glfwDestroyWindow(window);

    glfwTerminate();
}

static std::string __GetCurrentDateTime(const bool &with_date) {
    std::time_t t = std::time(nullptr);
    std::tm *tm = std::localtime(&t);

    char buf[64];

    if (tm) {
        if (with_date) {
            std::strftime(buf, 64, "%Y-%m-%d %X", tm);
        } else {
            std::strftime(buf, 64, "%X", tm);
        }

        return std::string { buf };
    }

    throw std::runtime_error("Date/time get failed");
}

static void __DebugCallback(char *msg, TL_DebugSeverityFlags_t sev, TL_DebugSourceFlags_t src, void *ptr) {
    std::cerr << ciocol(CIOCOL_GREY, 0xff).code << "tl";
    switch (src) {
        case TL_DEBUG_SOURCE_VULKAN_BIT:
            std::cerr << ciocol(CIOCOL_RED, 0xff).code << "VK ";
            break;
        default:
            std::cerr << "   ";
            break;
    }
    std::cerr << ciocol(CIOCOL_GREY, 0xff).code << ": ";

    std::cerr << __GetCurrentDateTime() << " ";

    switch (sev) {
        case TL_DEBUG_SEVERITY_VERBOSE_BIT:
            std::cerr << ciocol(CIOCOL_BLUE, 0xff).code << "LOG   ";
            break;
        case TL_DEBUG_SEVERITY_NOTIF_BIT:
            std::cerr << ciocol(CIOCOL_GREEN, 0xff).code << "NOTE  ";
            break;
        case TL_DEBUG_SEVERITY_WARNING_BIT:
            std::cerr << ciocol(CIOCOL_YELLOW, 0xff).code << "WARN  ";
            break;
        case TL_DEBUG_SEVERITY_ERROR_BIT:
            std::cerr << ciocol(CIOCOL_RED, 0xff).code << "ERROR ";
            break;
        case TL_DEBUG_SEVERITY_FATAL_BIT:
            std::cerr << ciocol(CIOCOL_RED, 0xff).code << "FATAL ";
            break;
        default:
            break;
    }

    std::cerr << ciocoldef().code << msg << std::endl;
}
