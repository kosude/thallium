/*
 *   Copyright (c) 2022 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"

#include "state/state.h"

const void th_ConfigureDebugMessageFilter(const th_DebugSeverity_t severityBits) {
    thallium.debugSeverityFilter = severityBits;
}
