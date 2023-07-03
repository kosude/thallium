/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#ifndef __tests__Utils_hpp__
#define __tests__Utils_hpp__

#include <iosfwd>

namespace TLTests::Framework {
    void Log(
        const std::string &msg,
        const bool &timestamp = true
    );
    void Error(
        const std::string &msg,
        const bool &timestamp = true,
        const bool &exit_proc = false
    );
}

#endif
