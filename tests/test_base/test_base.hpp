/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#pragma once
#ifndef __test_base_hpp__
#define __test_base_hpp__

#include "thallium.h"

#include <iostream>

namespace thallium_tests {
    enum class TestApi {
#       ifdef THALLIUM_VULKAN_INCL
            Vulkan
#       endif
    };

    template <TestApi Api>
    class TestBase {
    private:
        std::string _test_name;

        th_Renderer_t *_renderer;
        th_Debugger_t *_debugger;

        void _CreateRenderer();

        inline void _CreateDebugger() {
            _debugger = th_CreateDebugger(
                (th_DebugSeverity_t) (
                    THALLIUM_DEBUG_SEVERITY_VERBOSE_BIT |
                    THALLIUM_DEBUG_SEVERITY_NOTIF_BIT |
                    THALLIUM_DEBUG_SEVERITY_WARNING_BIT |
                    THALLIUM_DEBUG_SEVERITY_ERROR_BIT |
                    THALLIUM_DEBUG_SEVERITY_FATAL_BIT
                ),
                (th_DebugType_t) (
                    THALLIUM_DEBUG_TYPE_CORE_BIT |
                    THALLIUM_DEBUG_TYPE_VULKAN_BIT
                )
            );

            if (!_debugger) {
                Print("Failed to create debugger!");
            } else {
                Print("Created debugger");
            }
        }

    public:
        inline TestBase(const std::string &test_name) : _test_name(test_name) {
            Print("Thallium ", THALLIUM_VERSION);

            _CreateDebugger();
            _CreateRenderer();
        }

        inline ~TestBase() {
            th_DestroyRenderer(_renderer);
            th_DestroyDebugger(_debugger);
        }

        // Print a formatted message to stdout.
        template <typename... Args>
        inline void Print(Args... args) {
            (std::cout << "[[ " << _test_name << " ]] >>> " << ... << args) << std::endl;
        }

        const inline th_Renderer_t *GetRenderer() const { return _renderer; }
        const inline th_Debugger_t *GetDebugger() const { return _debugger; }
    };
}

#endif // !__test_base_hpp__
