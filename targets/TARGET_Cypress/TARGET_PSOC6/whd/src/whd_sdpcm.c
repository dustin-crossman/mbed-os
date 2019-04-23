/*
 * $ Copyright Cypress Semiconductor Apache2 $
 */

/** @file
 *  Provides an implementation of the Broadcom SDPCM protocol.
 *  The Broadcom SDPCM protocol provides multiplexing of Wireless Data frames,
 *  I/O Control functions (IOCTL), and Asynchronous Event signalling.
 *  It is required when communicating with Broadcom 802.11 devices.
 *
 */
#include "whd_sdpcm.h"
#include "bus_protocols/whd_bus_protocol_interface.h"
#include "whd_endian.h"
#include "whd_chip_constants.h"
#include "whd_chip.h"
#include "whd_thread_internal.h"
#include "whd_debug.h"
#include "whd_events_int.h"
#include "whd_wifi_api.h"
#include "whd_buffer_api.h"
#include "whd_network_if.h"
#include "whd_wlioctl.h"
#include "whd_types_int.h"

/******************************************************
* @cond       Constants
******************************************************/
#define WHD_NEVER_TIMEOUT                         (0xFFFFFFFF)
#define WHD_IOCTL_PACKET_TIMEOUT      WHD_NEVER_TIMEOUT

#define WHD_IOCTL_TIMEOUT_MS         (5000)     /** Need to give enough time for coming out of Deep sleep (was 400) */
#define WHD_IOCTL_MAX_TX_PKT_LEN     (1500)


#define BDC_PROTO_VER                  (2)      /** Version number of BDC header */
#define BDC_FLAG_VER_SHIFT             (4)      /** Number of bits to shift BDC version number in the flags field */
#define ETHER_TYPE_BRCM           (0x886C)      /** Broadcom Ethertype for identifying event packets - Copied from DHD include/proto/ethernet.h */
#define BRCM_OUI            "\x00\x10\x18"      /** Broadcom OUI (Organizationally Unique Identifier): Used in the proprietary(221) IE (Information Element) in all Broadcom devices */
#define BCM_MSG_IFNAME_MAX            (16)      /** Maximum length of an interface name in a wl_event_msg_t structure*/

/* QoS related definitions (type of service) */
#define IPV4_DSCP_OFFSET              (15)      /** Offset for finding the DSCP field in an IPv4 header */

/* CDC flag definitions taken from bcmcdc.h */
#define CDCF_IOC_ERROR              (0x01)      /** 0=success, 1=ioctl cmd failed */
#define CDCF_IOC_IF_MASK          (0xF000)      /** I/F index */
#define CDCF_IOC_IF_SHIFT             (12)      /** # of bits of shift for I/F Mask */
#define CDCF_IOC_ID_MASK      (0xFFFF0000)      /** used to uniquely id an ioctl req/resp pairing */
#define CDCF_IOC_ID_SHIFT             (16)      /** # of bits of shift for ID Mask */

#define BDC_FLAG2_IF_MASK           (0x0f)

#define SDPCM_HEADER_LEN              (12)
#define BDC_HEADER_LEN                 (4)

/* Event flags */
#define WLC_EVENT_MSG_LINK      (0x01)    /** link is up */
#define WLC_EVENT_MSG_FLUSHTXQ  (0x02)    /** flush tx queue on MIC error */
#define WLC_EVENT_MSG_GROUP     (0x04)    /** group MIC error */
#define WLC_EVENT_MSG_UNKBSS    (0x08)    /** unknown source bsscfg */
#define WLC_EVENT_MSG_UNKIF     (0x10)    /** unknown source OS i/f */

/******************************************************
*             Macros
******************************************************/

/* bit map related macros */
#define  NBBY  8  /* 8 bits per byte */
#define  setbit(a, i)   ( ( (uint8_t *)a )[(int)(i) / (int)(NBBY)] |= (uint8_t)(1 << ( (i) % NBBY ) ) )
#define  clrbit(a, i)   ( ( (uint8_t *)a )[(int)(i) / (int)(NBBY)] &= (uint8_t) ~(1 << ( (i) % NBBY ) ) )
#define  isset(a, i)    ( ( (const uint8_t *)a )[(int)(i) / (int)(NBBY)] & (1 << ( (i) % NBBY ) ) )
#define  isclr(a, i)    ( ( ( (const uint8_t *)a )[(int)(i) / (int)(NBBY)] & (1 << ( (i) % NBBY ) ) ) == 0 )

#define DATA_AFTER_HEADER(x)   ( (void *)(&x[1]) )

/******************************************************
*             Local Structures
******************************************************/

typedef enum
{
    DATA_HEADER       = 2,
    ASYNCEVENT_HEADER = 1,
    CONTROL_HEADER    = 0
} sdpcm_header_type_t;

#pragma pack(1)

/*TODO: Keep this typedef? (in preference to the defines above */
#if 0
typedef struct
{
    uint16_t control_id;
    uint8_t interface_index : 4;
    uint16_t reserved        : 10;
    uint8_t set             : 1;
    uint8_t error           : 1;
}sdpcm_cdc_flags_t;
#endif /* if 0 */
typedef struct
{
    uint8_t sequence;
    uint8_t channel_and_flags;
    uint8_t next_length;
    uint8_t header_length;
    uint8_t wireless_flow_control;
    uint8_t bus_data_credit;
    uint8_t _reserved[2];
} sdpcm_sw_header_t;

typedef struct
{
    uint32_t cmd;    /* ioctl command value */
    uint32_t len;    /* lower 16: output buflen; upper 16: input buflen (excludes header) */
    uint32_t flags;  /* flag defns given in bcmcdc.h */
    uint32_t status; /* status code returned from the device */
} sdpcm_cdc_header_t;

typedef struct
{
    uint8_t flags;      /* Flags */
    uint8_t priority;   /* 802.1d Priority (low 3 bits) */
    uint8_t flags2;
    uint8_t data_offset; /* Offset from end of BDC header to packet data, in 4-uint8_t words.
                          * Leaves room for optional headers.*/
} sdpcm_bdc_header_t;

typedef struct
{
    whd_mac_t destination_address;
    whd_mac_t source_address;
    uint16_t ethertype;
} sdpcm_ethernet_header_t;


/*
 * SDPCM header definitions
 */
typedef struct
{
    uint16_t frametag[2];
    sdpcm_sw_header_t sw_header;
} sdpcm_header_t;

/*
 * SDPCM Packet structure definitions
 */
typedef struct
{
    whd_buffer_header_t buffer_header;
    sdpcm_header_t sdpcm_header;
} sdpcm_common_header_t;

typedef struct
{
    sdpcm_common_header_t common;
    sdpcm_cdc_header_t cdc_header;
} sdpcm_control_header_t;

typedef struct
{
    sdpcm_common_header_t common;
    uint8_t _padding[2];
    sdpcm_bdc_header_t bdc_header;
} sdpcm_data_header_t;

typedef struct bcmeth_hdr
{
    uint16_t subtype;      /** Vendor specific..32769 */
    uint16_t length;
    uint8_t version;       /** Version is 0 */
    uint8_t oui[3];        /** Broadcom OUI */
    uint16_t usr_subtype;  /** user specific Data */
} sdpcm_bcmeth_header_t;

/* these fields are stored in network order */
typedef struct
{

    uint16_t version;                         /** Version 1 has fields up to ifname.
                                               * Version 2 has all fields including ifidx and bss_cfg_idx */
    uint16_t flags;                           /** see flags */
    uint32_t event_type;                      /** Message */
    uint32_t status;                          /** Status code */
    uint32_t reason;                          /** Reason code (if applicable) */
    uint32_t auth_type;                       /** WLC_E_AUTH */
    uint32_t datalen;                         /** data buf */
    whd_mac_t addr;                         /** Station address (if applicable) */
    char ifname[BCM_MSG_IFNAME_MAX];          /** name of the packet incoming interface */
    uint8_t ifidx;                            /** destination OS i/f index */
    uint8_t bss_cfg_idx;                      /** source bsscfg index */
} sdpcm_raw_event_header_t;

/* used by driver msgs */
typedef struct bcm_event
{
    sdpcm_ethernet_header_t ether;
    sdpcm_bcmeth_header_t bcmeth;
    union
    {
        whd_event_header_t whd;
        sdpcm_raw_event_header_t raw;
    } event;
} sdpcm_bcm_event_t;

#pragma pack()

/******************************************************
*             Static Variables
******************************************************/

/* helper function for event messages ext API */
static uint8_t *whd_management_alloc_event_msgs_buffer(whd_interface_t ifp, whd_buffer_t *buffer);
/******************************************************
*             SDPCM Logging
*
* Enable this section to allow logging of SDPCM packets
* into a buffer for later perusal
*
* See sdpcm_log  and  next_sdpcm_log_pos
*
******************************************************/
/** @cond */

#if 0

#define SDPCM_LOG_SIZE 30
#define SDPCM_LOG_HEADER_SIZE (0x60)

typedef enum { UNUSED, LOG_TX, LOG_RX } sdpcm_log_direction_t;
typedef enum { IOCTL, DATA, EVENT } sdpcm_log_type_t;

typedef struct SDPCM_log_entry_struct
{
    sdpcm_log_direction_t direction;
    sdpcm_log_type_t type;
    unsigned long time;
    unsigned long length;
    unsigned char header[SDPCM_LOG_HEADER_SIZE];
}sdpcm_log_entry_t;

static int next_sdpcm_log_pos = 0;
static sdpcm_log_entry_t sdpcm_log[SDPCM_LOG_SIZE];

static void add_sdpcm_log_entry(sdpcm_log_direction_t dir, sdpcm_log_type_t type, unsigned long length, char *eth_data)
{

    sdpcm_log[next_sdpcm_log_pos].direction = dir;
    sdpcm_log[next_sdpcm_log_pos].type = type;
    sdpcm_log[next_sdpcm_log_pos].time = whd_rtos_get_time();
    sdpcm_log[next_sdpcm_log_pos].length = length;
    memcpy(sdpcm_log[next_sdpcm_log_pos].header, eth_data, SDPCM_LOG_HEADER_SIZE);
    next_sdpcm_log_pos++;
    if (next_sdpcm_log_pos >= SDPCM_LOG_SIZE)
    {
        next_sdpcm_log_pos = 0;
    }
}

#else
#define add_sdpcm_log_entry(dir, type, length, eth_data)
#endif

/** @endcond */

/******************************************************
*             Static Function Prototypes
******************************************************/

static whd_buffer_t  whd_sdpcm_get_next_buffer_in_queue(whd_driver_t whd_driver, whd_buffer_t buffer);
static void            whd_sdpcm_set_next_buffer_in_queue(whd_driver_t whd_driver, whd_buffer_t buffer,
                                                          whd_buffer_t prev_buffer);
static void            whd_sdpcm_send_common(whd_driver_t whd_driver, /*@only@*/ whd_buffer_t buffer,
                                             sdpcm_header_type_t header_type);
static uint8_t         whd_map_dscp_to_priority(whd_driver_t whd_driver, uint8_t dscp_val);
extern void whd_wifi_log_event(whd_driver_t whd_driver, const whd_event_header_t *event_header,
                               const uint8_t *event_data);
/******************************************************
*             Function definitions
******************************************************/

void whd_sdpcm_info_init(whd_driver_t whd_driver)
{
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;

    sdpcm_info->last_bus_data_credit    = 0;
    sdpcm_info->credit_diff             = 0;
    sdpcm_info->largest_credit_diff     = 0;

    /* Packet send queue variables */
    sdpcm_info->send_queue_head   = (whd_buffer_t)NULL;
    sdpcm_info->send_queue_tail   = (whd_buffer_t)NULL;
}

/** Initialises the SDPCM protocol handler
 *
 *  Initialises mutex and semaphore flags needed by the SDPCM handler.
 *  Also initialises the list of event handlers. This function is called
 *  from the @ref whd_thread_init function.
 *
 * @return    WHD result code
 */

whd_result_t whd_sdpcm_init(whd_driver_t whd_driver)
{
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;

    /* Create the mutex protecting the packet send queue */
    if (whd_rtos_init_semaphore(&sdpcm_info->ioctl_mutex) != WHD_SUCCESS)
    {
        return WHD_SEMAPHORE_ERROR;
    }
    if (whd_rtos_set_semaphore(&sdpcm_info->ioctl_mutex, WHD_FALSE) != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );
        return WHD_SEMAPHORE_ERROR;
    }

    /* Create the event flag which signals the whd thread needs to wake up */
    if (whd_rtos_init_semaphore(&sdpcm_info->ioctl_sleep) != WHD_SUCCESS)
    {
        whd_rtos_deinit_semaphore(&sdpcm_info->ioctl_mutex);
        return WHD_SEMAPHORE_ERROR;
    }

    /* Create the sdpcm packet queue semaphore */
    if (whd_rtos_init_semaphore(&sdpcm_info->send_queue_mutex) != WHD_SUCCESS)
    {
        whd_rtos_deinit_semaphore(&sdpcm_info->ioctl_sleep);
        whd_rtos_deinit_semaphore(&sdpcm_info->ioctl_mutex);
        return WHD_SEMAPHORE_ERROR;
    }
    if (whd_rtos_set_semaphore(&sdpcm_info->send_queue_mutex, WHD_FALSE) != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );
        return WHD_SEMAPHORE_ERROR;
    }

    /* Create semaphore to protect event list management */
    if (whd_rtos_init_semaphore(&sdpcm_info->event_list_mutex) != WHD_SUCCESS)
    {
        whd_rtos_deinit_semaphore(&sdpcm_info->ioctl_sleep);
        whd_rtos_deinit_semaphore(&sdpcm_info->ioctl_mutex);
        whd_rtos_deinit_semaphore(&sdpcm_info->send_queue_mutex);
        return WHD_SEMAPHORE_ERROR;
    }
    if (whd_rtos_set_semaphore(&sdpcm_info->event_list_mutex, WHD_FALSE) != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );
        return WHD_SEMAPHORE_ERROR;
    }

    sdpcm_info->send_queue_head = NULL;
    sdpcm_info->send_queue_tail = NULL;

    /* Initialise the list of event handler functions */
    memset(sdpcm_info->whd_sdpcm_event_list, 0, sizeof(sdpcm_info->whd_sdpcm_event_list) );

    whd_sdpcm_bus_vars_init(whd_driver);

    return WHD_SUCCESS;
}

/* Re-initialize the bus variables after deep sleep */
void whd_sdpcm_bus_vars_init(whd_driver_t whd_driver)
{
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;

    /* Bus data credit variables */
    sdpcm_info->credit_diff                     = 0;
    sdpcm_info->largest_credit_diff             = 0;

    sdpcm_info->packet_transmit_sequence_number = 0;
    sdpcm_info->last_bus_data_credit            = (uint8_t)1;
}

/** Initialises the SDPCM protocol handler
 *
 *  De-initialises mutex and semaphore flags needed by the SDPCM handler.
 *  This function is called from the @ref whd_thread_func function when it is exiting.
 */
/*@globals killed whd_sdpcm_ioctl_sleep, killed whd_sdpcm_ioctl_mutex, killed whd_sdpcm_send_queue_mutex@*/
/*@modifies whd_sdpcm_send_queue_head@*/

void whd_sdpcm_quit(whd_driver_t whd_driver)
{
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;
    whd_result_t result;

    /* Delete the sleep mutex */
    (void)whd_rtos_deinit_semaphore(&sdpcm_info->ioctl_sleep);    /* Ignore return - not much can be done about failure */

    /* Delete the queue mutex.  */
    (void)whd_rtos_deinit_semaphore(&sdpcm_info->ioctl_mutex);    /* Ignore return - not much can be done about failure */

    /* Delete the SDPCM queue mutex */
    (void)whd_rtos_deinit_semaphore(&sdpcm_info->send_queue_mutex);    /* Ignore return - not much can be done about failure */

    /* Delete the event list management mutex */
    (void)whd_rtos_deinit_semaphore(&sdpcm_info->event_list_mutex);    /* Ignore return - not much can be done about failure */

    /* Free any left over packets in the queue */
    while (sdpcm_info->send_queue_head != NULL)
    {
        whd_buffer_t buf = whd_sdpcm_get_next_buffer_in_queue(whd_driver, sdpcm_info->send_queue_head);
        result = whd_buffer_release(whd_driver, sdpcm_info->send_queue_head, WHD_NETWORK_TX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
        sdpcm_info->send_queue_head = buf;
    }
}

/** Sets/Gets an I/O Variable (IOVar)
 *
 *  This function either sets or retrieves the value of an I/O variable from the Broadcom 802.11 device.
 *  The data which is set or retrieved must be in a format structure which is appropriate for the particular
 *  I/O variable being accessed. These structures can only be found in the DHD source code such as wl/exe/wlu.c.
 *
 *  @Note: The function blocks until the I/O variable read/write has completed
 *
 * @param type       : SDPCM_SET or SDPCM_GET - indicating whether to set or get the I/O variable value
 * @param send_buffer_hnd : A handle for a packet buffer containing the data value to be sent.
 * @param response_buffer_hnd : A pointer which will receive the handle for the packet buffer
 *                              containing the response data value received.
 * @param interface : Which interface to send the iovar to (AP or STA)
 *
 * @return    WHD result code
 */
/*@allocates *response_buffer_hnd@*/  /*@defines **response_buffer_hnd@*/

whd_result_t whd_sdpcm_send_iovar(whd_interface_t ifp, sdpcm_command_type_t type,
                                  /*@only@*/ whd_buffer_t send_buffer_hnd,
                                  /*@special@*/ /*@out@*/ /*@null@*/ whd_buffer_t *response_buffer_hnd)
{
    if (type == SDPCM_SET)
    {
        return whd_sdpcm_send_ioctl(ifp, SDPCM_SET, (uint32_t)WLC_SET_VAR, send_buffer_hnd, response_buffer_hnd);
    }
    else
    {
        return whd_sdpcm_send_ioctl(ifp, SDPCM_GET, (uint32_t)WLC_GET_VAR, send_buffer_hnd, response_buffer_hnd);
    }
}

/** Sends a data packet.
 *
 *  This function should be called by the bottom of the network stack in order for it
 *  to send an ethernet frame.
 *  The function prepends a BDC header, before sending to @ref whd_sdpcm_send_common where
 *  the SDPCM header will be added
 *
 * @param buffer  : The ethernet packet buffer to be sent
 * @param interface : the interface over which to send the packet (AP or STA)
 *
 */
/* Returns immediately - whd_buffer_tx_completed will be called once the transmission has finished */
void whd_network_send_ethernet_data(whd_interface_t ifp, /*@only@*/ whd_buffer_t buffer)
{
    sdpcm_data_header_t *packet;
    whd_result_t result;
    uint8_t *dscp = NULL;
    uint8_t priority = 0;
    uint8_t whd_tos_map[8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
    whd_driver_t whd_driver = ifp->whd_driver;
    sdpcm_ethernet_header_t *ethernet_header = (sdpcm_ethernet_header_t *)whd_buffer_get_current_piece_data_pointer(
                                                                          whd_driver, buffer);
    uint16_t ether_type;

    ether_type = NTOH16(ethernet_header->ethertype);
    if ( (ether_type == WHD_ETHERTYPE_IPv4) || (ether_type == WHD_ETHERTYPE_DOT1AS) )
    {
        dscp = (uint8_t*)whd_buffer_get_current_piece_data_pointer( whd_driver, buffer ) + IPV4_DSCP_OFFSET;
    }

    add_sdpcm_log_entry(LOG_TX, DATA, whd_buffer_get_current_piece_size( whd_driver,
                            buffer), (char *)whd_buffer_get_current_piece_data_pointer( whd_driver, buffer) );

    WPRINT_WHD_DATA_LOG( ("Wcd:> DATA pkt 0x%08lX len %d\n", (unsigned long)buffer,
              (int)whd_buffer_get_current_piece_size( whd_driver, buffer) ) );


    /* Add link space at front of packet */
    result = whd_buffer_add_remove_at_front( whd_driver, &buffer, - (int) (sizeof(sdpcm_data_header_t)) );
    if (result != WHD_SUCCESS)
    {
        WPRINT_WHD_DEBUG( ("Unable to adjust header space\n") );
        result = whd_buffer_release(ifp->whd_driver, buffer, WHD_NETWORK_TX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
        return;
    }

    packet = (sdpcm_data_header_t*) whd_buffer_get_current_piece_data_pointer( whd_driver,buffer );

    if (ifp->bsscfgidx > WHD_INTERFACE_MAX)
    {
        WPRINT_WHD_DEBUG( ("No interface for packet send\n") );
        result = whd_buffer_release(ifp->whd_driver, buffer, WHD_NETWORK_TX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
        return;
    }

    /* Prepare the BDC header */
    packet->bdc_header.flags    = 0;
    packet->bdc_header.flags    = (uint8_t)(BDC_PROTO_VER << BDC_FLAG_VER_SHIFT);
    /* If it's an IPv4 packet set the BDC header priority based on the DSCP field */
    if ( ( (ether_type == WHD_ETHERTYPE_IPv4) || (ether_type == WHD_ETHERTYPE_DOT1AS) ) && (dscp != NULL) )
    {
        if (*dscp != 0) /* If it's equal 0 then it's best effort traffic and nothing needs to be done */
        {
            priority = whd_map_dscp_to_priority( whd_driver, *dscp );
        }
    }

    /* If STA interface, re-map prio to the prio allowed by the AP, regardless of whether it's an IPv4 packet */
    if (ifp->role == WHD_STA_ROLE)
    {
        packet->bdc_header.priority = whd_tos_map[priority];
    }
    else
    {
        packet->bdc_header.priority = priority;
    }

    packet->bdc_header.flags2   = ifp->bsscfgidx;
    packet->bdc_header.data_offset = 0;

    /* Add the length of the BDC header and pass "down" */
    whd_sdpcm_send_common( whd_driver, buffer, DATA_HEADER );
}

void whd_sdpcm_update_credit(whd_driver_t whd_driver, uint8_t *data)
{
    sdpcm_sw_header_t *header = (sdpcm_sw_header_t *)(data + 4);
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;

    if ( (header->channel_and_flags & 0x0f) < (uint8_t)3 )
    {
        sdpcm_info->credit_diff = (uint8_t)(header->bus_data_credit - sdpcm_info->last_bus_data_credit);
        WPRINT_WHD_DATA_LOG(("credit update =%d\n ",header->bus_data_credit) );
        if (sdpcm_info->credit_diff <= GET_C_VAR(whd_driver, BUS_CREDIT_DIFF) )
        {
            sdpcm_info->last_bus_data_credit = header->bus_data_credit;
        }
        else
        {
            if (sdpcm_info->credit_diff > sdpcm_info->largest_credit_diff)
            {
                sdpcm_info->largest_credit_diff = sdpcm_info->credit_diff;
            }
        }
    }

    whd_bus_set_flow_control(whd_driver, header->wireless_flow_control);
}

/** Processes and directs incoming SDPCM packets
 *
 *  This function receives SDPCM packets from the Broadcom 802.11 device and decodes the SDPCM header
 *  to determine where the packet should be directed.
 *
 *  - Control packets (IOCTL/IOVAR) cause the IOCTL flag to be set to allow the resumption of the thread
 *    which sent the IOCTL
 *  - Data Packets are sent to the bottom layer of the network stack via the @ref whd_network_process_ethernet_data function
 *  - Event Packets are decoded to determine which event occurred, and the event handler list is consulted
 *    and the appropriate event handler is called.
 *
 * @param buffer  : The SDPCM packet buffer received from the Broadcom 802.11 device
 *
 */
void whd_sdpcm_process_rx_packet(/*@only@*/ whd_driver_t whd_driver, whd_buffer_t buffer)
{
    sdpcm_common_header_t *packet;
    uint16_t i;
    uint16_t j;
    uint16_t size;
    uint16_t size_inv;
    uint32_t flags;
    uint16_t id;
    sdpcm_common_header_t *common_header;
    sdpcm_cdc_header_t *cdc_header;
    whd_result_t result;

    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;

    packet = (sdpcm_common_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer);

    /* Extract the total SDPCM packet size from the first two frametag bytes */
    size = packet->sdpcm_header.frametag[0];

    /* Check that the second two frametag bytes are the binary inverse of the size */
    size_inv = (uint16_t) ~size;  /* Separate variable due to GCC Bug 38341 */
    if (packet->sdpcm_header.frametag[1] != size_inv)
    {
        WPRINT_WHD_DEBUG( ("Received a packet with a frametag which is wrong\n") );
        result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
        return;
    }

    /* Check whether the packet is big enough to contain the SDPCM header (or) it's too big to handle */
    if ( (size < (uint16_t)SDPCM_HEADER_LEN) || (size > whd_buffer_get_current_piece_size(whd_driver, buffer) ) )
    {
        whd_minor_assert("Packet size invalid", 0 == 1);
        WPRINT_WHD_DEBUG( (
                              "Received a packet that is too small to contain anything useful (or) too big. Packet Size = [%d]\n",
                              size) );
        result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
        return;
    }

    /* Get address of packet->sdpcm_header.frametag indirectly to avoid IAR's unaligned address warning */
    whd_sdpcm_update_credit(whd_driver,
                            (uint8_t *)&packet->sdpcm_header.sw_header - sizeof(packet->sdpcm_header.frametag) );

    if (size == (uint16_t)SDPCM_HEADER_LEN)
    {
        /* This is a flow control update packet with no data - release it. */
        result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );

        return;
    }

    /* Check the SDPCM channel to decide what to do with packet. */
    switch (packet->sdpcm_header.sw_header.channel_and_flags & 0x0f)
    {
        case CONTROL_HEADER:  /* IOCTL/IOVAR reply packet */
            add_sdpcm_log_entry(LOG_RX, IOCTL, whd_buffer_get_current_piece_size(whd_driver, buffer),
                                (char *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer) );

            /* Check that packet size is big enough to contain the CDC header as well as the SDPCM header */
            if (packet->sdpcm_header.frametag[0] <
                (sizeof(packet->sdpcm_header.frametag) + sizeof(sdpcm_sw_header_t) + sizeof(sdpcm_cdc_header_t) ) )
            {
                /* Received a too-short SDPCM packet! */
                WPRINT_WHD_DEBUG( ("Received a too-short SDPCM packet!\n") );
                result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
                if (result != WHD_SUCCESS)
                    WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );

                break;
            }

            /* Check that the IOCTL identifier matches the identifier that was sent */
            common_header = (sdpcm_common_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer);
            cdc_header =
                (sdpcm_cdc_header_t *)&( (char *)&common_header->sdpcm_header )[common_header->sdpcm_header.sw_header.
                                                                                header_length];
            flags         = ltoh32(cdc_header->flags);
            id            = (uint16_t)( (flags & CDCF_IOC_ID_MASK) >> CDCF_IOC_ID_SHIFT );

            if (id == sdpcm_info->requested_ioctl_id)
            {
                /* Save the response packet in a variable */
                sdpcm_info->ioctl_response = buffer;

                WPRINT_WHD_DATA_LOG( ( "Wcd:< Procd pkt 0x%08lX: IOCTL Response (%d bytes)\n", (unsigned long)buffer, size ) );

                /* Wake the thread which sent the IOCTL/IOVAR so that it will resume */
                result = whd_rtos_set_semaphore(&sdpcm_info->ioctl_sleep, WHD_FALSE);
                if (result != WHD_SUCCESS)
                    WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );

            }
            else
            {
                result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
                if (result != WHD_SUCCESS)
                    WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );

                WPRINT_WHD_DEBUG( ("Received a response for a different IOCTL - retry\n") );
            }
            break;

        case DATA_HEADER:
        {
            sdpcm_bdc_header_t *bdc_header;
            int32_t headers_len_below_payload;
            uint32_t ip_data_start_add;
            uint32_t bssid_index;
            whd_interface_t ifp;

            /* Check that the packet is big enough to contain SDPCM & BDC headers */
            if (packet->sdpcm_header.frametag[0] <= (uint16_t)(SDPCM_HEADER_LEN + BDC_HEADER_LEN) )
            {
                WPRINT_WHD_DEBUG( ("Packet too small to contain SDPCM + BDC headers\n") );
                result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
                if (result != WHD_SUCCESS)
                    WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );

                break;
            }

            /* Calculate where the BDC header is - this is dependent on the data offset field of the SDPCM SW header */
            bdc_header =
                (sdpcm_bdc_header_t *)&( (char *)&packet->sdpcm_header )[packet->sdpcm_header.sw_header.header_length];

            /* Calculate where the payload is - this is dependent on the data offset fields of the
             * SDPCM SW header and the BDC header */
            headers_len_below_payload =
                (int32_t)( (int32_t)sizeof(whd_buffer_header_t) +
                           (int32_t)packet->sdpcm_header.sw_header.header_length +
                           (int32_t)BDC_HEADER_LEN + (int32_t)(bdc_header->data_offset << 2) );

            /* Move buffer pointer past gSPI, SDPCM, BCD headers and padding,
             * so that the network stack or 802.11 monitor sees only the payload */
            if (WHD_SUCCESS != whd_buffer_add_remove_at_front(whd_driver, &buffer, headers_len_below_payload) )
            {
                WPRINT_WHD_DEBUG( ("No space for headers without chaining. this should never happen\n") );
                result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
                if (result != WHD_SUCCESS)
                    WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );

                break;
            }

            /* It is preferable to have IP data at address aligned to 4 bytes. IP data startes after ethernet header */
            ip_data_start_add =
                (uint32_t )whd_buffer_get_current_piece_data_pointer(whd_driver, buffer) - WHD_ETHERNET_SIZE;
            if ( ( (ip_data_start_add >> 2) << 2 ) != ip_data_start_add )
            {
                    WPRINT_WHD_DATA_LOG(("IP data not aligned to 4 bytes %lx\n", ip_data_start_add));
            }

            add_sdpcm_log_entry(LOG_RX, DATA, whd_buffer_get_current_piece_size(whd_driver, buffer),
                                (char *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer) );
                WPRINT_WHD_DATA_LOG( ( "Wcd:< Procd pkt 0x%08lX: Data (%d bytes)\n", (unsigned long)buffer, size ) );
            bssid_index = (uint32_t)(bdc_header->flags2 & BDC_FLAG2_IF_MASK);
            ifp = whd_driver->iflist[bssid_index];

            /* Send packet to bottom of network stack */
            result = whd_network_process_ethernet_data(ifp, buffer);
            if (result != WHD_SUCCESS)
                WPRINT_WHD_ERROR( ("%s failed at %d \n", __func__, __LINE__) );

        }
        break;

        case ASYNCEVENT_HEADER:
        {
            sdpcm_bdc_header_t *bdc_header;
            uint16_t ether_type;
            whd_event_header_t *whd_event;
            whd_event_t *event;

            bdc_header =
                (sdpcm_bdc_header_t *)&( (char *)&packet->sdpcm_header )[packet->sdpcm_header.sw_header.header_length];

            event = (whd_event_t *)&bdc_header[bdc_header->data_offset + 1];

            ether_type = NTOH16(event->eth.ethertype);

            /* If frame is truly an event, it should have EtherType equal to the Broadcom type. */
            if (ether_type != (uint16_t)ETHER_TYPE_BRCM)
            {
                WPRINT_WHD_DEBUG( ("Error - received a channel 1 packet which was not BRCM ethertype\n") );
                result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
                if (result != WHD_SUCCESS)
                    WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
                break;
            }

            /* If ethertype is correct, the contents of the ethernet packet
             * are a structure of type bcm_event_t
             */

            /* Check that the OUI matches the Broadcom OUI */
            if (0 != memcmp(BRCM_OUI, &event->eth_evt_hdr.oui[0], (size_t)DOT11_OUI_LEN) )
            {
                WPRINT_WHD_DEBUG( ("Event OUI mismatch\n") );
                result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
                if (result != WHD_SUCCESS)
                    WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
                break;
            }

            whd_event = &event->whd_event;

            /* Search for the event type in the list of event handler functions
             * event data is stored in network endianness
             */
            whd_event->flags      =                        NTOH16(whd_event->flags);
            whd_event->event_type = (whd_event_num_t)NTOH32(whd_event->event_type);
            whd_event->status     = (whd_event_status_t)NTOH32(whd_event->status);
            whd_event->reason     = (whd_event_reason_t)NTOH32(whd_event->reason);
            whd_event->auth_type  =                        NTOH32(whd_event->auth_type);
            whd_event->datalen    =                        NTOH32(whd_event->datalen);
            whd_event->ifidx =  whd_event->ifidx;
            whd_event->bsscfgidx =  whd_event->bsscfgidx;
            /* Ensure data length is correct */
            if (whd_event->datalen >
                (uint32_t)(size - ( (char *)DATA_AFTER_HEADER(event) - (char *)&packet->sdpcm_header ) ) )
            {
                WPRINT_WHD_DEBUG( (
                                      "Error - (data length received [%d] > expected data length [%d]). SDPCM packet size = [%d]. Ignoring the packet\n",
                                      (int)whd_event->datalen,
                                      size - ( (char *)DATA_AFTER_HEADER(event) - (char *)&packet->sdpcm_header ),
                                      size) );
                result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
                if (result != WHD_SUCCESS)
                    WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );

                break;
            }
            //whd_event->interface  = ( event->event.raw.ifidx & 3);

            /* This is necessary because people who defined event statuses and reasons overlapped values. */
            if (whd_event->event_type == WLC_E_PSK_SUP)
            {
                whd_event->status = (whd_event_status_t)( (int)whd_event->status + WLC_SUP_STATUS_OFFSET );
                whd_event->reason = (whd_event_reason_t)( (int)whd_event->reason + WLC_E_SUP_REASON_OFFSET );
            }
            else if (whd_event->event_type == WLC_E_PRUNE)
            {
                whd_event->reason = (whd_event_reason_t)( (int)whd_event->reason + WLC_E_PRUNE_REASON_OFFSET );
            }
            else if ( (whd_event->event_type == WLC_E_DISASSOC) || (whd_event->event_type == WLC_E_DEAUTH) )
            {
                whd_event->status = (whd_event_status_t)( (int)whd_event->status + WLC_DOT11_SC_STATUS_OFFSET );
                whd_event->reason = (whd_event_reason_t)( (int)whd_event->reason + WLC_E_DOT11_RC_REASON_OFFSET );
            }

            /* do any needed debug logging of event */
            whd_wifi_log_event(whd_driver, whd_event, (uint8_t *)DATA_AFTER_HEADER(event) );

            if (whd_rtos_get_semaphore(&sdpcm_info->event_list_mutex, NEVER_TIMEOUT, WHD_FALSE) != WHD_SUCCESS)
            {
                WPRINT_WHD_DEBUG( ("Failed to obtain mutex for event list access!\n") );
                result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
                if (result != WHD_SUCCESS)
                    WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
                break;
            }

            for (i = 0; i < (uint16_t)WHD_EVENT_HANDLER_LIST_SIZE; i++)
            {
                if (sdpcm_info->whd_sdpcm_event_list[i].events != NULL)
                {
                    for (j = 0; sdpcm_info->whd_sdpcm_event_list[i].events[j] != WLC_E_NONE; ++j)
                    {
                        if ( (sdpcm_info->whd_sdpcm_event_list[i].events[j] == whd_event->event_type) &&
                             (sdpcm_info->whd_sdpcm_event_list[i].ifidx == whd_event->ifidx) )
                        {
                            /* Correct event type has been found - call the handler function and exit loop */
                            sdpcm_info->whd_sdpcm_event_list[i].handler_user_data =
                                sdpcm_info->whd_sdpcm_event_list[i].handler(whd_driver->iflist[whd_event->bsscfgidx],
                                                                            whd_event,
                                                                            (uint8_t *)DATA_AFTER_HEADER(
                                                                                event),
                                                                            sdpcm_info->whd_sdpcm_event_list[i].handler_user_data);
                            /*@innerbreak@*/
                            break;
                        }
                    }
                }
            }

            result = whd_rtos_set_semaphore(&sdpcm_info->event_list_mutex, WHD_FALSE);
            if (result != WHD_SUCCESS)
                WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );


            add_sdpcm_log_entry(LOG_RX, EVENT, whd_buffer_get_current_piece_size(whd_driver, buffer),
                                (char *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer) );
            WPRINT_WHD_DATA_LOG( ("Wcd:< Procd pkt 0x%08lX: Evnt %d (%d bytes)\n", (unsigned long)buffer,
                      (int)whd_event->event_type, size) );

            /* Release the event packet buffer */
            result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
            if (result != WHD_SUCCESS)
                WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
        }
        break;

        default:
            whd_minor_assert("SDPCM packet of unknown channel received - dropping packet", 0 != 0);
            result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
            if (result != WHD_SUCCESS)
                WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
            break;
    }
}

/** Sends an IOCTL command
 *
 *  Sends a I/O Control command to the Broadcom 802.11 device.
 *  The data which is set or retrieved must be in a format structure which is appropriate for the particular
 *  I/O control being sent. These structures can only be found in the DHD source code such as wl/exe/wlu.c.
 *  The I/O control will always respond with a packet buffer which may contain data in a format specific to
 *  the I/O control being used.
 *
 *  @Note: The caller is responsible for releasing the response buffer.
 *  @Note: The function blocks until the IOCTL has completed
 *  @Note: Only one IOCTL may happen simultaneously.
 *
 *  @param type       : SDPCM_SET or SDPCM_GET - indicating whether to set or get the I/O control
 *  @param send_buffer_hnd : A handle for a packet buffer containing the data value to be sent.
 *  @param response_buffer_hnd : A pointer which will receive the handle for the packet buffer
 *                               containing the response data value received.
 *  @param interface : Which interface to send the iovar to (WHD_STA_INTERFACE or WHD_AP_INTERFACE)
 *
 *  @return    WHD result code
 */
whd_result_t whd_sdpcm_send_ioctl(whd_interface_t ifp, sdpcm_command_type_t type, uint32_t command,
                                  /*@only@*/ whd_buffer_t send_buffer_hnd,
                                  /*@special@*/ /*@out@*/ /*@null@*/ whd_buffer_t *response_buffer_hnd)                                                                                                                /*@allocates *response_buffer_hnd@*/  /*@defines **response_buffer_hnd@*/
{
    uint32_t data_length;
    uint32_t flags;
    whd_result_t retval;
    sdpcm_control_header_t *send_packet;
    sdpcm_common_header_t *common_header;
    sdpcm_cdc_header_t *cdc_header;
    uint32_t bss_index = ifp->bsscfgidx;
    whd_driver_t whd_driver = ifp->whd_driver;
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;

    /* Acquire mutex which prevents multiple simultaneous IOCTLs */
    retval = whd_rtos_get_semaphore(&sdpcm_info->ioctl_mutex, NEVER_TIMEOUT, WHD_FALSE);
    if (retval != WHD_SUCCESS)
    {
        CHECK_RETURN(whd_buffer_release(whd_driver, send_buffer_hnd, WHD_NETWORK_TX) );
        return retval;
    }

    /* Get the data length and cast packet to a CDC SDPCM header */
    data_length =
        (uint32_t)(whd_buffer_get_current_piece_size(whd_driver,
                                                     send_buffer_hnd) - sizeof(sdpcm_common_header_t) -
                   sizeof(sdpcm_cdc_header_t) );
    send_packet = (sdpcm_control_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, send_buffer_hnd);

    WHD_IOCTL_LOG_ADD(ifp->whd_driver, command, send_buffer_hnd);

    /* Check if IOCTL is actually IOVAR */
    if ( (command == WLC_SET_VAR) || (command == WLC_GET_VAR) )
    {
        uint8_t *data = (uint8_t *)DATA_AFTER_HEADER(send_packet);
        uint8_t *ptr = data;

        /* Calculate the offset added to compensate for IOVAR string creating unaligned data section */
        while (*ptr == 0)
        {
            ptr++;
        }
        if (data != ptr)
        {
            data_length -= (uint32_t)(ptr - data);
            memmove(data, ptr, data_length);
            CHECK_RETURN(whd_buffer_set_size(whd_driver, send_buffer_hnd,
                                             (uint16_t)(data_length + sizeof(sdpcm_common_header_t) +
                                                        sizeof(sdpcm_cdc_header_t) ) ) );
        }
    }

    /* Prepare the CDC header */
    send_packet->cdc_header.cmd    = command;
    send_packet->cdc_header.len    = data_length;
    send_packet->cdc_header.flags =
        ( ( (uint32_t)++ (sdpcm_info->requested_ioctl_id) << CDCF_IOC_ID_SHIFT ) & CDCF_IOC_ID_MASK ) | type |
        bss_index <<
            CDCF_IOC_IF_SHIFT;
    send_packet->cdc_header.status = 0;

    /* Manufacturing test can receive big buffers, but sending big buffers causes a wlan firmware error */
    /* Even though data portion needs to be truncated, cdc_header should have the actual length of the ioctl packet */
    if (whd_buffer_get_current_piece_size(whd_driver, send_buffer_hnd) > WHD_IOCTL_MAX_TX_PKT_LEN)
    {
        CHECK_RETURN(whd_buffer_set_size(whd_driver, send_buffer_hnd, WHD_IOCTL_MAX_TX_PKT_LEN) );
    }

    /* Store the length of the data and the IO control header and pass "down" */
    whd_sdpcm_send_common(whd_driver, send_buffer_hnd, CONTROL_HEADER);

    /* Wait till response has been received  */
    retval = whd_rtos_get_semaphore(&sdpcm_info->ioctl_sleep, (uint32_t)WHD_IOCTL_TIMEOUT_MS, WHD_FALSE);
    if (retval != WHD_SUCCESS)
    {
        /* Release the mutex since whd_sdpcm_ioctl_response will no longer be referenced. */
        CHECK_RETURN(whd_rtos_set_semaphore(&sdpcm_info->ioctl_mutex, WHD_FALSE) );
        return retval;
    }

    common_header = (sdpcm_common_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver,
                                                                                       sdpcm_info->ioctl_response);
    cdc_header =
        (sdpcm_cdc_header_t *)&( (char *)&common_header->sdpcm_header )[common_header->sdpcm_header.sw_header.
                                                                        header_length];
    flags         = ltoh32(cdc_header->flags);

    retval = (whd_result_t)(WLAN_ENUM_OFFSET -  ltoh32(cdc_header->status) );

    /* Check if the caller wants the response */
    if (response_buffer_hnd != NULL)
    {
        *response_buffer_hnd = sdpcm_info->ioctl_response;
        CHECK_RETURN(whd_buffer_add_remove_at_front(whd_driver, response_buffer_hnd,
                                                    (int32_t)(sizeof(whd_buffer_header_t) + sizeof(sdpcm_cdc_header_t) +
                                                              common_header->sdpcm_header.sw_header.header_length) ) );
    }
    else
    {
        CHECK_RETURN(whd_buffer_release(whd_driver, sdpcm_info->ioctl_response, WHD_NETWORK_RX) );
    }

    sdpcm_info->ioctl_response = NULL;

    /* Release the mutex since whd_sdpcm_ioctl_response will no longer be referenced. */
    CHECK_RETURN(whd_rtos_set_semaphore(&sdpcm_info->ioctl_mutex, WHD_FALSE) );

    /* Check whether the IOCTL response indicates it failed. */
    if ( (flags & CDCF_IOC_ERROR) != 0 )
    {
        if (response_buffer_hnd != NULL)
        {
            CHECK_RETURN(whd_buffer_release(whd_driver, *response_buffer_hnd, WHD_NETWORK_RX) );
            *response_buffer_hnd = NULL;
        }
        whd_minor_assert("IOCTL failed\n", 0 != 0);
        return retval;
    }

    return WHD_SUCCESS;
}

/**
 * Registers locally a handler to receive event callbacks.
 * Does not notify Wi-Fi about event subscription change.
 * Can be used to refresh local callbacks (e.g. after deep-sleep)
 * if Wi-Fi is already notified about them.
 *
 * This function registers a callback handler to be notified when
 * a particular event is received.
 *
 * Alternately the function clears callbacks for given event type.
 *
 * @note : Currently each event may only be registered to one handler
 *         and there is a limit to the number of simultaneously registered
 *         events
 *
 * @param  event_nums     An array of event types that is to trigger the handler. The array must be terminated with a WLC_E_NONE event
 *                        See @ref whd_event_num_t for available events
 * @param handler_func   A function pointer to the new handler callback,
 *                        or NULL if callbacks are to be disabled for the given event type
 * @param handler_user_data  A pointer value which will be passed to the event handler function
 *                            at the time an event is triggered (NULL is allowed)
 * @param interface      The interface to set the handler for.
 *
 * @return WHD result code
 */
whd_result_t whd_management_set_event_handler_locally(whd_interface_t ifp, /*@keep@*/ const whd_event_num_t *event_nums,
                                                      /*@null@*/ whd_event_handler_t handler_func,
                                                      /*@null@*/ /*@keep@*/ void *handler_user_data)
{
    uint16_t entry = (uint16_t)0xFF;
    uint16_t i;
    whd_driver_t whd_driver = ifp->whd_driver;
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;

    /* Find an existing matching entry OR the next empty entry */
    for (i = 0; i < (uint16_t)WHD_EVENT_HANDLER_LIST_SIZE; i++)
    {
        /* Find a matching event list OR the first empty event entry */
        if (sdpcm_info->whd_sdpcm_event_list[i].events == event_nums)
        {
            /* Check if all the data already matches */
            if ( (sdpcm_info->whd_sdpcm_event_list[i].handler           == handler_func) &&
                 (sdpcm_info->whd_sdpcm_event_list[i].handler_user_data == handler_user_data) &&
                 (sdpcm_info->whd_sdpcm_event_list[i].ifidx == ifp->ifidx) )
            {
                return WHD_SUCCESS;
            }

            /* Delete the entry */
            sdpcm_info->whd_sdpcm_event_list[i].events = NULL;
            sdpcm_info->whd_sdpcm_event_list[i].handler = NULL;
            sdpcm_info->whd_sdpcm_event_list[i].handler_user_data = NULL;

            entry = i;
            break;
        }
        else if ( (entry == (uint16_t)0xFF) && (sdpcm_info->whd_sdpcm_event_list[i].events == NULL) )
        {
            entry = i;
        }
    }

    /* Check if handler function was provided */
    if (handler_func != NULL)
    {
        /* Check if an empty entry was not found */
        if (entry == (uint16_t)0xFF)
        {
            WPRINT_WHD_DEBUG( ("Out of space in event handlers table - try increasing WHD_EVENT_HANDLER_LIST_SIZE\n") );
            return WHD_OUT_OF_EVENT_HANDLER_SPACE;
        }

        /* Add the new handler in at the free space */
        sdpcm_info->whd_sdpcm_event_list[entry].handler           = handler_func;
        sdpcm_info->whd_sdpcm_event_list[entry].handler_user_data = handler_user_data;
        sdpcm_info->whd_sdpcm_event_list[entry].events            = event_nums;
        sdpcm_info->whd_sdpcm_event_list[entry].ifidx             = ifp->ifidx;
    }

    return WHD_SUCCESS;
}

/* allocates memory for the needed iovar and returns a pointer to the event mask */
static uint8_t *whd_management_alloc_event_msgs_buffer(whd_interface_t ifp, whd_buffer_t *buffer)
{
    uint16_t i;
    uint16_t j;
    whd_bool_t use_extended_evt       = WHD_FALSE;
    uint32_t max_event                  = 0;
    eventmsgs_ext_t *eventmsgs_ext_data = NULL;
    uint32_t *data                      = NULL;
    whd_driver_t whd_driver = ifp->whd_driver;
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;

    /* Check to see if event that's set requires more than 128 bit */
    for (i = 0; i < (uint16_t)WHD_EVENT_HANDLER_LIST_SIZE; i++)
    {
        if (sdpcm_info->whd_sdpcm_event_list[i].events != NULL)
        {
            for (j = 0; sdpcm_info->whd_sdpcm_event_list[i].events[j] != WLC_E_NONE; j++)
            {
                uint32_t event_value = sdpcm_info->whd_sdpcm_event_list[i].events[j];
                if (event_value > 127)
                {
                    use_extended_evt = WHD_TRUE;
                    if (event_value > max_event)
                    {
                        max_event = event_value;
                    }
                    /* keep going to get highest value */
                }
            }
        }
    }

    if (WHD_FALSE == use_extended_evt)
    {
        /* use old iovar for backwards compat */
        data = (uint32_t *)whd_sdpcm_get_iovar_buffer(whd_driver, buffer, (uint16_t)WL_EVENTING_MASK_LEN + 4,
                                                      "bsscfg:" IOVAR_STR_EVENT_MSGS);

        if (NULL == data)
        {
            return NULL;
        }

        data[0] = ifp->bsscfgidx;

        return (uint8_t *)&data[1];
    }
    else
    {
        uint8_t mask_len   = (uint8_t)( (max_event + 8) / 8 );
        data =
            (uint32_t *)whd_sdpcm_get_iovar_buffer(whd_driver, buffer,
                                                   (uint16_t)(sizeof(eventmsgs_ext_t) + mask_len + 4),
                                                   "bsscfg:" IOVAR_STR_EVENT_MSGS_EXT);

        if (NULL == data)
        {
            return NULL;
        }

        data[0] = ifp->bsscfgidx;

        eventmsgs_ext_data = (eventmsgs_ext_t *)&data[1];

        memset(eventmsgs_ext_data, 0, sizeof(*eventmsgs_ext_data) );
        eventmsgs_ext_data->ver     = EVENTMSGS_VER;
        eventmsgs_ext_data->command = EVENTMSGS_SET_MASK;
        eventmsgs_ext_data->len     = mask_len;
        return eventmsgs_ext_data->mask;
    }
}

/**
 * Registers a handler to receive event callbacks.
 * Subscribe locally and notify Wi-Fi about subscription.
 *
 * This function registers a callback handler to be notified when
 * a particular event is received.
 *
 * Alternately the function clears callbacks for given event type.
 *
 * @note : Currently each event may only be registered to one handler
 *         and there is a limit to the number of simultaneously registered
 *         events
 *
 * @param  event_nums     An array of event types that is to trigger the handler.
 *                        The array must be terminated with a WLC_E_NONE event
 *                        See @ref whd_event_num_t for available events
 * @param handler_func   A function pointer to the new handler callback,
 *                        or NULL if callbacks are to be disabled for the given event type
 * @param handler_user_data  A pointer value which will be passed to the event handler function
 *                            at the time an event is triggered (NULL is allowed)
 * @param interface      The interface to set the handler for.
 *
 * @return WHD result code
 */
whd_result_t whd_management_set_event_handler(/*@keep@*/ whd_interface_t ifp, const whd_event_num_t *event_nums,
                                                         /*@null@*/ whd_event_handler_t handler_func,
                                                         /*@null@*/ /*@keep@*/ void *handler_user_data)
{
    whd_buffer_t buffer;
    uint8_t *event_mask;
    uint16_t i;
    uint16_t j;
    whd_result_t res;
    whd_driver_t whd_driver = ifp->whd_driver;
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;
    whd_interface_t prim_ifp = whd_get_primary_interface(whd_driver);

    if (prim_ifp == NULL)
    {
        return WHD_UNKNOWN_INTERFACE;
    }

    /* Acquire mutex preventing multiple threads accessing the handler at the same time */
    res = whd_rtos_get_semaphore(&sdpcm_info->event_list_mutex, NEVER_TIMEOUT, WHD_FALSE);
    if (res != WHD_SUCCESS)
    {
        return res;
    }

    /* Set event handler locally  */
    res = whd_management_set_event_handler_locally(ifp, event_nums, handler_func, handler_user_data);
    if (res != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Error in setting event handler locally, %s failed at %d \n", __func__, __LINE__) );
        goto set_event_handler_exit;
    }

    /* Send the new event mask value to the wifi chip */
    event_mask = whd_management_alloc_event_msgs_buffer(ifp, &buffer);

    if (NULL == event_mask)
    {
        res = WHD_BUFFER_UNAVAILABLE_PERMANENT;
        WPRINT_WHD_ERROR( ("Buffer unavailable permanently, %s failed at %d \n", __func__, __LINE__) );
        goto set_event_handler_exit;
    }

    /* Keep the wlan awake while we set the event_msgs */
    WHD_WLAN_KEEP_AWAKE(whd_driver);

    /* Set the event bits for each event from every handler */
    memset(event_mask, 0, (size_t)WL_EVENTING_MASK_LEN);
    for (i = 0; i < (uint16_t)WHD_EVENT_HANDLER_LIST_SIZE; i++)
    {
        if (sdpcm_info->whd_sdpcm_event_list[i].events != NULL)
        {
            for (j = 0; sdpcm_info->whd_sdpcm_event_list[i].events[j] != WLC_E_NONE; j++)
            {
                setbit(event_mask, sdpcm_info->whd_sdpcm_event_list[i].events[j]);
            }
        }
    }

    /* set the event_list_mutex from calling thread before sending iovar
     * as the RX thread also waits on this Mutex when an ASYNC Event received
     * causing deadlock
     */
    CHECK_RETURN(whd_rtos_set_semaphore(&sdpcm_info->event_list_mutex, WHD_FALSE) );

    CHECK_RETURN(whd_sdpcm_send_iovar(prim_ifp, SDPCM_SET, buffer, 0) );

    /* The wlan chip can sleep from now on */
    WHD_WLAN_LET_SLEEP(whd_driver);
    return WHD_SUCCESS;

set_event_handler_exit:
    CHECK_RETURN(whd_rtos_set_semaphore(&sdpcm_info->event_list_mutex, WHD_FALSE) );
    return res;
}

/** A helper function to easily acquire and initialise a buffer destined for use as an iovar
 *
 * @param  buffer      : A pointer to a whd_buffer_t object where the created buffer will be stored
 * @param  data_length : The length of space reserved for user data
 * @param  name        : The name of the iovar
 *
 * @return A pointer to the start of user data with data_length space available
 */
/*@null@*/ /*@exposed@*/ void *whd_sdpcm_get_iovar_buffer(/*@special@*/ /*@out@*/ whd_driver_t whd_driver,
                                                                                  whd_buffer_t *buffer,
                                                                                  uint16_t data_length,
                                                                                  const char *name)                                                                         /*@allocates *buffer@*/ /*@defines **buffer@*/
{
    uint32_t name_length = (uint32_t)strlen(name) + 1;    /* + 1 for terminating null */
    uint32_t name_length_alignment_offset = (64 - name_length) % sizeof(uint32_t);

    if (whd_host_buffer_get(whd_driver, buffer, WHD_NETWORK_TX,
                            (unsigned short)(IOCTL_OFFSET + data_length + name_length + name_length_alignment_offset),
                            (unsigned long)WHD_IOCTL_PACKET_TIMEOUT) == WHD_SUCCESS)
    {
        uint8_t *data = (whd_buffer_get_current_piece_data_pointer(whd_driver, *buffer) + IOCTL_OFFSET);
        memset(data, 0, name_length_alignment_offset);
        memcpy(data + name_length_alignment_offset, name, name_length);
        return (data + name_length + name_length_alignment_offset);
    }
    else
    {
        WPRINT_WHD_DEBUG( ("Error - failed to allocate a packet buffer for IOVAR\n") );
        return NULL;
    }
}

/** A helper function to easily acquire and initialise a buffer destined for use as an ioctl
 *
 * @param  buffer      : A pointer to a whd_buffer_t object where the created buffer will be stored
 * @param  data_length : The length of space reserved for user data
 *
 * @return A pointer to the start of user data with data_length space available
 */
/*@null@*/ /*@exposed@*/ void *whd_sdpcm_get_ioctl_buffer(/*@special@*/ /*@out@*/ whd_driver_t whd_driver,
                                                                                  whd_buffer_t *buffer,
                                                                                  uint16_t data_length)                                                  /*@allocates *buffer@*/  /*@defines **buffer@*/
{
    if (whd_host_buffer_get(whd_driver, buffer, WHD_NETWORK_TX, (unsigned short)(IOCTL_OFFSET + data_length),
                            (unsigned long)WHD_IOCTL_PACKET_TIMEOUT) == WHD_SUCCESS)
    {
        return (whd_buffer_get_current_piece_data_pointer(whd_driver, *buffer) + IOCTL_OFFSET);
    }
    else
    {
        WPRINT_WHD_DEBUG( ("Error - failed to allocate a packet buffer for IOCTL\n") );
        return NULL;
    }
}

whd_bool_t whd_sdpcm_has_tx_packet(whd_driver_t whd_driver)
{
    if (whd_driver->sdpcm_info.send_queue_head != NULL)
    {
        return WHD_TRUE;
    }

    return WHD_FALSE;
}

whd_result_t whd_sdpcm_get_packet_to_send(whd_driver_t whd_driver, /*@special@*/ /*@out@*/ whd_buffer_t *buffer)
{
    sdpcm_common_header_t *packet;
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;
    whd_result_t result;

    if (sdpcm_info->send_queue_head != NULL)
    {
        /* Check if we're being flow controlled */
        if (whd_bus_is_flow_controlled(whd_driver) == WHD_TRUE)
        {
            WHD_STATS_INCREMENT_VARIABLE(whd_driver, flow_control );
            return WHD_FLOW_CONTROLLED;
        }

        /* Check if we have enough bus data credits spare */
        if (sdpcm_info->packet_transmit_sequence_number == sdpcm_info->last_bus_data_credit)
        {
            WHD_STATS_INCREMENT_VARIABLE(whd_driver, no_credit);
            WPRINT_WHD_ERROR( ("Packet retrieval cancelled due to lack of bus credits , %s failed at %d \n", __func__,
                               __LINE__) );
            return WHD_NO_CREDITS;
        }

        /* There is a packet waiting to be sent - send it then fix up queue and release packet */
        if (whd_rtos_get_semaphore(&sdpcm_info->send_queue_mutex, NEVER_TIMEOUT, WHD_FALSE) != WHD_SUCCESS)
        {
            /* Could not obtain mutex, push back the flow control semaphore */
            WPRINT_WHD_ERROR( ("Error manipulating a semaphore, %s failed at %d \n", __func__, __LINE__) );
            return WHD_SEMAPHORE_ERROR;
        }

        /* Pop the head off and set the new send_queue head */
        *buffer = sdpcm_info->send_queue_head;
        sdpcm_info->send_queue_head = whd_sdpcm_get_next_buffer_in_queue(whd_driver, *buffer);
        if (sdpcm_info->send_queue_head == NULL)
        {
            sdpcm_info->send_queue_tail = NULL;
        }
        result = whd_rtos_set_semaphore(&sdpcm_info->send_queue_mutex, WHD_FALSE);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );


        /* Set the sequence number */
        packet = (sdpcm_common_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, *buffer);
        packet->sdpcm_header.sw_header.sequence = sdpcm_info->packet_transmit_sequence_number;
        sdpcm_info->packet_transmit_sequence_number++;

        return WHD_SUCCESS;
    }
    else
    {
        return WHD_NO_PACKET_TO_SEND;
    }
}

/** Returns the number of bus credits available
 *
 * @return The number of bus credits available
 */
uint8_t whd_sdpcm_get_available_credits(whd_driver_t whd_driver)
{
    return (uint8_t)(whd_driver->sdpcm_info.last_bus_data_credit -
                     whd_driver->sdpcm_info.packet_transmit_sequence_number);
}

/******************************************************
*             Static Functions
******************************************************/

/** Writes SDPCM headers and sends packet to WHD Thread
 *
 *  Prepends the given packet with a new SDPCM header,
 *  then passes the packet to the WHD thread via a queue
 *
 *  This function is called by @ref whd_network_send_ethernet_data and @ref whd_sdpcm_send_ioctl
 *
 *  @param buffer     : The handle of the packet buffer to send
 *  @param header_type  : DATA_HEADER, ASYNCEVENT_HEADER or CONTROL_HEADER - indicating what type of SDPCM packet this is.
 */

static void whd_sdpcm_send_common(whd_driver_t whd_driver, /*@only@*/ whd_buffer_t buffer,
                                  sdpcm_header_type_t header_type)
{
    uint16_t size;
    sdpcm_common_header_t *packet =
        (sdpcm_common_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer);
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;
    whd_result_t result;

    size = whd_buffer_get_current_piece_size(whd_driver, buffer);

    size = (uint16_t)(size - (uint16_t)sizeof(whd_buffer_header_t) );

    /* Prepare the SDPCM header */
    memset( (uint8_t *)&packet->sdpcm_header, 0, sizeof(sdpcm_header_t) );
    packet->sdpcm_header.sw_header.channel_and_flags = (uint8_t)header_type;
    packet->sdpcm_header.sw_header.header_length =
        (header_type == DATA_HEADER) ? sizeof(sdpcm_header_t) + 2 : sizeof(sdpcm_header_t);
    packet->sdpcm_header.sw_header.sequence = 0; /* Note: The real sequence will be written later */
    packet->sdpcm_header.frametag[0] = size;
    packet->sdpcm_header.frametag[1] = (uint16_t) ~size;

    add_sdpcm_log_entry(LOG_TX, (header_type == DATA_HEADER) ? DATA : (header_type == CONTROL_HEADER) ? IOCTL : EVENT,
                        whd_buffer_get_current_piece_size(whd_driver, buffer),
                        (char *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer) );

    /* Add the length of the SDPCM header and pass "down" */
    if (whd_rtos_get_semaphore(&sdpcm_info->send_queue_mutex, NEVER_TIMEOUT, WHD_FALSE) != WHD_SUCCESS)
    {
        /* Could not obtain mutex */
        /* Fatal error */
        result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_TX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) )
            return;
    }

    whd_sdpcm_set_next_buffer_in_queue(whd_driver, NULL, buffer);
    if (sdpcm_info->send_queue_tail != NULL)
    {
        whd_sdpcm_set_next_buffer_in_queue(whd_driver, buffer, sdpcm_info->send_queue_tail);
    }
    sdpcm_info->send_queue_tail = buffer;
    if (sdpcm_info->send_queue_head == NULL)
    {
        sdpcm_info->send_queue_head = buffer;
    }
    result = whd_rtos_set_semaphore(&sdpcm_info->send_queue_mutex, WHD_FALSE);
    if (result != WHD_SUCCESS)
        WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );

    whd_thread_notify(whd_driver);
}

static whd_buffer_t whd_sdpcm_get_next_buffer_in_queue(whd_driver_t whd_driver, whd_buffer_t buffer)
{
    whd_buffer_header_t *packet = (whd_buffer_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer);
    return packet->queue_next;
}

/** Sets the next buffer in the send queue
 *
 *  The send queue is a linked list of packet buffers where the 'next' pointer
 *  is stored in the first 4 bytes of the buffer content.
 *  This function sets that pointer.
 *
 * @param buffer       : handle of packet in the send queue
 *        prev_buffer  : handle of new packet whose 'next' pointer will point to 'buffer'
 */
static void whd_sdpcm_set_next_buffer_in_queue(whd_driver_t whd_driver, whd_buffer_t buffer, whd_buffer_t prev_buffer)
{
    whd_buffer_header_t *packet =
        (whd_buffer_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, prev_buffer);
    packet->queue_next = buffer;
}

/** Map a DSCP value from an IP header to a WMM QoS priority
 *
 * @param dscp_val : DSCP value from IP header
 *
 * @return wmm_qos : WMM priority
 *
 */
static const uint8_t dscp_to_wmm_qos[] =
{ 0, 0, 0, 0, 0, 0, 0, 0,                                       /* 0  - 7 */
  1, 1, 1, 1, 1, 1, 1,                                          /* 8  - 14 */
  1, 1, 1, 1, 1, 1, 1,                                          /* 15 - 21 */
  1, 1, 0, 0, 0, 0, 0,                                          /* 22 - 28 */
  0, 0, 0, 5, 5, 5, 5,                                          /* 29 - 35 */
  5, 5, 5, 5, 5, 5, 5,                                          /* 36 - 42 */
  5, 5, 5, 5, 5, 7, 7,                                          /* 43 - 49 */
  7, 7, 7, 7, 7, 7, 7,                                          /* 50 - 56 */
  7, 7, 7, 7, 7, 7, 7,                                          /* 57 - 63 */
};

static uint8_t whd_map_dscp_to_priority(whd_driver_t whd_driver, uint8_t val)
{
    uint8_t dscp_val = (uint8_t)(val >> 2); /* DSCP field is the high 6 bits of the second byte of an IPv4 header */

    return dscp_to_wmm_qos[dscp_val];
}

