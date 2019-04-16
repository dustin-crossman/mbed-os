/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *  Defines the WHD RTOS Interface.
 *
 *  Provides prototypes for functions that allow WHD to use RTOS resources
 *  such as threads, semaphores & timing functions in an abstract way.
 */

#ifndef INCLUDED_WHD_RTOS_INTERFACE_H_
#define INCLUDED_WHD_RTOS_INTERFACE_H_

#include "whd_rtos.h"
#include "whd_types.h"

#ifdef __x86_64__
typedef uint64_t whd_thread_arg_t;
#else
typedef uint32_t whd_thread_arg_t;
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
* @cond       Constants
******************************************************/

#define NEVER_TIMEOUT ( (uint32_t)0xffffffffUL ) /* Used with whd_rtos_get_semaphore */

/******************************************************
*             Structures & Typedefs
******************************************************/

/** @endcond */

/** @addtogroup rtosif RTOS Interface
 * Allows WHD to use use RTOS resources
 *  such as threads, semaphores & timing functions in an abstract way.
 *  @{
 */

/******************************************************
*             Function declarations
******************************************************/

/* Thread management functions */

/**
 * Create a thread
 *
 * Implemented in the Port Layer RTOS interface which is specific to the
 * RTOS in use.
 * WHD uses this function to create a new thread and start it running.
 *
 * @param thread         : pointer to a variable which will receive the new thread handle
 * @param entry_function : function pointer which points to the main function for the new thread
 * @param name           : a string thread name used for a debugger
 * @param stack_size     : the size of the thread stack in bytes
 * @param priority       : the priority of the thread
 *
 * @return WHD_SUCCESS or Error code
 */
extern whd_result_t whd_rtos_create_thread(/*@out@*/ whd_thread_type_t *thread, void (*entry_function)(
                                                         whd_thread_arg_t arg), const char *name, /*@null@*/ void *stack, uint32_t stack_size, uint32_t priority) /*@modifies *thread@*/;

/**
 * Create a thread with specific thread argument
 *
 * Implemented in the Port Layer RTOS interface which is specific to the
 * RTOS in use.
 *
 * @param thread         : pointer to a variable which will receive the new thread handle
 * @param entry_function : function pointer which points to the main function for the new thread
 * @param name           : a string thread name used for a debugger
 * @param stack_size     : the size of the thread stack in bytes
 * @param priority       : the priority of the thread
 * @param arg            : the argument to pass to the new thread
 *
 * @return WHD result code
 */
extern whd_result_t whd_rtos_create_thread_with_arg(/*@out@*/ whd_thread_type_t *thread, void (*entry_function)(
                                                                  whd_thread_arg_t arg), const char *name, /*@null@*/ void *stack, uint32_t stack_size, uint32_t priority, whd_thread_arg_t arg);

/**
 * Note: different RTOS have different parameters for creating threads.
 * Use this function carefully if portability is important.
 * Create a thread with RTOS specific thread argument (E.g. specify time-slicing behavior)
 *
 * Implemented in the Port Layer RTOS interface which is specific to the
 * RTOS in use.
 *
 * @param thread         : pointer to a variable which will receive the new thread handle
 * @param entry_function : function pointer which points to the main function for the new thread
 * @param name           : a string thread name used for a debugger
 * @param stack_size     : the size of the thread stack in bytes
 * @param priority       : the priority of the thread
 * @param arg            : the argument to pass to the new thread
 * @param config        : os specific thread configuration
 * @return WHD result code
 */
extern whd_result_t whd_rtos_create_configed_thread(/*@out@*/ whd_thread_type_t *thread, void (*entry_function)(
                                                                  uint32_t), const char *name, /*@null@*/ void *stack, uint32_t stack_size, uint32_t priority, whd_rtos_thread_config_type_t *config);

/**
 * Exit a thread
 *
 * Implemented in the Port Layer RTOS interface which is specific to the
 * RTOS in use.
 * WHD uses this function to exit the current thread just before its main
 * function would otherwise return. Some RTOSs allow threads to exit by simply returning
 * from their main function. If this is the case, then the implementation of
 * this function should be empty.
 *
 * @param thread         : Pointer to the current thread handle
 *
 * @return WHD_SUCCESS or Error code
 */
extern whd_result_t whd_rtos_finish_thread(whd_thread_type_t *thread) /*@modifies *thread@*/;

/**
 * Deletes a terminated thread
 *
 * Implemented in the Port Layer RTOS interface which is specific to the
 * RTOS in use.
 * Some RTOS implementations require that another thread deletes any terminated thread
 * If RTOS does not require this, leave empty
 *
 * @param thread         : handle of the terminated thread to delete
 *
 * @returns WHD_SUCCESS on success, Error code otherwise
 */
extern whd_result_t whd_rtos_delete_terminated_thread(whd_thread_type_t *thread);

/**
 * Waits for a thread to complete
 *
 * Implemented in the Port Layer RTOS interface which is specific to the
 * RTOS in use.
 *
 * @param thread         : handle of the thread to wait for
 *
 * @returns WHD_SUCCESS on success, Error code otherwise
 */
extern whd_result_t whd_rtos_join_thread(whd_thread_type_t *thread);

/* Semaphore management functions */

/**
 * Create a Semaphore
 *
 * Implemented in the Port Layer RTOS interface which is specific to the
 * RTOS in use.
 * WHD uses this function to create a semaphore
 *
 * @param semaphore         : Pointer to the semaphore handle to be initialized
 *
 * @return WHD_SUCCESS or Error code
 */
/*@allocates *semaphore@*//*@defines **semaphore@*/
extern whd_result_t whd_rtos_init_semaphore(/*@special@*//*@out@*/ whd_semaphore_type_t *semaphore);

/**
 * Get a semaphore
 *
 * Implemented in the Port Layer RTOS interface which is specific to the
 * RTOS in use.
 * WHD uses this function to get a semaphore.
 * - If the semaphore value is greater than zero, the sempahore value is decremented and the function returns immediately.
 * - If the semaphore value is zero, the current thread is put on a queue of threads waiting on the
 *   semaphore, and sleeps until another thread sets the semaphore and causes it to wake. Upon waking, the
 *   semaphore is decremented and the function returns.
 *
 * @note : This function must not be called from an interrupt context as it may block.
 *
 * @param semaphore   : Pointer to the semaphore handle
 * @param timeout_ms  : Maximum number of milliseconds to wait while attempting to get
 *                      the semaphore. Use the NEVER_TIMEOUT constant to wait forever.
 * @param will_set_in_isr : True if the semaphore will be set in an ISR. Currently only used for NoOS/NoNS
 *
 * @return whd_result_t : WHD_SUCCESS if semaphore was successfully acquired
 *                     : WHD_TIMEOUT if semaphore was not acquired before timeout_ms period
 */
/*@modifies internalState@*/
extern whd_result_t whd_rtos_get_semaphore(whd_semaphore_type_t *semaphore, uint32_t timeout_ms,
                                           whd_bool_t will_set_in_isr);

/**
 * Set a semaphore
 *
 * Implemented in the Port Layer RTOS interface which is specific to the
 * RTOS in use.
 * WHD uses this function to set a semaphore.
 * The value of the semaphore is incremented, and if there are any threads waiting on the semaphore,
 * then the first waiting thread is woken.
 *
 * Some RTOS implementations require different processing when setting a semaphore from within an ISR.
 * A parameter is provided to allow this.
 *
 * @param semaphore       : Pointer to the semaphore handle
 * @param called_from_ISR : Value of WHD_TRUE indicates calling from interrupt context
 *                          Value of WHD_FALSE indicates calling from normal thread context
 *
 * @return whd_result_t : WHD_SUCCESS if semaphore was successfully set
 *                        : Error code if an error occurred
 *
 */
extern whd_result_t whd_rtos_set_semaphore(whd_semaphore_type_t *semaphore, whd_bool_t called_from_ISR);

/**
 * Deletes a semaphore
 *
 * Implemented in the Port Layer RTOS interface which is specific to the
 * RTOS in use.
 * WHD uses this function to delete a semaphore.
 *
 * @param semaphore         : Pointer to the semaphore handle
 *
 * @return whd_result_t : WHD_SUCCESS if semaphore was successfully deleted
 *                        : Error code if an error occurred
 */
/*@releases *semaphore@*/
extern whd_result_t whd_rtos_deinit_semaphore(/*@special@*/ whd_semaphore_type_t *semaphore);

/* Time management functions */

/**
 * Gets time in milliseconds since RTOS start
 *
 * Implemented in the Port Layer RTOS interface which is specific to the
 * RTOS in use.
 * WHD uses this function to retrieve the current time.
 *
 * @note: Since this is only 32 bits, it will roll over every 49 days, 17 hours, 2 mins, 47.296 seconds
 *
 * @returns Time in milliseconds since the RTOS started.
 */
/*@modifies internalState@*/
extern whd_time_t whd_rtos_get_time(void);

/**
 * Delay for a number of milliseconds
 *
 * Implemented in the Port Layer RTOS interface which is specific to the
 * RTOS in use.
 * WHD uses this function to delay processing
 * Processing of this function depends on the minimum sleep
 * time resolution of the RTOS.
 * The current thread should sleep for the longest period possible which
 * is less than the delay required, then makes up the difference
 * with a tight loop
 *
 * @return whd_result_t : WHD_SUCCESS if delay was successful
 *                        : Error code if an error occurred
 *
 */
extern whd_result_t whd_rtos_delay_milliseconds(uint32_t num_ms);

/* Message queue management functions */
/*@allocates *queue@*//*@defines **queue@*/
extern whd_result_t whd_rtos_init_queue(/*@special@*//*@out@*/ whd_queue_type_t *queue, void *buffer,
                                                               uint32_t buffer_size,
                                                               uint32_t message_size);

extern whd_result_t whd_rtos_push_to_queue(whd_queue_type_t *queue, void *message, uint32_t timeout_ms);

extern whd_result_t whd_rtos_pop_from_queue(whd_queue_type_t *queue, void *message, uint32_t timeout_ms);

extern whd_result_t whd_rtos_deinit_queue(/*@special@*/ whd_queue_type_t *queue); /*@releases *queue@*/

extern whd_result_t whd_rtos_init_mutex(whd_mutex_type_t *mutex);
extern whd_result_t whd_rtos_lock_mutex(whd_mutex_type_t *mutex);
extern whd_result_t whd_rtos_unlock_mutex(whd_mutex_type_t *mutex);
extern whd_result_t whd_rtos_deinit_mutex(whd_mutex_type_t *mutex);

unsigned long whd_rtos_get_tickrate(void);

/* Support timers for WHD */
typedef void (whd_rtos_timer_handler_t)(void *);
typedef int (whd_rtos_init_timer_t)(void *timer, uint32_t time_ms, whd_rtos_timer_handler_t function, void *arg);
typedef int (whd_rtos_start_timer_t)(void *timer);
typedef int (whd_rtos_stop_timer_t)(void *timer);
typedef int (whd_rtos_deinit_timer_t)(void *timer);
typedef int (whd_rtos_is_timer_running_t)(void *timer);
typedef int (whd_rtos_alloc_timer_t)(void **timer);
typedef int (whd_rtos_free_timer_t)(void **timer);

typedef struct
{
    /* order of operations should be: first allocate, then init, then start/stop/is_running any number of times,
     * then deinit, then free */
    whd_rtos_alloc_timer_t *whd_rtos_alloc_timer;

    /* These functions behave identically to the whd_rtos.h functions */
    whd_rtos_init_timer_t *whd_rtos_init_timer;

    whd_rtos_start_timer_t *whd_rtos_start_timer;
    whd_rtos_stop_timer_t *whd_rtos_stop_timer;
    whd_rtos_is_timer_running_t *whd_rtos_is_timer_running;

    whd_rtos_deinit_timer_t *whd_rtos_deinit_timer;

    /* call after successful deinit */
    whd_rtos_free_timer_t *whd_rtos_free_timer;
} whd_rtos_timer_ifc_t;

extern const whd_rtos_timer_ifc_t *whd_rtos_timer_ifc;

/**
 * Register a timer interface for WHD to use.
 *
 * @return whd_result_t : WHD_SUCCESS if successful
 *                        : Error code if an error occurred
 *
 */
extern whd_result_t whd_rtos_register_timer_ifc(const whd_rtos_timer_ifc_t *whd_rtos_timer_ifc);

/**
 * Unregister a timer interface WHD was using.
 *
 * @return whd_result_t : WHD_SUCCESS if successful
 *                        : Error code if an error occurred
 *
 */
extern whd_result_t whd_rtos_unregister_timer_ifc(const whd_rtos_timer_ifc_t *whd_rtos_timer_ifc);

#define HOST_RTOS_TIMER_IFC whd_rtos_timer_ifc
#define HOST_RTOS_TIMER_IFC_CALL(func) (*(HOST_RTOS_TIMER_IFC->func) )

/** @} */

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* ifndef INCLUDED_WHD_RTOS_INTERFACE_H_ */

