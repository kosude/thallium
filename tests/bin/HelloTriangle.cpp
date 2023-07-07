/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "framework/Test.hpp"
#include "framework/object/Window.hpp"
#include "framework/Utils.hpp"

#include <iostream>

using namespace TLTests::Framework;

Test &test = Test::GetInstance();

Window window1(640, 480, "Hello Triangle");

int main() {
    test.AddRenderer<GraphicsAPI::Vulkan>();
    test.CreateRenderers();

    // swapchains - temp
    TL_SwapchainDescriptor_t scdescr = {};
    TL_Swapchain_t *swapchain = TL_SwapchainCreate(test.GetRenderers()[0], scdescr);

    TL_SwapchainDestroy(swapchain);

    test.Destroy();

    Window::TerminateAPI();

    return 0;
}
