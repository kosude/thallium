/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"
#include "thallium_vulkan.h"

#include <iostream>

int main() {
    thvk_TestVkFunc();

    th_ConfigureDebugMessageFilter(THALLIUM_DEBUG_SEVERITY_ALL_BIT);

    const th_Renderer_t vulkan = th_CreateRenderer("vulkan");

    std::cout << vulkan.apiName << std::endl;
    std::cout << std::to_string(vulkan.apiId) << std::endl;

    return 0;
}
