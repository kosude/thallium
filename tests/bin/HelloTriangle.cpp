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
TL_Swapchain_t *swapchain1;

int main() {
    test.AddRenderer<GraphicsAPI::Vulkan>();
    test.CreateRenderers();

    swapchain1 = window1.CreateSwapchain(test.GetRenderers()[0]);

    for (; !window1.ShouldClose();) {
        Window::PollEvents();
    }

    TL_SwapchainDestroy(swapchain1);

    test.Destroy();
    Window::TerminateAPI();

    return 0;
}
