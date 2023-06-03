/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium.h"

#include <iostream>

TL_Context_t *CONTEXT;

TL_Debugger_t *DEBUGGER;

int main() {
    // ..:: Create a debugger ::..

    TL_DebuggerDescriptor_t debugger_descriptor = {};
    (int &) debugger_descriptor.severities = TL_DEBUG_SEVERITY_ALL_BIT;
    (int &) debugger_descriptor.sources = TL_DEBUG_SOURCE_THALLIUM_BIT | TL_DEBUG_SOURCE_VULKAN_BIT;

    DEBUGGER = TL_CreateDebugger(debugger_descriptor);
    if (!DEBUGGER) {
        throw std::runtime_error("Failed to create debugger");
    }


    // ..:: Create the Thallium context ::..

    CONTEXT = TL_CreateContext(DEBUGGER);
    if (!CONTEXT) {
        throw std::runtime_error("Failed to create Thallium context");
    }


    // ..:: Termination ::..

    TL_DestroyDebugger(DEBUGGER);

    TL_DestroyContext(CONTEXT);

    return 0;
}
