/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "io_colour.h"

#include "cmake_platform.h"

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#else
#   include <stdio.h>
#endif

#ifdef UNIX
    /**
     * @brief Convert a numerical IO colour output (@ref th_IOColour_t) to a UNIX text colour escape string
     *
     * This function relates to **foreground colours**.
     *
     * @param fg the colour enumerator
     * @return escape string
     */
    static const char *_GetFGColourCode(const th_IOColour_t fg) {
        // Courtesy of ProjectPhysX on StackOverflow
        switch(fg) {
            case  0: return "30";
            case  1: return "34";
            case  2: return "32";
            case  3: return "36";
            case  4: return "31";
            case  5: return "35";
            case  6: return "33";
            case  7: return "37";
            case  8: return "90";
            case  9: return "94";
            case 10: return "92";
            case 11: return "96";
            case 12: return "91";
            case 13: return "95";
            case 14: return "93";
            case 15: return "97";
            default: return "37"; // 37 (light grey) is default
        };
    }

    /**
     * @brief Convert a numerical IO colour output (@ref th_IOColour_t) to a UNIX text colour escape string
     *
     * This function relates to **background colours**.
     *
     * @param bg the colour enumerator
     * @return escape string
     */
    static const char *_GetBGColourCode(const th_IOColour_t bg) {
        // Courtesy of ProjectPhysX on StackOverflow
        switch(bg) {
            case  0: return  "40";
            case  1: return  "44";
            case  2: return  "42";
            case  3: return  "46";
            case  4: return  "41";
            case  5: return  "45";
            case  6: return  "43";
            case  7: return  "47";
            case  8: return "100";
            case  9: return "104";
            case 10: return "102";
            case 11: return "106";
            case 12: return "101";
            case 13: return "105";
            case 14: return "103";
            case 15: return "107";
            default: return  "40"; // 40 (black) is default
        };
    }
#endif

const void th_SetIOColour(const th_IOColour_t fg, const th_IOColour_t bg, FILE *stream) {
    if (fg == 0xFF && bg == 0xFF) {
        // no values given
        return;
    }

#   ifdef WIN32
        // NOTE: this is yet to be tested

        unsigned int colourAttribute = 0;

        if (fg != 0xFF && bg != 0xFF) {
            // both values are given
            colourAttribute = bg << 4 | fg;
        } else if (fg != 0xFF && bg == 0xFF) {
            // only fg is given
            colourAttribute = fg;
        } else {
            // only bg is given
            colourAttribute = bg << 4;
        }

        static const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handle, bg << 4 | fg);
#   elif defined(UNIX)
        fprintf(stream,
            "\033[%s%s%sm",
            (fg != 0xFF) ? _GetFGColourCode(fg) : "0",
            (bg != 0xFF) ? ";" : "",
            (bg != 0xFF) ? _GetBGColourCode(bg) : ""
        );
#   else
        // TODO: macOS support
        printf("/?/");
#   endif
}

const void th_DefaultIOColour(FILE *stream) {
#   if defined(WIN32)
        // NOTE: this is yet to be tested
        static const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handle, 7);
#   elif defined(UNIX)
        fprintf(stream, "\033[0m");
#   else
        // TODO: macOS support
        printf("/?/");
#   endif
}
