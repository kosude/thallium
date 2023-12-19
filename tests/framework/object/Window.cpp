/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "Window.hpp"

#include "../Utils.hpp"

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

#include <iostream>

#if defined(_THALLIUM_VULKAN_INCL)
#   include "thallium_vulkan.h"
#endif

namespace TLTests::Framework {
    void Window::_InitGLFW() {
        glfwSetErrorCallback([](int e, const char *d) {
            Utils::Error("GLFW error code " + std::to_string(e) + " - " + d);
        });

        if (!glfwInit()) {
            // fatal error if fail
            Utils::Error("Failed to initialise GLFW", true);
        }
    }

    Window::Window(const uint32_t &width, const uint32_t &height, const std::string &title) {
        if (!_init) {
            _InitGLFW();
            _init = true;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        _handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

#       if defined(GLFW_EXPOSE_NATIVE_COCOA)
            id win = glfwGetCocoaWindow(_handle);
            _surface = TL_WindowSurfaceCreateCocoa(win, nullptr);
#       elif defined(GLFW_EXPOSE_NATIVE_X11)
            // note that Thallium *does* also support X11 (Xlib) instead of xcb, but we're using xcb here because X11 is stupid and has generic type
            // names like 'Window' and guess what this class is called :))

            xcb_connection_t *con = XGetXCBConnection(glfwGetX11Display());
            xcb_window_t win = glfwGetX11Window(_handle); // xcb ref == x11 ref

            _surface = TL_WindowSurfaceCreateXCB(con, win, nullptr);
#       endif
    }

    void Window::Destroy() {
        glfwDestroyWindow(_handle);
    }

    void Window::TerminateAPI() {
        glfwTerminate();
    }

    TL_Swapchain_t *Window::CreateSwapchain(const TL_Renderer_t *renderer) const {
        TL_SwapchainDescriptor_t scdescr = {};

        scdescr.window_surface = _surface;
        glfwGetFramebufferSize(_handle, (int *) &scdescr.resolution.width, (int *) &scdescr.resolution.height);

        return TL_SwapchainCreate(renderer, scdescr);
    }

    bool Window::ShouldClose() const {
        return glfwWindowShouldClose(_handle);
    }

    void Window::PollEvents() {
        glfwPollEvents();
    }
}
