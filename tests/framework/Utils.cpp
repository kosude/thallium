/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "Utils.hpp"

#include "cutils/cio/cio.h"
#include "cutils/cio/cioenum.h"

#include <iostream>
#include <ctime>

static std::string __GetCurrentDateTime(const bool &with_date = false) {
    std::time_t t = std::time(nullptr);
    std::tm *tm = std::localtime(&t);

    char buf[64];

    if (tm) {
        if (with_date) {
            std::strftime(buf, 64, "%Y-%m-%d %X", tm);
        } else {
            std::strftime(buf, 64, "%X", tm);
        }

        return std::string { buf };
    }

    throw std::runtime_error("Date/time get failed");
}

namespace TLTests::Framework {
    void Log(const std::string &msg, const bool &timestamp) {
        // verbose messages not shown in release
#       ifdef _DEBUG
            if (timestamp) {
                std::cout << "[" << __GetCurrentDateTime() << "] ";
            }
            std::cout << msg << std::endl;
#       endif
    }

    void Error(const std::string &msg, const bool &timestamp, const bool &exit_proc) {
        ciocolstateset(CIOCOL_RED, 0xff, stdout);

        if (timestamp) {
            std::cout << "[" << __GetCurrentDateTime() << "] ";
        }
        std::cout << msg << std::endl;

        ciocolstatedef(stdout);

        if (exit_proc) {
            throw new std::runtime_error(msg);
        }
    }
}
