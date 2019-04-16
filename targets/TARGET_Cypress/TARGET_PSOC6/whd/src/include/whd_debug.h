/*
 * $ Copyright Cypress Semiconductor $
 */

#ifndef INCLUDED_WHD_DEBUG_H
#define INCLUDED_WHD_DEBUG_H

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include "whd.h"

#ifdef PLATFORM_TRACE
#include "platform_trace.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*                      Macros
******************************************************/
#define WPRINT_ENABLE_WHD_ERROR
#define WHD_ENABLE_STATS
/*#define WWD_LOGGING_BUFFER_ENABLE*/

#ifdef DEBUG
#include "platform_assert.h"
#define WHD_BREAK_IF_DEBUG( ) WHD_TRIGGER_BREAKPOINT()
#else
#define WHD_BREAK_IF_DEBUG( )
#endif

#ifdef WPRINT_MACRO
#undef WPRINT_MACRO
#endif

#ifdef whd_assert
#undef whd_assert
#endif

#ifdef whd_minor_assert
#undef whd_minor_assert
#endif

#ifdef WPRINT_ENABLE_ERROR
#define WPRINT_ERROR(args)                      do { WPRINT_MACRO(args); WICED_ASSERTION_FAIL_ACTION(); } while (0)
#define whd_assert(error_string, assertion) do { if (!(assertion) ){ WICED_ASSERTION_FAIL_ACTION(); } } while (0)
#define whd_minor_assert(error_string, \
                         assertion)   do { if (!(assertion) ) WPRINT_MACRO( (error_string) ); } while (0)
#else
#define WHD_BREAK_IF_DEBUG( )
#define whd_assert(error_string, assertion)         do { if (!(assertion) ) WPRINT_MACRO( (error_string) ); } while (0)
#define whd_minor_assert(error_string, assertion)   do { (void)(assertion); } while (0)
#endif

/******************************************************
*             Print declarations
******************************************************/
#if defined( WWD_LOGGING_BUFFER_ENABLE )
#define WPRINT_MACRO(args) do { whd_buffer_printf args; } while(0==1)
#else
#define WPRINT_MACRO(args) do { printf args;} while (0 == 1)
#endif


/* WICED printing macros for Wiced Wi-Fi Driver*/
#ifdef WPRINT_ENABLE_WHD_INFO
#define WPRINT_WHD_INFO(args) WPRINT_MACRO(args)
#else
#define WPRINT_WHD_INFO(args)
#endif

#ifdef WPRINT_ENABLE_WHD_DEBUG
#define WPRINT_WHD_DEBUG(args) WPRINT_MACRO(args)
#else
#define WPRINT_WHD_DEBUG(args)
#endif

#ifdef WPRINT_ENABLE_WHD_ERROR
#define WPRINT_WHD_ERROR(args) { WPRINT_MACRO(args); WHD_BREAK_IF_DEBUG(); }
#else
#define WPRINT_WHD_ERROR(args) { WHD_BREAK_IF_DEBUG(); }
#endif

#ifdef WPRINT_ENABLE_WHD_DATA_LOG
#define WPRINT_WHD_DATA_LOG(args) WPRINT_MACRO(args)
#else
#define WPRINT_WHD_DATA_LOG(args)
#endif

#define WHD_STATS_INCREMENT_VARIABLE(whd_driver, var) \
    do { whd_driver->whd_stats.var++; } while (0)

#define WHD_STATS_CONDITIONAL_INCREMENT_VARIABLE(whd_driver, condition, var) \
    do { if (condition){ whd_driver->whd_stats.var++; }} while (0)

#if (defined(__GNUC__) && (__GNUC__ >= 6) )
#define __FUNCTION__ __func__
#endif


void whd_init_stats(whd_driver_t whd_driver);
void whd_print_logbuffer(void);


#ifdef WWD_LOGGING_BUFFER_ENABLE
#define LOGGING_BUFFER_SIZE (4*1024)
int whd_buffer_printf (const char *format, ...);

typedef struct
{
    uint32_t buffer_write;
    uint32_t buffer_read;
    char buffer[LOGGING_BUFFER_SIZE + 1];
    whd_bool_t roll_over;
    whd_bool_t over_write;
} whd_logging_t;
#else
#define whd_print_logbuffer()
#endif /* WWD_LOGGING_BUFFER_ENABLE */

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* ifndef INCLUDED_WHD_DEBUG_H */

