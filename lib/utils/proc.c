/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "proc.h"

#include "thallium/debug.h"

#include <stdio.h>
#include <stdlib.h>

const uint8_t th_KillProc() {
    fprintf(stderr, "HALT AND CATCH FIRE -- invoked by libthallium\n");

    fprintf(stderr, "Killed process\n");
    exit(EXIT_FAILURE);

    return THALLIUM_STATUS_OK;
}
