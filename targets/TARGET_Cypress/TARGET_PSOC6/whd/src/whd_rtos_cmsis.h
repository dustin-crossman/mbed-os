/*
 * $ Copyright Cypress Semiconductor $
 */
#pragma once

#include "cmsis_os2.h"
#include "whd_types.h"
#include "whd_debug.h"
#include "rtx_os.h"
#include "os_tick.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
*                      Macros
******************************************************/

/******************************************************
*                    Constants
******************************************************/

#define NEVER_TIMEOUT ( (uint32_t)0xffffffffUL )  /* Used with whd_rtos_get_semaphore */

#ifndef SYSTICK_FREQUENCY
#define SYSTICK_FREQUENCY  (1000)
#endif

/******************************************************
*                   Enumerations
******************************************************/

/******************************************************
*                 Type Definitions
******************************************************/
typedef osSemaphoreId_t whd_semaphore_type_t;             /** CMSIS definition of a semaphore */
typedef osMutexId_t whd_mutex_type_t;                     /** CMSIS definition of a mutex */
typedef osThreadId_t whd_thread_type_t;                   /** CMSIS definition of a thread handle */
typedef osMessageQueueId_t whd_queue_type_t;              /** CMSIS definition of a message queue */

/******************************************************
*                    Structures
******************************************************/

/******************************************************
*                 Global Variables
******************************************************/

/******************************************************
*               Function Declarations
******************************************************/

whd_result_t whd_rtos_cmsis_create_thread(whd_thread_type_t *thread, void (*entry_function)(
                                              uint32_t), const char *name,  void *stack, uint32_t stack_size, uint32_t priority, uint32_t arg);
whd_result_t whd_rtos_cmsis_finish_thread(whd_thread_type_t *thread);
whd_result_t whd_rtos_cmsis_delete_terminated_thread(whd_thread_type_t *thread);
whd_result_t whd_rtos_cmsis_join_thread(whd_thread_type_t *thread);
whd_result_t whd_rtos_cmsis_init_semaphore(/*@out@*/ whd_semaphore_type_t *semaphore);
whd_result_t whd_rtos_cmsis_get_semaphore(whd_semaphore_type_t *semaphore, uint32_t timeout_ms,
                                          whd_bool_t will_set_in_isr);
whd_result_t whd_rtos_cmsis_set_semaphore(whd_semaphore_type_t *semaphore, whd_bool_t called_from_ISR);
whd_result_t whd_rtos_cmsis_deinit_semaphore(whd_semaphore_type_t *semaphore);
whd_time_t whd_rtos_cmsis_get_time(void);
whd_result_t whd_rtos_cmsis_delay_milliseconds(uint32_t num_ms);

#ifdef __cplusplus
} /* extern "C" */
#endif
