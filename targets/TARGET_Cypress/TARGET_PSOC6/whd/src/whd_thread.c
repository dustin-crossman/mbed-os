/*
 * $ Copyright Cypress Semiconductor Apache2 $
 */

/** @file
 *  Allows thread safe access to the WHD WiFi Driver (WHD) hardware bus
 *
 *  This file provides functions which allow multiple threads to use the WHD hardware bus (SDIO or SPI)
 *  This is achieved by having a single thread (the "WHD Thread") which queues messages to be sent, sending
 *  them sequentially, as well as receiving messages as they arrive.
 *
 *  Messages to be sent come from the @ref whd_sdpcm_send_common function in whd_sdpcm.c .  The messages already
 *  contain SDPCM headers, but not any bus headers (GSPI), and are passed to the whd_thread_send_data function.
 *  This function can be called from any thread.
 *
 *  Messages are received by way of a callback supplied by in SDPCM.c - whd_sdpcm_process_rx_packet
 *  Received messages are delivered in the context of the WHD Thread, so the callback function needs to avoid blocking.
 *
 *  It is also possible to use these functions without any operating system, by periodically calling the whd_thread_send_one_packet,
 *  @ref whd_thread_receive_one_packet or @ref whd_thread_poll_all functions
 *
 */
#include "whd_debug.h"
#include "whd_thread.h"
#include "bus_protocols/whd_bus_protocol_interface.h"
#include "whd_rtos_interface.h"
#include "whd_int.h"
#include "whd_chip.h"
#include "whd_poll.h"
#include "whd_sdpcm.h"
#include "whd_buffer_api.h"

/******************************************************
*             Static Function Prototypes
******************************************************/
/*@globals killed whd_transceive_semaphore@*/ /*@modifies whd_wlan_status, whd_bus_interrupt, whd_thread_quit_flag@*/
static void whd_thread_func(whd_thread_arg_t /*@unused@*/ thread_input);

/******************************************************
*             Global Functions
******************************************************/
void whd_thread_info_init(whd_driver_t whd_driver, whd_init_config_t *whd_init_config)
{
    memset(&whd_driver->thread_info, 0, sizeof(whd_driver->thread_info) );
    whd_driver->thread_info.thread_stack_start = whd_init_config->thread_stack_start;
    whd_driver->thread_info.thread_stack_size = whd_init_config->thread_stack_size;
    whd_driver->thread_info.thread_priority = whd_init_config->thread_priority;
}

/** Initialises the WHD Thread
 *
 * Initialises the WHD thread, and its flags/semaphores,
 * then starts it running
 *
 * @return    WHD_SUCCESS : if initialisation succeeds
 *            otherwise, a result code
 */
/*@globals undef whd_thread, undef whd_transceive_semaphore@*/ /*@modifies whd_inited@*/
whd_result_t whd_thread_init(whd_driver_t whd_driver)
{
    whd_result_t retval;

    retval = whd_sdpcm_init(whd_driver);

    if (retval != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Could not initialize SDPCM codec\n") );
        /*@-unreachable@*/ /*@-globstate@*/ /* Lint: Reachable after hitting assert & globals not defined due to error */
        return retval;
        /*@+unreachable@*/ /*@+globstate@*/
    }

    /* Create the event flag which signals the WHD thread needs to wake up */
    retval = whd_rtos_init_semaphore(&whd_driver->thread_info.transceive_semaphore);
    if (retval != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Could not initialize WHD thread semaphore\n") );
        /*@-unreachable@*/ /*@-globstate@*/ /* Lint: Reachable after hitting assert & globals not defined due to error */
        return retval;
        /*@+unreachable@*/ /*@+globstate@*/
    }

    retval = whd_rtos_create_thread_with_arg(&whd_driver->thread_info.whd_thread, whd_thread_func,
                                             "WHD", whd_driver->thread_info.thread_stack_start,
                                             whd_driver->thread_info.thread_stack_size,
                                             whd_driver->thread_info.thread_priority, (uint32_t)whd_driver);
    if (retval != WHD_SUCCESS)
    {
        /* Could not start WHD main thread */
        WPRINT_WHD_ERROR( ("Could not start WHD thread\n") );
        /*@-unreachable@*/ /* Reachable after hitting assert */
        return retval;
        /*@+unreachable@*/
    }

    /* Ready now. Indicate it here and in thread, whatever be executed first. */
    whd_driver->thread_info.whd_inited = WHD_TRUE;

    return WHD_SUCCESS;
}

/** Sends the first queued packet
 *
 * Checks the queue to determine if there is any packets waiting
 * to be sent. If there are, then it sends the first one.
 *
 * This function is normally used by the WHD Thread, but can be
 * called periodically by systems which have no RTOS to ensure
 * packets get sent.
 *
 * @return    1 : packet was sent
 *            0 : no packet sent
 */
int8_t whd_thread_send_one_packet(whd_driver_t whd_driver)   /*@modifies internalState @*/
{
    whd_buffer_t tmp_buf_hnd = NULL;
    whd_result_t result;

    if (whd_sdpcm_get_packet_to_send(whd_driver, &tmp_buf_hnd) != WHD_SUCCESS)
    {
        /*@-mustfreeonly@*/ /* Failed to get a packet */
        return 0;
        /*@+mustfreeonly@*/
    }

    /* Ensure the wlan backplane bus is up */
    result = whd_ensure_wlan_bus_is_up(whd_driver);
    if (result != WHD_SUCCESS)
    {
        whd_assert("Could not bring bus back up", 0 != 0);
        CHECK_RETURN(whd_buffer_release(whd_driver, tmp_buf_hnd, WHD_NETWORK_TX) );
        return 0;
    }

    WPRINT_WHD_DATA_LOG( ("Wcd:> Sending pkt 0x%08lX\n", (unsigned long)tmp_buf_hnd) );
    if (whd_bus_send_buffer(whd_driver, tmp_buf_hnd) != WHD_SUCCESS)
    {
        WHD_STATS_INCREMENT_VARIABLE(whd_driver, tx_fail);
        return 0;
    }

    WHD_STATS_INCREMENT_VARIABLE(whd_driver, tx_total);
    return (int8_t)1;
}

/** Receives a packet if one is waiting
 *
 * Checks the wifi chip fifo to determine if there is any packets waiting
 * to be received. If there are, then it receives the first one, and calls
 * the callback @ref whd_sdpcm_process_rx_packet (in whd_sdpcm.c).
 *
 * This function is normally used by the WHD Thread, but can be
 * called periodically by systems which have no RTOS to ensure
 * packets get received properly.
 *
 * @return    1 : packet was received
 *            0 : no packet waiting
 */
int8_t whd_thread_receive_one_packet(whd_driver_t whd_driver)
{
    /* Check if there is a packet ready to be received */
    whd_buffer_t recv_buffer;
    if (whd_bus_read_frame(whd_driver, &recv_buffer) != WHD_SUCCESS)
    {
        /*@-mustfreeonly@*/ /* Failed to read a packet */
        return 0;
        /*@+mustfreeonly@*/
    }

    if (recv_buffer != NULL)    /* Could be null if it was only a credit update */
    {

        WPRINT_WHD_DATA_LOG( ("Wcd:< Rcvd pkt 0x%08lX\n", (unsigned long)recv_buffer) );
        WHD_STATS_INCREMENT_VARIABLE(whd_driver, rx_total);

        /* Send received buffer up to SDPCM layer */
        whd_sdpcm_process_rx_packet(whd_driver, recv_buffer);
    }
    return (int8_t)1;
}

/** Sends and Receives all waiting packets
 *
 * Calls whd_thread_send_one_packet and whd_thread_receive_one_packet
 * once to send and receive packets, until there are no more packets waiting to
 * be transferred.
 *
 * This function is normally used by the WHD Thread, but can be
 * called periodically by systems which have no RTOS to ensure
 * packets get send and received properly.
 *
 * Note: do not loop in here, to avoid overwriting previously rx-ed packets
 */
int8_t whd_thread_poll_all(whd_driver_t whd_driver)   /*@modifies internalState@*/
{
    int8_t result = 0;
    result |= whd_thread_send_one_packet(whd_driver);
    result |= whd_thread_receive_one_packet(whd_driver);
    return result;
}

/** Terminates the WHD Thread
 *
 * Sets a flag then wakes the WHD Thread to force it to terminate.
 *
 */
void whd_thread_quit(whd_driver_t whd_driver)
{
    whd_result_t result;

    /* signal main thread and wake it */
    whd_driver->thread_info.thread_quit_flag = WHD_TRUE;
    result = whd_rtos_set_semaphore(&whd_driver->thread_info.transceive_semaphore, WHD_FALSE);
    if (result == WHD_SUCCESS)
    {
        /* Wait for the WHD thread to end */
        whd_rtos_join_thread(&whd_driver->thread_info.whd_thread);

        /* Ignore return - not much can be done about failure */
        (void)whd_rtos_delete_terminated_thread(&whd_driver->thread_info.whd_thread);
    }
    else
    {
        WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );
    }
}

/**
 * Informs WHD of an interrupt
 *
 * This function should be called from the SDIO/SPI interrupt function
 * and usually indicates newly received data is available.
 * It wakes the WHD Thread, forcing it to check the send/receive
 *
 */
/* ignore failure since there is nothing that can be done about it in a ISR */
void whd_thread_notify_irq(whd_driver_t whd_driver)
{
    whd_driver->thread_info.bus_interrupt = WHD_TRUE;

    /* just wake up the main thread and let it deal with the data */
    if (whd_driver->thread_info.whd_inited == WHD_TRUE)
    {
        (void)whd_rtos_set_semaphore(&whd_driver->thread_info.transceive_semaphore, WHD_TRUE);
    }
}

void whd_thread_notify(whd_driver_t whd_driver)
{
    /* just wake up the main thread and let it deal with the data */
    if (whd_driver->thread_info.whd_inited == WHD_TRUE)
    {
        /* Ignore return - not much can be done about failure */
        (void)whd_rtos_set_semaphore(&whd_driver->thread_info.transceive_semaphore, WHD_FALSE);
    }
}

/******************************************************
*             Static Functions
******************************************************/

/** The WHD Thread function
 *
 *  This is the main loop of the WHD Thread.
 *  It simply calls @ref whd_thread_poll_all to send/receive all waiting packets, then goes
 *  to sleep.  The sleep has a 100ms timeout, causing the send/receive queues to be
 *  checked 10 times per second in case an interrupt is missed.
 *  Once the quit flag has been set, flags/mutexes are cleaned up, and the function exits.
 *
 * @param thread_input  : unused parameter needed to match thread prototype.
 *
 */
/*@globals killed whd_transceive_semaphore@*/
/*@modifies whd_wlan_status, whd_bus_interrupt, whd_thread_quit_flag, whd_inited, whd_thread@*/
static void whd_thread_func(whd_thread_arg_t /*@unused@*/ thread_input)
{
    int8_t rx_status;
    int8_t tx_status;
    whd_driver_t whd_driver = ( whd_driver_t )thread_input;
    whd_thread_info_t *thread_info = &whd_driver->thread_info;

    WPRINT_WHD_DATA_LOG( ("Started whd Thread\n") );

    /* Interrupts may be enabled inside thread. To make sure none lost set flag now. */
    thread_info->whd_inited = WHD_TRUE;

    while (thread_info->thread_quit_flag != WHD_TRUE)
    {
        /* Check if we were woken by interrupt */
        if ( (thread_info->bus_interrupt == WHD_TRUE) ||
             (whd_bus_use_status_report_scheme(whd_driver) ) )
        {
            thread_info->bus_interrupt = WHD_FALSE;

            /* Check if the interrupt indicated there is a packet to read */
            if (whd_bus_packet_available_to_read(whd_driver) != 0)
            {
                /* Receive all available packets */
                do
                {
                    rx_status = whd_thread_receive_one_packet(whd_driver);
                } while (rx_status != 0);
            }
        }

        /* Send all queued packets */
        do
        {
            tx_status = whd_thread_send_one_packet(whd_driver);
        } while (tx_status != 0);

        /* Sleep till WLAN do something */
        whd_bus_wait_for_wlan_event(whd_driver, &thread_info->transceive_semaphore);
        WPRINT_WHD_DATA_LOG( ("whd Thread: Woke\n") );
    }

    /* Set flag before releasing objects */
    thread_info->whd_inited = WHD_FALSE;

    /* Reset the quit flag */
    thread_info->thread_quit_flag = WHD_FALSE;

    /* Delete the semaphore */
    /* Ignore return - not much can be done about failure */
    (void)whd_rtos_deinit_semaphore(&thread_info->transceive_semaphore);

    whd_sdpcm_quit(whd_driver);

    WPRINT_WHD_DATA_LOG( ("Stopped whd Thread\n") );

    if (WHD_SUCCESS != whd_rtos_finish_thread(&thread_info->whd_thread) )
    {
        WPRINT_WHD_DEBUG( ("Could not close WHD thread\n") );
    }
}

