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

    test.Destroy();

    Window::TerminateAPI();

    return 0;
}
