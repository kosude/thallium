/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "Test.hpp"

#if defined(_THALLIUM_VULKAN_INCL)
#   include "thallium_vulkan.h"
#endif

#include "Utils.hpp"

#include <algorithm>
#include <iostream>

namespace TLTests::Framework {
    TL_RendererFeatures_t Test::_GetRendererRequirements() {
        TL_RendererFeatures_t features = {};

        features.presentation = true;

        return features;
    }

    Test::Test()
        : _renderers(), _renderer_descriptors()
    {
        // create debugger (if in debug mode)

#       if defined(_DEBUG)
            TL_DebuggerDescriptor_t debugger_descriptor = {};

            reinterpret_cast<int &>(debugger_descriptor.severities) =
                TL_DEBUG_SEVERITY_VERBOSE_BIT |
                TL_DEBUG_SEVERITY_NOTIF_BIT |
                TL_DEBUG_SEVERITY_WARNING_BIT |
                TL_DEBUG_SEVERITY_ERROR_BIT |
                TL_DEBUG_SEVERITY_FATAL_BIT;

            debugger_descriptor.sources = TL_DEBUG_SOURCE_ALL_BIT;

            // debug callback
            debugger_descriptor.callback = Utils::DebugCallback;

            _debugger = TL_DebuggerCreate(debugger_descriptor);
            if (!_debugger) {
                Utils::Error(std::string{"Failed to create debugger"});
            }

            // attachment for attaching the debugger to the context
            TL_DebuggerAttachmentDescriptor_t debugger_attachment = {};
            debugger_attachment.debugger = _debugger;
#       else
            _debugger = nullptr;
#       endif

        // create context

        TL_ContextDescriptor_t context_descriptor = {};
#       if defined(_DEBUG)
            context_descriptor.debug_attachment_descriptor = &debugger_attachment;
#       endif

        _context = TL_ContextCreate(context_descriptor, _debugger);
        if (!_context) {
            Utils::Error(std::string{"Failed to create Thallium context"});
        }

        Utils::Log(std::string{"Starting Thallium test"});
    }

    void Test::Destroy() {
        // destroy renderers
        for (auto r : _renderers) {
            TL_RendererDestroy(r);
        }

        // destroy context
        TL_ContextDestroy(_context);

        // destroy debugger
        TL_DebuggerDestroy(_debugger);
    }

    // description for VULKAN renderers...
    template<>
    void Test::AddRenderer<GraphicsAPI::Vulkan>() {
#       if defined(_THALLIUM_VULKAN_INCL)
            TL_RendererDescriptor_t renderer_descriptor = {};

            renderer_descriptor.api = TL_RENDERER_API_VULKAN_BIT;
            renderer_descriptor.api_version = TL_Version_t { 1, 3, 0 };

            renderer_descriptor.requirements = _GetRendererRequirements();

            TLVK_RendererSystemDescriptor_t vk_renderer_system_descriptor = {};
            vk_renderer_system_descriptor.physical_device_mode = TLVK_PHYSICAL_DEVICE_SELECTION_MODE_OPTIMAL;

            renderer_descriptor.renderer_system_descriptor = &vk_renderer_system_descriptor;

            _renderer_descriptors.push_back(renderer_descriptor);
            _renderers.push_back(nullptr); // pointer will be populated when renderers are created
#       else
            Utils::Error("Can't add Vulkan renderer as Thallium's Vulkan module was not compiled");
#       endif
    }

    void Test::CreateRenderers() {
        std::vector<TL_Renderer_t **> renderer_ptrs(_renderers.size());
        std::transform(_renderers.begin(), _renderers.end(), renderer_ptrs.begin(), [](TL_Renderer_t *&r) { return &r; });

        uint32_t c = TL_RendererCreate(_context, _renderers.size(), _renderer_descriptors.data(), renderer_ptrs.data(), _debugger);
        if (c < _renderers.size()) {
            Utils::Error(std::string{"Failed to create Thallium renderer(s)"}, true);
        }
    }
}
