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

#include "wiced_tcp_keepalive_interface.h"
#include "cy_syspm.h"
#include <assert.h>

static const wwd_event_num_t tko_events[]   = { WLC_E_TKO, WLC_E_NONE };
void* wwd_callback_handler ( const wwd_event_header_t* event_header, const uint8_t* event_data, /*@null@*/ void* handler_user_data );
static void tcp_keepalive_offload_resume(tcp_keepalive_offload_info_t *tcp_keepalive_info);
static int tcp_keepalive_offload_register_socket_callbacks(tcp_keepalive_offload_info_t *tcp_keepalive_info);
static int tcp_keepalive_offload_deregister_socket_callbacks(tcp_keepalive_offload_info_t *tcp_keepalive_info);

LowPowerTicker lp_timer;
EventQueue queue;
Thread t;
Mutex tcp_ka_offload_mutex;

int tcp_keepalive_offload_enable(tcp_keepalive_offload_info_t *tcp_keepalive_offload_info)
{
    nsapi_error_t response;
    struct eth_addr *dest_mac_addr = NULL;
    struct eth_addr src_mac_addr;
    ip4_addr_t host_ip_addr;
    ip4_addr_t remote_ip_addr;
    const ip4_addr_t *dest_ip_addr = NULL;
    tcp_keepalive_offload_internal_data_t *tcp_keepalive_offload_internal = NULL;
    tcp_keepalive_offload_config_t *tcp_keepalive_config = NULL;
    TCPSocket *socket = NULL;
    SocketAddress peer_addr;
    struct tcp_pcb *tcp_sock_info = NULL;
    wwd_event_handler_t handler = wwd_callback_handler;
    WiFiInterface *wifi = NULL;
    static bool tcp_keepalive_offload_enabled = false;
    int result = TCP_KEEPALIVE_ERROR;

    /* Error Checking */
    if (tcp_keepalive_offload_info == NULL) {
        TCP_KA_ERROR_PRINTF(("KeepAlive Instance not created\n"));
        return TCP_KEEPALIVE_ERROR;
    }

    tcp_ka_offload_mutex.lock();
    if( true == tcp_keepalive_offload_enabled) {
        TCP_KA_ERROR_PRINTF(("TCP Keepalive offload already enabled, call tcp_keepalive_offload_disable() before re-enabling \n"));
        goto tcp_keepalive_offload_enable_unlock_mutex_return;
    }

    tcp_keepalive_offload_internal = &(tcp_keepalive_offload_info->tcp_keepalive_offload_internal_data);
    tcp_keepalive_config           = &(tcp_keepalive_offload_info->tcp_keepalive_offload_config);
    wifi                           = tcp_keepalive_config->wifi_interface;

    if (tcp_keepalive_config->socket == NULL) {
        TCP_KA_ERROR_PRINTF(("Socket not created\n"));
        goto tcp_keepalive_offload_enable_unlock_mutex_return;
    }

    /* Get Local MAC address */
    TCP_KA_DEBUG_PRINTF(("Local mac address is %s\n", wifi->get_mac_address()));

    sscanf( wifi->get_mac_address(), "%02x:%02x:%02x:%02x:%02x:%02x%*c",
        &src_mac_addr.addr[0], &src_mac_addr.addr[1], &src_mac_addr.addr[2],
		&src_mac_addr.addr[3], &src_mac_addr.addr[4], &src_mac_addr.addr[5]);

    /* Get local IP address */
    result = ipaddr_aton(wifi->get_ip_address(), &host_ip_addr);
    if (1 != result) {
        TCP_KA_ERROR_PRINTF(("ipaddr_aton() failed result = %d\n", result));
        result = TCP_KEEPALIVE_ERROR;
        goto tcp_keepalive_offload_enable_unlock_mutex_return;
    }

    /* Get remote IP address */
    socket = tcp_keepalive_config->socket;
    response = socket->getpeername(&peer_addr);
    if (0 != response) {
        TCP_KA_ERROR_PRINTF(("socket.getpeeranme failed result = %d\n", response));
        result = TCP_KEEPALIVE_ERROR;
        goto tcp_keepalive_offload_enable_unlock_mutex_return;
    }

    TCP_KA_DEBUG_PRINTF(("Destination ip address is %s\n", peer_addr.get_ip_address()));
    result = ipaddr_aton(peer_addr.get_ip_address(), &remote_ip_addr);
    if (1 != result )
    {
        TCP_KA_ERROR_PRINTF(("ipaddr_aton() failed result = %d\n", result));
        goto tcp_keepalive_offload_enable_unlock_mutex_return;
    }

    result = etharp_find_addr(NULL, &remote_ip_addr, &dest_mac_addr, &dest_ip_addr);
    if (TCP_KEEPALIVE_ERROR == result) {
        TCP_KA_ERROR_PRINTF(("etharp_find_addr() failed\n"));
        goto tcp_keepalive_offload_enable_unlock_mutex_return;
    }
    else
    {
        TCP_KA_DEBUG_PRINTF(("Destination mac address is %02x:%02x:%02x:%02x:%02x:%02x\n",
            dest_mac_addr->addr[0], dest_mac_addr->addr[1], dest_mac_addr->addr[2],
            dest_mac_addr->addr[3], dest_mac_addr->addr[4], dest_mac_addr->addr[5]));
    }

    response =  get_tcp_socket_parameters( socket, (void **)&tcp_sock_info);
    if (NSAPI_ERROR_OK != response)
    {
        TCP_KA_ERROR_PRINTF(("socket.getsockopt failed result = %d\n", response));
        result = TCP_KEEPALIVE_ERROR;
        goto tcp_keepalive_offload_enable_unlock_mutex_return;
    }

    TCP_KA_DEBUG_PRINTF(("TX seq no %lu\n", tcp_sock_info->snd_nxt));
    TCP_KA_DEBUG_PRINTF(("RX seq no %lu\n", tcp_sock_info->rcv_nxt));
    TCP_KA_DEBUG_PRINTF(("rx window size %d\n", tcp_sock_info->rcv_wnd));

    /* Disable tko */
    result = tko_enable(false);
    if (TCP_KEEPALIVE_SUCCESS != result) {
        TCP_KA_ERROR_PRINTF(("tko_enable failed result = %d\n", result));
        goto tcp_keepalive_offload_enable_unlock_mutex_return;
    }

    /* Set tko params */
    result = tko_param(tcp_keepalive_config->keepalive_interval, tcp_keepalive_config->keepalive_retry_count, tcp_keepalive_config->keepalive_retry_interval);
    if (TCP_KEEPALIVE_SUCCESS != result) {
        TCP_KA_ERROR_PRINTF(("tko_param failed result = %d\n", result));
        goto tcp_keepalive_offload_enable_unlock_mutex_return;
    }

    tcp_keepalive_offload_info->tcp_keepalive_offload_state = TCP_KEEPALIVE_OFFLOAD_DISABLED;
    memcpy(&(tcp_keepalive_offload_info->tcp_keepalive_offload_config), tcp_keepalive_config, sizeof(tcp_keepalive_offload_config_t));

    memset(tcp_keepalive_offload_internal, 0, sizeof(tcp_keepalive_offload_internal_data_t ));
    remote_ip_addr.addr = lwip_htonl(remote_ip_addr.addr);
    memcpy(&(tcp_keepalive_offload_internal->dstip), &remote_ip_addr, sizeof(tcp_keepalive_offload_internal->dstip));
    tcp_keepalive_offload_internal->dstport   = tcp_keepalive_config->remote_port;
    tcp_keepalive_offload_internal->srcport   = tcp_keepalive_config->local_port;
    tcp_keepalive_offload_internal->seqnum    = tcp_sock_info->snd_nxt;
    tcp_keepalive_offload_internal->acknum    = tcp_sock_info->rcv_nxt;
    tcp_keepalive_offload_internal->rx_window = tcp_sock_info->rcv_wnd;
    tcp_keepalive_offload_internal->index     = TCP_KEEPALIVE_OFFLOAD_INDEX;

    host_ip_addr.addr = lwip_htonl(host_ip_addr.addr);
    memcpy(&(tcp_keepalive_offload_internal->srcip), &host_ip_addr, sizeof(tcp_keepalive_offload_internal->srcip)) ;

    tcp_keepalive_offload_internal->src_mac.octet[0] = src_mac_addr.addr[0];
    tcp_keepalive_offload_internal->src_mac.octet[1] = src_mac_addr.addr[1];
    tcp_keepalive_offload_internal->src_mac.octet[2] = src_mac_addr.addr[2];
    tcp_keepalive_offload_internal->src_mac.octet[3] = src_mac_addr.addr[3];
    tcp_keepalive_offload_internal->src_mac.octet[4] = src_mac_addr.addr[4];
    tcp_keepalive_offload_internal->src_mac.octet[5] = src_mac_addr.addr[5];

    tcp_keepalive_offload_internal->dst_mac.octet[0] = dest_mac_addr->addr[0];
    tcp_keepalive_offload_internal->dst_mac.octet[1] = dest_mac_addr->addr[1];
    tcp_keepalive_offload_internal->dst_mac.octet[2] = dest_mac_addr->addr[2];
    tcp_keepalive_offload_internal->dst_mac.octet[3] = dest_mac_addr->addr[3];
    tcp_keepalive_offload_internal->dst_mac.octet[4] = dest_mac_addr->addr[4];
    tcp_keepalive_offload_internal->dst_mac.octet[5] = dest_mac_addr->addr[5];

    result = tcp_client_connect(tcp_keepalive_offload_internal);
    if (TCP_KEEPALIVE_SUCCESS != result) {
    	TCP_KA_DEBUG_PRINTF(("tcp_client_connect failed result = %d\n", result));
    	goto tcp_keepalive_offload_enable_unlock_mutex_return;
    }

    /* Register Socket Callbacks for RX/TX activity */
    result =  tcp_keepalive_offload_register_socket_callbacks(tcp_keepalive_offload_info);
    if ( result != TCP_KEEPALIVE_SUCCESS )
    {
        TCP_KA_ERROR_PRINTF(("tcp_register_socket_callbacks failed result = %d\n", result));
        tcp_keepalive_offload_disable(tcp_keepalive_offload_info);
        goto tcp_keepalive_offload_enable_unlock_mutex_return;
    }

    /* Enable TKO */
    result = tko_enable(WICED_TRUE);
    if (TCP_KEEPALIVE_SUCCESS != result)
    {
        TCP_KA_ERROR_PRINTF(("tko_enable failed result = %d\n", result));
        tcp_keepalive_offload_disable(tcp_keepalive_offload_info);
        goto tcp_keepalive_offload_enable_unlock_mutex_return;
    }

    t.start(callback(&queue, &EventQueue::dispatch_forever));
    wwd_management_set_event_handler( tko_events, handler, (void*)tcp_keepalive_offload_info, WWD_STA_INTERFACE );

    tcp_keepalive_offload_enabled = true;
    tcp_keepalive_offload_info->tcp_keepalive_offload_state = TCP_KEEPALIVE_OFFLOAD_ACTIVE;
    tcp_keepalive_offload_info->socket_tcb_pcb = tcp_sock_info;

tcp_keepalive_offload_enable_unlock_mutex_return:
    tcp_ka_offload_mutex.unlock();
    return result;
}

static void tcp_keepalive_offload_resume(tcp_keepalive_offload_info_t *tcp_keepalive_offload_info)
{
    int result;
    tcp_keepalive_offload_config_t *tcp_keepalive_config = &(tcp_keepalive_offload_info->tcp_keepalive_offload_config);
    tcp_keepalive_offload_internal_data_t *tcp_keepalive_offload = &(tcp_keepalive_offload_info->tcp_keepalive_offload_internal_data);
    struct tcp_pcb *tcp_sock_info = tcp_keepalive_offload_info->socket_tcb_pcb;

    /* Disable tko */
    result = tko_enable(false);
    if (TCP_KEEPALIVE_SUCCESS != result) {
        TCP_KA_ERROR_PRINTF(("tko_enable failed result = %d\n", result));
        return;
    }

    /* Set tko params */
    result = tko_param(tcp_keepalive_config->keepalive_interval, tcp_keepalive_config->keepalive_retry_count, tcp_keepalive_config->keepalive_retry_interval);
    if (TCP_KEEPALIVE_SUCCESS != result) {
        TCP_KA_ERROR_PRINTF(("tko_param failed result = %d\n", result));
        return;
    }

    tcp_keepalive_offload->seqnum    = tcp_sock_info->snd_nxt;
    tcp_keepalive_offload->acknum    = tcp_sock_info->rcv_nxt;
    tcp_keepalive_offload->rx_window = tcp_sock_info->rcv_wnd;

    result = tcp_client_connect(tcp_keepalive_offload);
    if (TCP_KEEPALIVE_SUCCESS != result) {
        TCP_KA_ERROR_PRINTF(("tcp_client_connessct failed result = %d\n", result));
        return;
    }

    /* Enable TKO */
    result = tko_enable(WICED_TRUE);
    if (TCP_KEEPALIVE_SUCCESS != result)
    {
        TCP_KA_ERROR_PRINTF(("tko_enable failed result = %d\n", result));
        return;
    }

    TCP_KA_DEBUG_PRINTF(("Resumed TCP KA offload\n"));
    tcp_keepalive_offload_info->tcp_keepalive_offload_state = TCP_KEEPALIVE_OFFLOAD_ACTIVE;

    result = TCP_KEEPALIVE_SUCCESS;
    return;
}

int tcp_keepalive_offload_disable(tcp_keepalive_offload_info_t *tcp_keepalive_offload_info)
{
    TCP_KA_DEBUG_PRINTF(("tcp_keepalive_offload_disable called\n"));
    tcp_keepalive_offload_deregister_socket_callbacks(tcp_keepalive_offload_info);
    wwd_management_set_event_handler( tko_events, NULL, NULL, WWD_STA_INTERFACE );
    memset(tcp_keepalive_offload_info, 0, sizeof(tcp_keepalive_offload_info_t));
    t.terminate();
	return tko_enable(false);
}

int tcp_keepalive_offload_register_socket_callbacks(tcp_keepalive_offload_info_t *tcp_keepalive_offload_info)
{
	TCPSocket *socketptr = NULL;
	tcp_keepalive_offload_config_t *tcp_keepalive_config = &(tcp_keepalive_offload_info->tcp_keepalive_offload_config);

	if ((tcp_keepalive_config != NULL) &&  (tcp_keepalive_config->socket != NULL))
	{
        socketptr = (TCPSocket *)tcp_keepalive_config->socket;
        socketptr->sigio(callback(tcp_socket_snd_rcv_callback, (void *)tcp_keepalive_offload_info));
	}

	return TCP_KEEPALIVE_SUCCESS;
}

int tcp_keepalive_offload_deregister_socket_callbacks (tcp_keepalive_offload_info_t *tcp_keepalive_offload_info)
{
	TCPSocket *socketptr = NULL;
	tcp_keepalive_offload_config_t *tcp_keepalive_config = &(tcp_keepalive_offload_info->tcp_keepalive_offload_config);

	if ((tcp_keepalive_config != NULL) &&  (tcp_keepalive_config->socket != NULL))
	{
        socketptr = (TCPSocket *)tcp_keepalive_config->socket;
        socketptr->sigio(NULL);
	}

	return TCP_KEEPALIVE_SUCCESS;
}

void tcp_keepalive_offload_ticker(tcp_keepalive_offload_info_t * tcp_keepalive_offload_info)
{
    /* Runs in interrupt context, schedule an event to run in non-interrupt context */
    queue.call(tcp_keepalive_offload_resume, tcp_keepalive_offload_info);
    lp_timer.detach();
}

void tcp_socket_snd_rcv_callback (void* tcp_keepalive_info)
{

	SocketAddress sockaddr;
	tcp_keepalive_offload_info_t *tcp_keepalive_offload_info = (tcp_keepalive_offload_info_t *)tcp_keepalive_info;
	TCPSocket *socketptr = (TCPSocket *)(tcp_keepalive_offload_info->tcp_keepalive_offload_config.socket);
	uint32_t flags = 0;

    TCP_KA_DEBUG_PRINTF(("tcp_socket_snd_rcv_callback socketptr:%p\n", socketptr));
	if (socketptr == NULL )
	{
	    TCP_KA_ERROR_PRINTF((" Invalid TCP instance\n"));
		return;
	}

    flags = socketptr->get_event();
    if ((tcp_keepalive_offload_info->tcp_keepalive_offload_state != TCP_KEEPALIVE_OFFLOAD_DISABLED) &&
       (((flags & socketptr->get_tcp_socket_read_flag()) != 0) || ((flags & socketptr->get_tcp_socket_write_flag()) != 0))) {
        tcp_keepalive_offload_info->tcp_keepalive_offload_state = TCP_KEEPALIVE_OFFLOAD_SUSPENDED;
        TCP_KA_DEBUG_PRINTF(("Calling TCP keepalive resume after %d secs \n", tcp_keepalive_offload_info->tcp_keepalive_offload_config.keepalive_interval));

        /* Start a lp_timer for keepalive_interval-1 secs
         * on timer expiry, re-enable keepalive
         */
        lp_timer.detach();
        lp_timer.attach(callback(&tcp_keepalive_offload_ticker, tcp_keepalive_offload_info), (float)(tcp_keepalive_offload_info->tcp_keepalive_offload_config.keepalive_interval-1));
    }
}

nsapi_error_t get_tcp_socket_parameters( TCPSocket *socket, void **tcpsock_info )
{
	nsapi_error_t response = NSAPI_ERROR_OK;
	response = socket->getsockopt(0, CY_GET_TCP_PCB, (void*)tcpsock_info, 0);

	if (NSAPI_ERROR_OK != response)
	{
	    TCP_KA_ERROR_PRINTF(("socket.getsockopt failed result = %d\n", response));
	}

	return response;
}

void* wwd_callback_handler ( const wwd_event_header_t* event_header, const uint8_t* event_data, /*@null@*/ void* handler_user_data )
{
    TCP_KA_DEBUG_PRINTF(("wwd_callback_handler called\n"));

	if ( event_header != NULL )
	{
		switch (event_header->event_type)
		{
			case WLC_E_TKO:
				if ( event_data != NULL && (*event_data == TCP_KEEPALIVE_OFFLOAD_INDEX))
				{
				    tcp_keepalive_offload_info_t *tcp_keepalive_offload_info = (tcp_keepalive_offload_info_t *)handler_user_data;
				    tcp_keepalive_offload_info->tcp_keepalive_offload_state = TCP_KEEPALIVE_OFFLOAD_DISABLED;
					TCP_KA_ERROR_PRINTF(("WLC_E_TKO Event Received.. Disable Keep Alive\n"));
					tcp_keepalive_offload_disable(tcp_keepalive_offload_info);
				}

			break;
			default:
				break;
		} /* end of switch */
	}

	return handler_user_data;
}
