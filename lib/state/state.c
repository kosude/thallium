/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "state.h"

#include "thallium.h"

th_State_t thallium = {
    .debugSeverityFilter = THALLIUM_DEBUG_SEVERITY_WARNING_BIT | THALLIUM_DEBUG_SEVERITY_ERROR_BIT | THALLIUM_DEBUG_SEVERITY_FATAL_BIT
};
