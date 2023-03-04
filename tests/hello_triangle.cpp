/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "test_base/test_base.hpp"

using namespace thallium_tests;

int main() {
    TestBase<TestApi::Vulkan> vulkan_test_base("Hello Triangle");

    return 0;
}
