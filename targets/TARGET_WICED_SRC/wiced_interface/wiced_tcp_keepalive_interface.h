/*
 * Copyright 2018, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
 *
 * This software, associated documentation and materials ("Software"),
 * is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

#ifndef __TCP_KEEPALIVE_H__
#define _TCP_KEEPALIVE_H__

#include "mbed.h"
#include "lwip/etharp.h"
#include "WicedInterface.h"
#include "LWIPStack.h"
#include "wiced_tcp_keepalive.h"

#define TCP_KEEPALIVE_ERROR   (-1)
#define TCP_KEEPALIVE_TIMEOUT (-2)
#define TCP_KEEPALIVE_SUCCESS (0)

typedef enum{
    TCP_KEEPALIVE_OFFLOAD_DISABLED,
    TCP_KEEPALIVE_OFFLOAD_SUSPENDED,
    TCP_KEEPALIVE_OFFLOAD_ACTIVE
} tcp_keepalive_offload_connection_state_t;

typedef struct tcp_keepalive_offload_configuration
{
    WiFiInterface *wifi_interface;
	TCPSocket *socket; /* TCP Socket */
	uint16_t keepalive_interval;
	uint16_t keepalive_retry_count;
	uint16_t keepalive_retry_interval;
	uint16_t remote_port;
	uint16_t local_port;
} tcp_keepalive_offload_config_t;

typedef struct tcp_keepalive_offload_information
{
    tcp_keepalive_offload_config_t  tcp_keepalive_offload_config;
    tcp_keepalive_offload_connection_state_t tcp_keepalive_offload_state;
    tcp_keepalive_offload_internal_data_t tcp_keepalive_offload_internal_data;
    struct tcp_pcb *socket_tcb_pcb;
} tcp_keepalive_offload_info_t;

int tcp_keepalive_offload_enable(tcp_keepalive_offload_info_t *tcp_keepalive_offload_info);
int tcp_keepalive_offload_disable(tcp_keepalive_offload_info_t *tcp_keepalive_offload_info);
nsapi_error_t get_tcp_socket_parameters( TCPSocket *socket, void **tcpsock_info );

#endif /* _TCP_KEEPALIVE_H__*/
