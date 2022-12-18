/*
 *   Copyright (c) 2022 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "utils/log.h"

#include <stdio.h>

void th_TestFunc() {
    th_Log("Hello world");
    th_Note("Hello world");
    th_Warn("Hello world");
    th_Error("Hello world");
    th_Fatal("Hello world");
}
