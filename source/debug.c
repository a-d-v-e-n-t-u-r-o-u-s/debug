/*!
 * \file
 * \brief Debug driver
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
#include "debug.h"
#include "usart.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <avr/pgmspace.h>

/*! \todo pass the buffer value from outside */
#define DEBUG_BUFFER_SIZE  64

static char debug_buffer[DEBUG_BUFFER_SIZE];

void DEBUG_output(const char *format, ...)
{
    va_list args;
    va_start( args, format);

    memset(debug_buffer, 0u, DEBUG_BUFFER_SIZE);

    uint8_t reserved_size = 2U;
    int written = vsnprintf(debug_buffer, DEBUG_BUFFER_SIZE - reserved_size - 1U, format, args);
    va_end(args);

    if(written > DEBUG_BUFFER_SIZE)
    {
        written = DEBUG_BUFFER_SIZE;
        debug_buffer[DEBUG_BUFFER_SIZE - 3U] = '~';
        debug_buffer[DEBUG_BUFFER_SIZE - 2U] = '\n';
    }

    for(uint8_t i = 0; i < written ; i++)
    {
        USART_transmit((uint8_t)debug_buffer[i]);
    }
}

void DEBUG_halt(const char *module, uint16_t line)
{
    /*! \todo change this naive implementation */
/*
 *    char tmp[5] = {0};
 *
 *    for(uint8_t i = 0; i < 4; i++, module++)
 *    {
 *        tmp[i] = pgm_read_byte(module);
 *    }
 */

    DEBUG_output("%s : Halt at %d line \n", module, line);
    while(true);
}

void DEBUG_init(const DEBUG_config_t *config)
{
    (void) config;
}
