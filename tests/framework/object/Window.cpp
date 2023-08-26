/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "Window.hpp"

#include "../Utils.hpp"

#include "glfw/include/GLFW/glfw3.h"

#define GLFW_EXPOSE_NATIVE_X11
#include "glfw/include/GLFW/glfw3native.h"

#include <xcb/xcb.h>
#include <X11/Xlib-xcb.h>

#include <iostream>

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

        xcb_connection_t *con = XGetXCBConnection(glfwGetX11Display());
        xcb_window_t win = glfwGetX11Window(_handle);

        _surface = TL_WindowSurfaceCreateXCB(con, win, nullptr);
        // _surface = TL_WindowSurfaceCreateXlib(glfwGetX11Display(), glfwGetX11Window(_handle), nullptr);
    }

    void Window::Destroy() {
        glfwDestroyWindow(_handle);
    }

    void Window::TerminateAPI() {
        glfwTerminate();
    }
}
