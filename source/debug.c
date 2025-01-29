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
#define DEBUG_BUFFER_SIZE               (64U)
#define RESERVED_CHARS_SIZE             (2U)

#define TOO_LONG_MSG_MARKER_IDX         (DEBUG_BUFFER_SIZE - 3U)
#define TOO_LONG_MSG_MARKER_LF_IDX      (DEBUG_BUFFER_SIZE - 2U)
#define TOO_LONG_MSG_MARKER_NULL_IDX    (DEBUG_BUFFER_SIZE - 1U)
#define TOO_LONG_MSG_SIZE               (DEBUG_BUFFER_SIZE)

#define ENCODING_ERROR_MSG_MARKER_IDX   (0U)
#define ENCODING_ERROR_MSG_LF_IDX       (1U)
#define ENCODING_ERROR_MSG_NULL_IDX     (2U)

#define ENCODING_ERROR_MSG_SIZE         (3)

static char debug_buffer[DEBUG_BUFFER_SIZE];

void DEBUG_output(const char *format, ...)
{
    va_list args;
    va_start( args, format);

    (void) memset(debug_buffer, 0u, DEBUG_BUFFER_SIZE);

    int16_t written = vsnprintf(debug_buffer,
            DEBUG_BUFFER_SIZE - RESERVED_CHARS_SIZE - 1U, format, args);
    va_end(args);

    if(written < 0)
    {
        written = ENCODING_ERROR_MSG_SIZE;
        debug_buffer[ENCODING_ERROR_MSG_MARKER_IDX] = '!';
        debug_buffer[ENCODING_ERROR_MSG_LF_IDX] = '\n';
        debug_buffer[ENCODING_ERROR_MSG_NULL_IDX] = 0;
    }
    if(((uint16_t) written) > DEBUG_BUFFER_SIZE)
    {
        written = TOO_LONG_MSG_SIZE;
        debug_buffer[TOO_LONG_MSG_MARKER_IDX] = '~';
        debug_buffer[TOO_LONG_MSG_MARKER_LF_IDX] = '\n';
        debug_buffer[TOO_LONG_MSG_MARKER_NULL_IDX] = 0;
    }
    else
    {
        /* nothing to do here*/
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
    bool is_too_much = false;

    (void) memset(debug_buffer, 0u, DEBUG_BUFFER_SIZE);

    while((len != 0U) && (!is_too_much))
    {
        uint8_t left_space = DEBUG_BUFFER_SIZE - RESERVED_CHARS_SIZE - offset - 1U;
        int16_t written = snprintf(&debug_buffer[offset], left_space,
                is_hex ? "%d:0x%02x\n" : "%d:%d\n" ,cnt, *buff);

        buff++;
        offset += written;

        if(offset > DEBUG_BUFFER_SIZE)
        {
            offset = DEBUG_BUFFER_SIZE;
            is_too_much = true;
        }

        len--;
        cnt++;
    }

    if(offset == DEBUG_BUFFER_SIZE)
    {
        debug_buffer[TOO_LONG_MSG_MARKER_IDX] = '~';
        debug_buffer[TOO_LONG_MSG_MARKER_LF_IDX] = '\n';
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
