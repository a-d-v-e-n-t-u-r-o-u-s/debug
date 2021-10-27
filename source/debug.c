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
/*! \todo (DB) currently added in order to silence compiler warning */
#define DEBUG_ENABLED   0
#define DEBUG_LEVEL     DL_WARNING

#include "debug.h"
#include "usart.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <avr/pgmspace.h>

/*! \todo (DB) pass the buffer value from outside */
#define DEBUG_BUFFER_SIZE       (64U)
#define RESERVED_CHARS_SIZE     (2U)

static char debug_buffer[DEBUG_BUFFER_SIZE];

void DEBUG_output(const char *format, ...)
{
    va_list args;
    va_start( args, format);

    memset(debug_buffer, 0u, DEBUG_BUFFER_SIZE);

    int written = vsnprintf(debug_buffer,
            DEBUG_BUFFER_SIZE - RESERVED_CHARS_SIZE - 1U, format, args);
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

/*! \todo (DB) enhance this quick implementation later */
void DEBUG_dump(const uint8_t *buffer, uint8_t size, bool is_hex)
{
    uint8_t offset = 0u;
    uint8_t cnt = 0u;
    const uint8_t *buff = buffer;
    uint8_t len = size;
    memset(debug_buffer, 0u, DEBUG_BUFFER_SIZE);

    while(len != 0U)
    {
        uint8_t left_space = DEBUG_BUFFER_SIZE - RESERVED_CHARS_SIZE - offset - 1U;
        uint8_t written = snprintf(&debug_buffer[offset], left_space,
                is_hex ? "%d:0x%02x\n" : "%d:%d\n" ,cnt, *buff);

        buff++;
        offset += written;

        if(offset > DEBUG_BUFFER_SIZE)
        {
            offset = DEBUG_BUFFER_SIZE;
            break;
        }

        len--;
        cnt++;
    }

    if(offset == DEBUG_BUFFER_SIZE)
    {
        debug_buffer[DEBUG_BUFFER_SIZE - 3U] = '~';
        debug_buffer[DEBUG_BUFFER_SIZE - 2U] = '\n';
    }
    else
    {
        offset++;
        debug_buffer[offset] = '\n';
        offset++;
    }

    for(uint8_t i = 0; i < offset ; i++)
    {
        USART_transmit((uint8_t)debug_buffer[i]);
    }
}

void DEBUG_halt(const char *module, uint16_t line)
{
    /*! \todo (DB) change this naive implementation */
    DEBUG_output("%s : Halt at %d line \n", module, line);

    while(true)
    {
        /* the edge of the world */
    }
}

void DEBUG_init(const DEBUG_config_t *config)
{
    (void) config;
}
