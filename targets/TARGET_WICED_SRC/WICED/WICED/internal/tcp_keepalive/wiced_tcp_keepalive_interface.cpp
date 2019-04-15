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
#include "wiced_tcp_keepalive.h"

static const wwd_event_num_t tko_events[]   = { WLC_E_TKO, WLC_E_NONE };
const uint8_t databuf[128] = {0};
void* wwd_callback_handler ( const wwd_event_header_t* event_header, const uint8_t* event_data, /*@null@*/ void* handler_user_data );

struct tcp_pcb *g_socket_tcp = NULL;
int tcp_keepalive_enable(WiFiInterface *wifi, tcp_keepalive_socket_params_t *tcp_keepalive_params)
{
    nsapi_error_t response;
    struct eth_addr *dest_mac_addr;
    struct eth_addr src_mac_addr;
    ip4_addr_t host_ip_addr;
    ip4_addr_t remote_ip_addr;
    const ip4_addr_t *dest_ip_addr;
    tcp_keep_alive_t tcp_keep_alive_offload;
    TCPSocket *socket = NULL;
    SocketAddress peer_addr;
    struct tcp_pcb *tcp_sock_info = NULL;
    struct tcp_pcb *tcp_sock_info_new = NULL;
    wwd_event_handler_t    handler = wwd_callback_handler;
    void *user_data = (void *)&databuf[0];

    int result;

    /* Get Local MAC address */
    DEBUG_PRINTF(("Local mac address is %s\n", wifi->get_mac_address()));

    sscanf( wifi->get_mac_address(), "%02x:%02x:%02x:%02x:%02x:%02x%*c",
        &src_mac_addr.addr[0], &src_mac_addr.addr[1], &src_mac_addr.addr[2],
		&src_mac_addr.addr[3], &src_mac_addr.addr[4], &src_mac_addr.addr[5]);

    /* Get local IP address */
    result = ipaddr_aton(wifi->get_ip_address(), &host_ip_addr);
    if (1 != result)
    {
    	DEBUG_PRINTF(("ipaddr_aton() failed result = %d\n", result));
        return result;
    }

    if ( tcp_keepalive_params == NULL )
    {
    	DEBUG_PRINTF(("KeepAlive Instance not created\n"));
    	return -1;
    }
    if ( tcp_keepalive_params->socket == NULL )
    {
    	DEBUG_PRINTF(("Socket not created\n"));
    	return -1;
    }
    socket = tcp_keepalive_params->socket;
    /* Get remote IP address */
    response = socket->getpeername(&peer_addr);
    if (0 != response) {
    	DEBUG_PRINTF(("socket.getpeeranme failed result = %d\n", response));
        return response;
    }

    DEBUG_PRINTF(("Destination ip address is %s\n", peer_addr.get_ip_address()));
    result = ipaddr_aton(peer_addr.get_ip_address(), &remote_ip_addr);
    if (1 != result )
    {
        DEBUG_PRINTF(("ipaddr_aton() failed result = %d\n", result));
        return result;
    }

    result = etharp_find_addr(NULL, &remote_ip_addr, &dest_mac_addr, &dest_ip_addr);
    if (TCP_KEEPALIVE_ERROR == result) {
        printf("etharp_find_addr() failed\n");
        return result;
    }
    else
    {
        DEBUG_PRINTF(("Destination mac address is %02x:%02x:%02x:%02x:%02x:%02x\n",
            dest_mac_addr->addr[0], dest_mac_addr->addr[1], dest_mac_addr->addr[2],
            dest_mac_addr->addr[3], dest_mac_addr->addr[4], dest_mac_addr->addr[5]));
    }

    response =  get_tcp_socket_parameters( socket, (void **)&tcp_sock_info);
    if ( (TCP_KEEPALIVE_SUCCESS != response) || ( tcp_sock_info == NULL ))
    {
    	DEBUG_PRINTF(("socket.getsockopt failed result = %d\n", response));
        return response;
    }

    DEBUG_PRINTF(("TX seq no %lu\n", tcp_sock_info->snd_nxt));
    DEBUG_PRINTF(("RX seq no %lu\n", tcp_sock_info->rcv_nxt));
    DEBUG_PRINTF(("rx window size %d\n", tcp_sock_info->rcv_wnd));

    wwd_management_set_event_handler( tko_events, handler, user_data, WWD_STA_INTERFACE );
    /* Disable tko */
    result = tko_enable(false);
    if (TCP_KEEPALIVE_SUCCESS != result) {
    	DEBUG_PRINTF_ERROR(("tko_enable failed result = %d\n", result));
        return result;
    }

    /* Get tko params */
    result = tko_param(tcp_keepalive_params->keepalive_interval, tcp_keepalive_params->keepalive_retry_count, tcp_keepalive_params->keepalive_retry_interval);
    if (TCP_KEEPALIVE_SUCCESS != result) {
    	DEBUG_PRINTF_ERROR(("tko_param failed result = %d\n", result));
        return result;
    }

    memset(&tcp_keep_alive_offload, 0, sizeof(tcp_keep_alive_offload ));
    remote_ip_addr.addr = lwip_htonl(remote_ip_addr.addr);
    memcpy(&tcp_keep_alive_offload.dstip, &remote_ip_addr, sizeof(tcp_keep_alive_offload.dstip));
    tcp_keep_alive_offload.dstport   = tcp_keepalive_params->remote_port;
    tcp_keep_alive_offload.srcport   = tcp_keepalive_params->local_port;
    tcp_keep_alive_offload.seqnum    = tcp_sock_info->snd_nxt;
    tcp_keep_alive_offload.acknum    = tcp_sock_info->rcv_nxt;
    tcp_keep_alive_offload.rx_window = tcp_sock_info->rcv_wnd;

    host_ip_addr.addr = lwip_htonl(host_ip_addr.addr);
    memcpy(&tcp_keep_alive_offload.srcip, &host_ip_addr, sizeof(tcp_keep_alive_offload.srcip)) ;

    tcp_keep_alive_offload.src_mac.octet[0] = src_mac_addr.addr[0];
    tcp_keep_alive_offload.src_mac.octet[1] = src_mac_addr.addr[1];
    tcp_keep_alive_offload.src_mac.octet[2] = src_mac_addr.addr[2];
    tcp_keep_alive_offload.src_mac.octet[3] = src_mac_addr.addr[3];
    tcp_keep_alive_offload.src_mac.octet[4] = src_mac_addr.addr[4];
    tcp_keep_alive_offload.src_mac.octet[5] = src_mac_addr.addr[5];

    tcp_keep_alive_offload.dst_mac.octet[0] = dest_mac_addr->addr[0];
    tcp_keep_alive_offload.dst_mac.octet[1] = dest_mac_addr->addr[1];
    tcp_keep_alive_offload.dst_mac.octet[2] = dest_mac_addr->addr[2];
    tcp_keep_alive_offload.dst_mac.octet[3] = dest_mac_addr->addr[3];
    tcp_keep_alive_offload.dst_mac.octet[4] = dest_mac_addr->addr[4];
    tcp_keep_alive_offload.dst_mac.octet[5] = dest_mac_addr->addr[5];

    result = tcp_client_connect(&tcp_keep_alive_offload);
    if (TCP_KEEPALIVE_SUCCESS != result) {
    	DEBUG_PRINTF(("tcp_client_connect failed result = %d\n", result));
        return result;
    }

    response =  get_tcp_socket_parameters(socket, (void **)&tcp_sock_info_new);
    if ( (TCP_KEEPALIVE_SUCCESS != response) || ( tcp_sock_info_new == NULL ))
    {
    	DEBUG_PRINTF(("socket.getsockopt failed result = %d\n", response));
         return response;
    }

     if(  ( tcp_sock_info_new->snd_nxt != tcp_sock_info->snd_nxt ) ||
    		 (tcp_sock_info_new->rcv_nxt != tcp_sock_info->rcv_nxt) ||
			 (tcp_sock_info_new->rcv_wnd != tcp_sock_info->rcv_wnd ))
     {
    	 DEBUG_PRINTF((" NEW TX seq no %lu\n", tcp_sock_info_new->snd_nxt));
    	 DEBUG_PRINTF(("NEW RX seq no %lu\n", tcp_sock_info_new->rcv_nxt));
    	 DEBUG_PRINTF(("NEW rx window size %d\n", tcp_sock_info_new->rcv_wnd));
    	 return TCP_KEEPALIVE_ERROR;
     }

    /* Register Socket Callbacks for RX/TX activity */
    result =  tcp_register_socket_callbacks(tcp_keepalive_params, tcp_keepalive_params->callbacks.send, tcp_keepalive_params->callbacks.receive, tcp_keepalive_params->callback_arg );
    if ( result != TCP_KEEPALIVE_SUCCESS )
    {
    	DEBUG_PRINTF(("tcp_register_socket_callbacks failed result = %d\n", result));
    }

    /* Enable TKO */
    result = tko_enable(WICED_TRUE);
    if (TCP_KEEPALIVE_SUCCESS != result)
    {
    	DEBUG_PRINTF(("tko_enable failed result = %d\n", result));
        return result;
    }

    return TCP_KEEPALIVE_SUCCESS;
}

int tcp_keepalive_disable(void)
{
	wwd_management_set_event_handler( tko_events, NULL, NULL, WWD_STA_INTERFACE );
	return tko_enable(false);
}

int tcp_register_socket_callbacks(tcp_keepalive_socket_params_t *tcp_keepalive_params,  tcp_socket_callback_t send_callback, tcp_socket_callback_t recv_callback, void * arg)
{
	TCPSocket *socketptr = NULL;

	if ( tcp_keepalive_params != NULL )
	{
		if ( tcp_keepalive_params->socket != NULL )
		{
		    socketptr = (TCPSocket *)tcp_keepalive_params->socket;
		    socketptr->sigio(callback(tcp_socket_snd_rcv_callback, (void *)tcp_keepalive_params));
		    tcp_keepalive_params->callbacks.send = send_callback;
		    tcp_keepalive_params->callbacks.receive = recv_callback;
	        tcp_keepalive_params->callback_arg = arg;
		}
		if ( tcp_keepalive_params->socket == NULL )
		{
			DEBUG_PRINTF(("\nUnable to register callbacks.. SOCKET IS NULL\n"));
		}
	}
	return TCP_KEEPALIVE_SUCCESS;
}

int tcp_deregister_socket_callbacks ( tcp_keepalive_socket_params_t *tcp_keep_alive_params )
{
	TCPSocket *socketptr = NULL;
	if ( tcp_keep_alive_params != NULL )
	{
	    if ( tcp_keep_alive_params->socket != NULL )
	    {
		    socketptr = (TCPSocket *)tcp_keep_alive_params->socket;
		    socketptr->sigio(NULL);
		    tcp_keep_alive_params->callbacks.send = NULL;
		    tcp_keep_alive_params->callbacks.receive = NULL;
		    tcp_keep_alive_params->callback_arg = NULL;
        }
		if ( tcp_keep_alive_params->socket == NULL )
		{
			DEBUG_PRINTF(("\nUnable to deregister callbacks.. SOCKET IS NULL\n"));
     	}
	}
	return TCP_KEEPALIVE_SUCCESS;
}

void tcp_socket_snd_rcv_callback (void* params)
{

	SocketAddress sockaddr;
	tcp_keepalive_socket_params_t *tcpkeep_alive = (tcp_keepalive_socket_params_t *)params;

	if ( tcpkeep_alive == NULL )
	{
		return;
	}

	if ( tcpkeep_alive->socket == NULL )
	{
		DEBUG_PRINTF((" Invalid TCP instance\n"));
		return;
	}

	TCPSocket *socketptr = (TCPSocket *)tcpkeep_alive->socket;
	uint32_t flags = 0;
	DEBUG_PRINTF(("tcp_socket_snd_rcv_callback socketptr:%p\n", socketptr ));
	if ( socketptr != NULL )
	{
		flags = socketptr->get_event();
		if ((flags & socketptr->get_tcp_socket_read_flag() ) != 0)
		{
		  	if ( tcpkeep_alive->callbacks.receive != NULL )
		  	{
		  		tcpkeep_alive->callbacks.receive(tcpkeep_alive, tcpkeep_alive->callback_arg );
		  	}
		}
		if ((flags & socketptr->get_tcp_socket_write_flag() ) != 0 )
		{
		  	if ( tcpkeep_alive->callbacks.send != NULL )
		  	{
		  		tcpkeep_alive->callbacks.send(tcpkeep_alive, tcpkeep_alive->callback_arg);
		   	}
		}
	}
}

nsapi_error_t get_tcp_socket_parameters( TCPSocket *socket, void **tcpsock_info )
{
	nsapi_error_t response = NSAPI_ERROR_OK;
	if ( g_socket_tcp == NULL )
	{
	    response = socket->getsockopt(0, CY_GET_TCP_PCB, (void*)&g_socket_tcp, 0);
	    if (TCP_KEEPALIVE_SUCCESS != response)
	    {
	    	DEBUG_PRINTF(("socket.getsockopt failed result = %d\n", response));
	       return response;
	    }
	}
	*tcpsock_info = g_socket_tcp;
	return response;
}

void* wwd_callback_handler ( const wwd_event_header_t* event_header, const uint8_t* event_data, /*@null@*/ void* handler_user_data )
{
	if ( event_header != NULL )
	{
		switch (event_header->event_type)
		{
			case WLC_E_TKO:
				if ( event_data != NULL )
				{
					DEBUG_PRINTF_ERROR(("WLC_E_TKO Event Received.. failed index:%d\n", *event_data));
					DEBUG_PRINTF_ERROR(("Disable Keep Alive\n"));
					/* tcp_keepalive_disable(); */
				}
			break;
			default:
				break;
		} /* end of switch */
	}
	return handler_user_data;
}
