/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 *
 *
 * Part of this file is (modified) from the log.c library, at https://github.com/rxi/log.c
 * That source's licence is shown below:
 *
 * Copyright (c) 2020 rxi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "log.h"

#include "utils/io/proc.h"
#include "types/core/debugger.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

// TODO: needs testing on Windows, as the colour output might not work properly. If it doesn't then libcio can be used instead of the current colours
// setup.

typedef struct {
    va_list ap;
    const char *fmt;
    struct tm *time;
    void *udata;
    int level;
} __log_Event;

typedef void (*__log_LogFn)(__log_Event *ev);
typedef void (*__log_LockFn)(bool lock, void *udata);

#define __MAX_CALLBACKS 32

typedef struct {
    __log_LogFn fn;
    void *udata;
    int level;
} __Callback;

static struct {
    void *udata;
    __log_LockFn lock;
    int level;
    bool quiet;
    __Callback callbacks[__MAX_CALLBACKS];
} __L;

enum {
    __LOG_VERBOSE,
    __LOG_NOTE,
    __LOG_WARN,
    __LOG_ERROR,
    __LOG_FATAL
};

static const char *level_strings[] = {
    "LOG  ", "NOTE ", "WARN ", "ERROR", "FATAL"
};

// note, spelling of colours fixed
static const char *level_colours[] = {
    "\x1b[94m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};

static void __stdout_callback(__log_Event *ev) {
    char buf[16];
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';

    fprintf(
        ev->udata, "%s %s%-5s\x1b[0m ",
        buf, level_colours[ev->level], level_strings[ev->level]);

    vfprintf(ev->udata, ev->fmt, ev->ap);
    fflush(ev->udata);
}

static void __lock(void) {
    if (__L.lock) {
        __L.lock(true, __L.udata);
    }
}

static void __unlock(void) {
    if (__L.lock) {
        __L.lock(false, __L.udata);
    }
}

static void __init_event(__log_Event *ev, void *udata) {
    if (!ev->time) {
        time_t t = time(NULL);
        ev->time = localtime(&t);
    }
    ev->udata = udata;
}

static void __log_log(int level, const char *fmt, ...) {
    __log_Event ev = {
        .fmt = fmt,
        .level = level,
    };

    __lock();

    if (!__L.quiet && level >= __L.level) {
        __init_event(&ev, stderr);
        va_start(ev.ap, fmt);
        __stdout_callback(&ev);
        va_end(ev.ap);
    }

    for (int i = 0; i < __MAX_CALLBACKS && __L.callbacks[i].fn; i++) {
        __Callback *cb = &__L.callbacks[i];
        if (level >= cb->level) {
            __init_event(&ev, cb->udata);
            va_start(ev.ap, fmt);
            cb->fn(&ev);
            va_end(ev.ap);
        }
    }

    __unlock();
}

// Maximum length of debug messages:
#define __MAX_DBGMSG_LEN 1024

void TL_ReportMessage(const TL_Debugger_t *const debugger, const TL_DebugSeverityFlags_t severity, const TL_DebugSourceFlags_t source,
    const char *const format, ...)
{
    // check against severity
    if (!debugger || !(debugger->severities & severity)) {
        return;
    }

    // check against source
    if (!debugger || !(debugger->sources & source)) {
        return;
    }

    switch (source) {
        case TL_DEBUG_SOURCE_THALLIUM_BIT:
            fprintf(stderr, "\x1b[90mTH   \x1b[0m");
            break;
        case TL_DEBUG_SOURCE_VULKAN_BIT:
            fprintf(stderr, "\x1b[90mTHVK \x1b[0m");
            break;
        default:
            break;
    }

    char msg[__MAX_DBGMSG_LEN];

    // format string msg with given format + variadic arguments
    va_list varargs;
    va_start(varargs, format);
    vsnprintf(msg, __MAX_DBGMSG_LEN, format, varargs);
    va_end(varargs);

    switch (severity) {
        case TL_DEBUG_SEVERITY_VERBOSE_BIT:
            __log_log(__LOG_VERBOSE, "%s\n", msg);
            break;
        case TL_DEBUG_SEVERITY_NOTIF_BIT:
            __log_log(__LOG_NOTE, "%s\n", msg);
            break;
        case TL_DEBUG_SEVERITY_WARNING_BIT:
            __log_log(__LOG_WARN, "%s\n", msg);
            break;
        case TL_DEBUG_SEVERITY_ERROR_BIT:
            __log_log(__LOG_ERROR,  "%s\n", msg);
            break;
        case TL_DEBUG_SEVERITY_FATAL_BIT:
            __log_log(__LOG_FATAL, "%s\n", msg);
            TL_KillProc();
            return;
        default:
            break;
    }

    return;
}
