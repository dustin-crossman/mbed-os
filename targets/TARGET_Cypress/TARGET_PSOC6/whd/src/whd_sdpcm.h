/*
 * $ Copyright Cypress Semiconductor Apache2 $
 */

/** @file
 *  Provides whd with function prototypes for IOCTL commands,
 *  and for communicating with the SDPCM module
 *
 */

#ifndef INCLUDED_WHD_SDPCM_H
#define INCLUDED_WHD_SDPCM_H

#include "whd.h"
#include "whd_events_int.h"
#include "whd_rtos.h"
#include "whd_network_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*             Constants
******************************************************/

/* CDC flag definition taken from bcmcdc.h */
#define CDCF_IOC_SET                (0x02)      /** 0=get, 1=set cmd */

typedef enum sdpcm_command_type_enum
{
    SDPCM_GET = 0x00, SDPCM_SET = CDCF_IOC_SET
} sdpcm_command_type_t;

/* IOCTL swapping mode for Big Endian host with Little Endian wlan.  Default to off */
#ifdef IL_BIGENDIAN
whd_bool_t swap = WHD_FALSE;
#define htod32(i) (swap ? bcmswap32(i) : i)
#define htod16(i) (swap ? bcmswap16(i) : i)
#define dtoh32(i) (swap ? bcmswap32(i) : i)
#define dtoh16(i) (swap ? bcmswap16(i) : i)
#else  /* IL_BIGENDIAN */
#ifndef htod32
#define htod32(i) ( (uint32_t)(i) )
#endif

#ifndef htod16
#define htod16(i) ( (uint16_t)(i) )
#endif

#ifndef dtoh32
#define dtoh32(i) ( (uint32_t)(i) )
#endif

#ifndef dtoh16
#define dtoh16(i) ( (uint16_t)(i) )
#endif
#endif  /* IL_BIGENDIAN */

/******************************************************
*             Macros
******************************************************/

#define IOCTL_OFFSET (sizeof(whd_buffer_header_t) + 12 + 16)

#define WHD_EVENT_HANDLER_LIST_SIZE    (5)      /** Maximum number of simultaneously registered event handlers */

/******************************************************
*             Structures
******************************************************/

/** Event list element structure
 *
 * events : A pointer to a whd_event_num_t array that is terminated with a WLC_E_NONE event
 * handler: A pointer to the whd_event_handler_t function that will receive the event
 * handler_user_data : User provided data that will be passed to the handler when a matching event occurs
 */
typedef struct
{
    const /*@null@*/ whd_event_num_t *events;
    /*@null@*/ whd_event_handler_t handler;
    /*@null@*/ void *handler_user_data;
    uint8_t ifidx;
} sdpcm_event_list_elem_t;

/** @endcond */

typedef struct whd_sdpcm_info
{
    /* Event list variables */
    sdpcm_event_list_elem_t whd_sdpcm_event_list[WHD_EVENT_HANDLER_LIST_SIZE];
    whd_semaphore_type_t event_list_mutex;

    /* IOCTL variables*/
    uint16_t requested_ioctl_id;
    whd_semaphore_type_t ioctl_mutex;
    /*@only@*/
    whd_buffer_t ioctl_response;
    whd_semaphore_type_t ioctl_sleep;

    /* Bus data credit variables */
    uint8_t packet_transmit_sequence_number;
    uint8_t last_bus_data_credit;
    uint8_t credit_diff;
    uint8_t largest_credit_diff;

    /* Packet send queue variables */
    whd_semaphore_type_t send_queue_mutex;
    whd_buffer_t /*@owned@*//*@null@*/ send_queue_head;
    whd_buffer_t /*@owned@*//*@null@*/ send_queue_tail;

} whd_sdpcm_info_t;

/******************************************************
*             Function declarations
******************************************************/

extern /*@null@*//*@exposed@*/ void *whd_sdpcm_get_iovar_buffer(/*@special@*//*@out@*/ whd_driver_t whd_driver,
                                                                                       whd_buffer_t *buffer,
                                                                                       uint16_t data_length,
                                                                                       const char *name) /*@allocates *buffer@*//*@defines **buffer@*/;
extern /*@null@*//*@exposed@*/ void *whd_sdpcm_get_ioctl_buffer(/*@special@*//*@out@*/ whd_driver_t whd_driver,
                                                                                       whd_buffer_t *buffer,
                                                                                       uint16_t data_length) /*@allocates *buffer@*//*@defines **buffer@*/;
extern whd_result_t whd_sdpcm_send_ioctl(whd_interface_t ifp, sdpcm_command_type_t type, uint32_t command,
                                         whd_buffer_t send_buffer_hnd,
                                         /*@null@*//*@out@*/ whd_buffer_t *response_buffer_hnd) /*@releases send_buffer_hnd@*/;
extern whd_result_t whd_sdpcm_send_iovar(whd_interface_t ifp, sdpcm_command_type_t type,
                                         /*@only@*/ whd_buffer_t send_buffer_hnd,
                                         /*@special@*//*@out@*//*@null@*/ whd_buffer_t *response_buffer_hnd) /*@allocates *response_buffer_hnd@*//*@defines **response_buffer_hnd@*/;
extern void whd_sdpcm_process_rx_packet(whd_driver_t whd_driver, /*@only@*/ whd_buffer_t buffer);
extern whd_result_t whd_sdpcm_init(whd_driver_t whd_driver);
extern void whd_sdpcm_quit(whd_driver_t whd_driver) /*@modifies internalState@*/;
extern void whd_sdpcm_bus_vars_init(whd_driver_t whd_driver);
extern void whd_sdpcm_quit(whd_driver_t whd_driver) /*@modifies internalState@*/;
extern whd_bool_t whd_sdpcm_has_tx_packet(whd_driver_t whd_driver);

extern whd_result_t whd_sdpcm_get_packet_to_send(whd_driver_t whd_driver, /*@out@*/ whd_buffer_t *buffer);
extern void whd_sdpcm_update_credit(whd_driver_t whd_driver, uint8_t *data);
extern uint8_t whd_sdpcm_get_available_credits(whd_driver_t whd_driver);
extern void whd_update_host_interface_to_bss_index_mapping(whd_driver_t whd_driver, whd_interface_t interface,
                                                           uint32_t bssid_index);
void whd_sdpcm_info_init(whd_driver_t whd_driver);

/******************************************************
*             Global variables
******************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_WHD_SDPCM_H */

