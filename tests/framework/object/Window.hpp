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

#include "thallium.h"

typedef struct GLFWwindow GLFWwindow;

namespace TLTests::Framework {
    class Window {
    private:
        GLFWwindow *_handle;

        TL_WindowSurface_t *_surface;

        inline static bool _init; // GLFW init flag
        void _InitGLFW();

    public:
        inline GLFWwindow *GetHandle() const { return _handle; }
        inline TL_WindowSurface_t *GetSurface() const { return _surface; }

        Window(
            const uint32_t &width,
            const uint32_t &height,
            const std::string &title
        );

        void Destroy();
        static void TerminateAPI();

        TL_Swapchain_t *CreateSwapchain(
            const TL_Renderer_t *renderer
        ) const;

        bool ShouldClose() const;
        static void PollEvents();
    };
}

#endif
