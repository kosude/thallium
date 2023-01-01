/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"

#include "state/state.h"

const th_Status_t th_ConfigureDebugMessageFilter(const th_DebugSeverity_t severityBits) {
    thallium.debugSeverityFilter = severityBits;

    return THALLIUM_STATUS_OK;
}
