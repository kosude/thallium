/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#ifndef __tests__Window_hpp__
#define __tests__Window_hpp__

#include <cstdint>
#include <iosfwd>

typedef struct GLFWwindow GLFWwindow;

namespace TLTests::Framework {
    class Window {
    private:
        GLFWwindow *_handle;

        inline static bool _init; // GLFW init flag
        void _InitGLFW();

    public:
        inline GLFWwindow *GetHandle() const { return _handle; }

        Window(
            const uint32_t &width,
            const uint32_t &height,
            const std::string &title
        );

        void Destroy();
        static void TerminateAPI();
    };
}

#endif
