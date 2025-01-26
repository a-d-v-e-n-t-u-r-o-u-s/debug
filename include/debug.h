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

/*!
 *
 * \addtogroup debug
 * \ingroup modules
 * \brief DEBUG module
 */

/*@{*/

/* \todo (DB) Should be moved to the makefile */
/*!
 * \brief Enables globally debug messages
 */
#define DEBUG_GLOBALLY_ENABLED 0

/* \todo (DB) Should be moved to the makefile */
/*!
 * \brief Enables globally asserts
 */
#define ASSERTS_GLOBALLY_ENABLED 0

/*!
 *
 * \addtogroup debug_lvls
 * \ingroup debug
 * \brief DEBUG levels
 */
/*@{*/
#define DL_ERROR        0
#define DL_WARNING      1
#define DL_INFO         2
#define DL_DEBUG        3
#define DL_VERBOSE      4
/*@}*/

/*!
 *
 * \addtogroup debug_lvl_per_module
 * \ingroup debug
 * \brief DEBUG level settings per module
 */
/*@{*/
#define DEBUG_1WIRE_MGR_LEVEL   DL_DEBUG
#define DEBUG_APP_LEVEL         DL_INFO
#define DEBUG_MAIN_LEVEL        DL_INFO
#define DEBUG_STAT_LEVEL        DL_INFO
#define DEBUG_SYS_LEVEL         DL_WARNING
#define DEBUG_GPIO_LEVEL        DL_WARNING
#define DEBUG_DS1302_LEVEL      DL_WARNING
#define DEBUG_SSDM_LEVEL        DL_WARNING
#define DEBUG_INPM_LEVEL        DL_WARNING
/*@}*/

/* \todo (DB) Should be moved to the makefile */
/*!
 *
 * \addtogroup debug_switch_per_module
 * \ingroup debug
 * \brief DEBUG enable/disable settings per module
 */
/*@{*/
#define DEBUG_1WIRE_MGR_ENABLED 0
#define DEBUG_APP_ENABLED       0
#define DEBUG_MAIN_ENABLED      0
#define DEBUG_STAT_ENABLED      0
#define DEBUG_SYS_ENABLED       0
#define DEBUG_GPIO_ENABLED      0
#define DEBUG_DS1302_ENABLED    0
#define DEBUG_SSDM_ENABLED      0
#define DEBUG_INPM_ENABLED      0
/*@}*/

#if DEBUG_GLOBALLY_ENABLED
#if ASSERTS_GLOBALLY_ENABLED

/*!
 * \brief Assert macro
 *
 * \param condition condition to fail in case assert should be triggered
 */
#define ASSERT(condition)                                               \
    do                                                                  \
    {                                                                   \
        condition ? condition :DEBUG_halt(DEBUG_APP_ID, __LINE__) ;     \
    } while(0)
#else
#define ASSERT(condition)
#endif
#if DEBUG_ENABLED

/*!
 * \brief Debug message helper macro
 *
 * \param level debug message level
 * \param str debug message
 */
#define DEBUG(level, str, ...)              \
    if(level <= DEBUG_LEVEL)                \
    {                                       \
        DEBUG_output(str, __VA_ARGS__);     \
    }

/*!
 * \brief Dump memory in hex
 *
 * \param level debug message level
 * \param buffer memory to be dumped
 * \param size size of memory to be dumped
 */
#define DEBUG_DUMP_HEX(level, buffer, size) \
    if(level <= DEBUG_LEVEL)                \
    {                                       \
        DEBUG_dump(buffer, size, true);     \
    }

/*!
 * \brief Dump memory in dec
 *
 * \param level debug message level
 * \param buffer memory to be dumped
 * \param size size of memory to be dumped
 */
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

/* \todo (DB) This structure seems to be unused currently */
/*!
 * \brief Debug module configuration structure
 */
typedef struct
{
    uint32_t baudrate; /*!< Baudrate */
} DEBUG_config_t;

/*!
 * \brief Outputs debug message
 *
 * \param format message to be sent
 */
void DEBUG_output(const char *format, ...);

/*!
 * \brief Outputs memory dump in hex or dec format
 *
 * \param buffer pointer to the memory to be dumped
 * \param size size of memory to be dumped
 * \param is_hex hex/dec format setting, true is hex
 */
void DEBUG_dump(const uint8_t *buffer, uint8_t size, bool is_hex);

/*!
 * \brief Halts system and outputs halt position
 *
 * \param module module identifier as string
 * \param line line number at which halt occured
 */
__attribute__((noreturn)) void DEBUG_halt(const char *module, uint16_t line);

/*!
 * \brief Initializes debug module
 *
 * \param config configuration structure of the module
 */
void DEBUG_init(const DEBUG_config_t *config);
/*@}*/
#endif
