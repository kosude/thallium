/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "Window.hpp"

#include "../Utils.hpp"

#include "glfw/include/GLFW/glfw3.h"
#include <iostream>

namespace TLTests::Framework {
    void Window::_InitGLFW() {
        glfwSetErrorCallback([](int e, const char *d) {
            Error("GLFW error code " + std::to_string(e) + " - " + d);
        });

        if (!glfwInit()) {
            // fatal error if fail
            Error("Failed to initialise GLFW", true);
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
    }

    void Window::Destroy() {
        glfwDestroyWindow(_handle);
    }

    void Window::TerminateAPI() {
        glfwTerminate();
    }
}
