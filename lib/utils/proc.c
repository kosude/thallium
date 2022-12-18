/*
 *   Copyright (c) 2022 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "proc.h"

#include <stdio.h>
#include <stdlib.h>

const void th_KillProc() {
    fprintf(stderr, "HALT AND CATCH FIRE -- invoked by libthallium\n");

    fprintf(stderr, "Killed process\n");
    exit(EXIT_FAILURE);
}
