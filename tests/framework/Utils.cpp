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

namespace TLTests::Framework::Utils {
    void Log(const std::string &msg, const bool &timestamp) {
        // verbose messages not shown in release
#       ifdef _DEBUG
            std::cerr << "TEST : ";

            if (timestamp) {
                std::cerr << __GetCurrentDateTime();
            } else {
                std::cerr << "        ";
            }

            std::cerr << "      " << msg << std::endl;
#       endif
    }

    void Error(const std::string &msg, const bool &timestamp, const bool &exit_proc) {
        ciocolstateset(CIOCOL_RED, 0xff, stderr);

        std::cerr << "ERROR: ";

        if (timestamp) {
            std::cerr << __GetCurrentDateTime();
        } else {
            std::cerr << "        ";
        }

        std::cerr << "      " << msg << std::endl;

        ciocolstatedef(stderr);

        if (exit_proc) {
            throw new std::runtime_error(msg);
        }
    }

    void DebugCallback(char *msg, TL_DebugSeverityFlags_t sev, TL_DebugSourceFlags_t src, void *ptr) {
        std::cerr << ciocol(CIOCOL_GREY, 0xff).code << "tl";
        switch (src) {
            case TL_DEBUG_SOURCE_VULKAN_BIT:
                std::cerr << ciocol(CIOCOL_RED, 0xff).code << "VK ";
                break;
            default:
                std::cerr << "   ";
                break;
        }
        std::cerr << ciocol(CIOCOL_GREY, 0xff).code << ": ";

        std::cerr << __GetCurrentDateTime() << " ";

        switch (sev) {
            case TL_DEBUG_SEVERITY_VERBOSE_BIT:
                std::cerr << ciocol(CIOCOL_BLUE, 0xff).code << "LOG   ";
                break;
            case TL_DEBUG_SEVERITY_NOTIF_BIT:
                std::cerr << ciocol(CIOCOL_GREEN, 0xff).code << "NOTE  ";
                break;
            case TL_DEBUG_SEVERITY_WARNING_BIT:
                std::cerr << ciocol(CIOCOL_YELLOW, 0xff).code << "WARN  ";
                break;
            case TL_DEBUG_SEVERITY_ERROR_BIT:
                std::cerr << ciocol(CIOCOL_RED, 0xff).code << "ERROR ";
                break;
            case TL_DEBUG_SEVERITY_FATAL_BIT:
                std::cerr << ciocol(CIOCOL_RED, 0xff).code << "FATAL ";
                break;
        }

        std::cerr << ciocoldef().code << msg << std::endl;
    }
}
