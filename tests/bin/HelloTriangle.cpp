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
    int ret = 0;

    test.AddRenderer<GraphicsAPI::Vulkan>();
    if (!test.CreateRenderers()) {
        Utils::Error("Failed to create renderer(s)");

        ret = 1;
        goto out;
    }

    swapchain1 = window1.CreateSwapchain(test.GetRenderers()[0]);

    Utils::Log("-- MAIN LOOP BEGIN --");

    for (; !window1.ShouldClose();) {
        Window::PollEvents();
    }

    Utils::Log("-- MAIN LOOP END --");

    TL_SwapchainDestroy(swapchain1);

out:
    test.Destroy();
    Window::TerminateAPI();

    return ret;
}
