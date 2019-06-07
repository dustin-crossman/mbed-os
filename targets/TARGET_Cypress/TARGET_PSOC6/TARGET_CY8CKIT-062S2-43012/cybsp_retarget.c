/***************************************************************************//**
* \file cybsp_retarget.c
*
* Description:
* Provides APIs for retargetting stdio to UART hardware contained on the Cypress
* CY8CKIT-062S2-43012 pioneer kit.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include <stdlib.h>
#include "cycfg.h"
#include "cybsp_cy8ckit_062s2_43012.h"

#if defined(__cplusplus)
extern "C" {
#endif


uint8_t cybsp_stdio_getchar(void)
{
	uint8 c = 0;
	cyhal_uart_getc(&cybsp_debug_uart, &c);
	return c;
}

void cybsp_stdio_putchar(char c)
{
	cyhal_uart_putc(&cybsp_debug_uart, c);
}

#if defined(__ARMCC_VERSION) /* ARM-MDK */
    /***************************************************************************
    * Function Name: fputc
    ***************************************************************************/
    __attribute__((weak)) int fputc(int ch, FILE *f)
    {
        (void)f;
    #if (STDOUT_CR_LF != 0)
        if ((char)ch == '\n')
        {
            cybsp_stdio_putchar('\r');
        }
    #endif /* STDOUT_CR_LF != 0 */
        cybsp_stdio_putchar(ch);
        return (ch);
    }
#elif defined (__ICCARM__) /* IAR */
    #include <yfuns.h>

    /***************************************************************************
    * Function Name: __write
    ***************************************************************************/
    __weak size_t __write(int handle, const unsigned char * buffer, size_t size)
    {
        size_t nChars = 0;
        /* This template only writes to "standard out", for all other file
        * handles it returns failure. */
        if (handle != _LLIO_STDOUT)
        {
            return (_LLIO_ERROR);
        }
        if (buffer != NULL)
        {
            for (/* Empty */; nChars < size; ++nChars)
            {
            #if (STDOUT_CR_LF != 0)
                if(*buffer == '\n')
                {
                    cybsp_stdio_putchar('\r');
                }
            #endif /* STDOUT_CR_LF != 0 */
                cybsp_stdio_putchar(*buffer);
                ++buffer;
            }
        }
        return (nChars);
    }
#else /* (__GNUC__)  GCC */
    /* Add an explicit reference to the floating point printf library to allow
    the usage of floating point conversion specifier. */
    __asm (".global _printf_float");
    /***************************************************************************
    * Function Name: _write
    ***************************************************************************/
    __attribute__((weak)) int _write (int fd, const char *ptr, int len)
    {
        int nChars = 0;
        (void)fd;
        if (ptr != NULL)
        {
            for (/* Empty */; nChars < len; ++nChars)
            {
            #if (STDOUT_CR_LF != 0)
                if(*ptr == '\n')
                {
                    cybsp_stdio_putchar('\r');
                }
            #endif /* STDOUT_CR_LF != 0 */
                cybsp_stdio_putchar((uint32_t)*ptr);
                ++ptr;
            }
        }
        return (nChars);
    }
#endif


#if defined(__ARMCC_VERSION) /* ARM-MDK */
    /***************************************************************************
    * Function Name: fgetc
    ***************************************************************************/
    __attribute__((weak)) int fgetc(FILE *f)
    {
        (void)f;
        return (cybsp_stdio_getchar());
    }
#elif defined (__ICCARM__) /* IAR */
    __weak size_t __read(int handle, unsigned char * buffer, size_t size)
    {
        /* This template only reads from "standard in", for all other file
        handles it returns failure. */
        if ((handle != _LLIO_STDIN) || (buffer == NULL))
        {
            return (_LLIO_ERROR);
        }
        else
        {
            *buffer = cybsp_stdio_getchar();
            return (1);
        }
    }
#else /* (__GNUC__)  GCC */
    /* Add an explicit reference to the floating point scanf library to allow
    the usage of floating point conversion specifier. */
    __asm (".global _scanf_float");
    __attribute__((weak)) int _read (int fd, char *ptr, int len)
    {
        int nChars = 0;
        (void)fd;
        if (ptr != NULL)
        {
            for(/* Empty */;nChars < len;++ptr)
            {
                *ptr = (char)cybsp_stdio_getchar();
                ++nChars;
                if((*ptr == '\n') || (*ptr == '\r'))
                {
                    break;
                }
            }
        }
        return (nChars);
    }
#endif


#if defined(__cplusplus)
}
#endif
