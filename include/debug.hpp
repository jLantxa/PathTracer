/*
 * This source file is part of PathTracer
 *
 * Copyright 2018, 2019  Javier Lancha Vázquez
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef _PATH_TRACER_INCLUDE_DEBUG_HPP_
#define _PATH_TRACER_INCLUDE_DEBUG_HPP_

#include <cstdio>

namespace Debug
{
    enum DebugLevel : int {
        NONE    = -1,
        ERROR    = 0,
        WARNING = 1,
        INFO   = 2,
        DEBUG   = 3,
        VERBOSE = 4,
    };

    static const char* level_tags[] {
        [DebugLevel::ERROR]     = "E",
        [DebugLevel::WARNING]   = "W",
        [DebugLevel::INFO]     = "I",
        [DebugLevel::DEBUG]     = "D",
        [DebugLevel::VERBOSE]   = "V"
    };

    const static int DEFAULT_LEVEL = INFO;

#if defined(NDEBUG) && NDEBUG !=0
    static int defined_level = NONE;
#else
    #ifdef DEBUG_LEVEL
        static int defined_level = DEBUG_LEVEL;
    #else
        static int defined_level = DEFAULT_LEVEL;
    #endif

#endif

    template <typename... Args>
    static void log(const int level, const char* tag, const char* str) {
        if (level <= defined_level) {
            printf("[%s] %s: ", level_tags[level], tag);
            printf("%s", str);
            printf("\n");
        }
    }

    template <typename... Args>
    static void log(const int level, const char* tag, const char* fmt, Args... args) {
        if (level <= defined_level) {
            printf("[%s] %s: ", level_tags[level], tag);
            printf(fmt, args...);
            printf("\n");
        }
    }

    // Visible debug functions
    class Log {
    public:
        template <typename... Args>
        static void e(const char* tag, Args... args) {
            log(ERROR, tag, args...);
        }

        template <typename... Args>
        static void w(const char* tag, Args... args) {
            log(WARNING, tag, args...);
        }

        template <typename... Args>
        static void i(const char* tag, Args... args) {
            log(INFO, tag, args...);
        }

        template <typename... Args>
        static void d(const char* tag, Args... args) {
            log(DEBUG, tag, args...);
        }

        template <typename... Arg>
        static void v(const char* tag, Arg... args) {
            log(VERBOSE, tag, args...);
        }

    };
}

#endif // _MODEM_INCLUDE_DEBUG_HPP_
