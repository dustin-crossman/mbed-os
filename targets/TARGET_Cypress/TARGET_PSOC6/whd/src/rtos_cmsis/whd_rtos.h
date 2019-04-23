/*
 * $ Copyright Cypress Semiconductor Apache2 $
 */

#include "whd_types.h"
#include "whd_debug.h"
#include "whd_rtos_interface.h"
#include "rtx_os.h"
#include "os_tick.h"
#include "cmsis_os2.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
*                      Macros
******************************************************/


/******************************************************
*                    Constants
******************************************************/
#ifndef SYSTICK_FREQUENCY
#define SYSTICK_FREQUENCY  (1000)
#endif

/******************************************************
*                   Enumerations
******************************************************/

/******************************************************
*                 Type Definitions
******************************************************/


typedef osSemaphoreId_t     whd_semaphore_type_t;             /** CMSIS definition of a semaphore */
typedef osMutexId_t         whd_mutex_type_t;                 /** CMSIS definition of a mutex */
typedef osThreadId_t        whd_thread_type_t;                /** CMSIS definition of a thread handle */
typedef osMessageQueueId_t  whd_queue_type_t;                 /** CMSIS definition of a message queue */


/******************************************************
*                    Structures
******************************************************/


/******************************************************
*                 Global Variables
******************************************************/

/******************************************************
*               Function Declarations
******************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
