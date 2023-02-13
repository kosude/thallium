/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"

// #include "dyn_version.h"

#include <stdio.h>


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

const void th_VersionToString(const th_Version_t version, char *out_string) {
    snprintf(out_string, 256, "%d.%d.%d", version.major, version.minor, version.patch);
}

const char *const th_GetThalliumVersion() {
    return THALLIUM_VERSION;
}
