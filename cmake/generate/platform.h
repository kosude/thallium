/*
 *   Copyright (c) 2022 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#pragma once
#ifndef _CMAKE_PLATFORM_H
#define _CMAKE_PLATFORM_H

#cmakedefine WIN32
#cmakedefine APPLE
#cmakedefine UNIX

#if defined(_MSC_VER) // if using MSVC:
#    if _DEBUG
#        define BUILD_DEBUG
#    endif
#elif defined(__apple_build_version__) // if using whatever rubbish Apple uses:
#    if DEBUG
#        define BUILD_DEBUG
#    endif
#else // if using another compiler, like GCC, Clang, etc.
#    cmakedefine BUILD_DEBUG
#endif

#endif // _CMAKE_PLATFORM_H
