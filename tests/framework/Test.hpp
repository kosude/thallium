/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#ifndef __tests__Test_hpp__
#define __tests__Test_hpp__

#include "thallium.h"

#include "Enums.hpp"

#include <vector>

namespace TLTests::Framework {
    class Test {
    private:
        TL_Debugger_t *_debugger;

        TL_Context_t *_context;

        std::vector<TL_RendererDescriptor_t> _renderer_descriptors;
        std::vector<TL_Renderer_t *> _renderers;

        Test();
        inline ~Test() {}

        static TL_RendererFeatures_t _GetRendererRequirements();

    public:
        Test(const Test &) = delete;
        void operator=(const Test &) = delete;

        inline static Test &GetInstance() {
            // instantiate on first use
            static Test instance;
            return instance;
        };

        inline const std::vector<TL_Renderer_t *> &GetRenderers() const { return _renderers; }

        void Destroy();

        template<GraphicsAPI A>
        void AddRenderer() = delete;

        void CreateRenderers();
    };

    template<>
    void Test::AddRenderer<GraphicsAPI::Vulkan>();
}

#endif
