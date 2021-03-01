/*!
 * \file
 * \brief Debug
 * \author Dawid Babula
 * \email dbabula@adventurous.pl
 *
 * \par Copyright (C) Dawid Babula, 2018
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include <stdbool.h>

#define DEBUG_GLOBALLY_ENABLED 1

#define DL_ERROR        0
#define DL_WARNING      1
#define DL_INFO         2
#define DL_DEBUG        3
#define DL_VERBOSE      4

#define DEBUG_1WIRE_MGR_LEVEL   DL_INFO
#define DEBUG_APP_LEVEL         DL_INFO
#define DEBUG_MAIN_LEVEL        DL_INFO
#define DEBUG_STAT_LEVEL        DL_INFO

#define DEBUG_1WIRE_MGR_ENABLED 1
#define DEBUG_APP_ENABLED       1
#define DEBUG_MAIN_ENABLED      1
#define DEBUG_STAT_ENABLED      1

#if DEBUG_GLOBALLY_ENABLED
#define ASSERT(condition)                                               \
    do                                                                  \
    {                                                                   \
        condition ? condition :DEBUG_halt(DEBUG_APP_ID, __LINE__) ;     \
    } while(0)
#if DEBUG_ENABLED
#define DEBUG(level, str, ...)              \
    if(level <= DEBUG_LEVEL)                \
    {                                       \
        DEBUG_output(str, __VA_ARGS__);     \
    }
#define DEBUG_DUMP_HEX(level, buffer, size) \
    if(level <= DEBUG_LEVEL)                \
    {                                       \
        DEBUG_dump(buffer, size, true);     \
    }
#define DEBUG_DUMP_DEC(level, buffer, size) \
    if(level <= DEBUG_LEVEL)                \
    {                                       \
        DEBUG_dump(buffer, size, false);    \
    }
#else
#define DEBUG(level, str, ...)
#define DEBUG_DUMP_HEX(level, buffer, size)
#define DEBUG_DUMP_DEC(level, buffer, size)
#endif
#else
#define ASSERT(condition)
#define DEBUG(str, ...)
#define DEBUG_DUMP_HEX(level, buffer, size)
#define DEBUG_DUMP_DEC(level, buffer, size)
#endif

typedef struct
{
    uint32_t baudrate;
} DEBUG_config_t;

void DEBUG_output(const char *format, ...);
void DEBUG_dump(const uint8_t *buffer, uint8_t size, bool is_hex);
void DEBUG_halt(const char *module, uint16_t line) __attribute__((noreturn));
void DEBUG_init(const DEBUG_config_t *config);
#endif
