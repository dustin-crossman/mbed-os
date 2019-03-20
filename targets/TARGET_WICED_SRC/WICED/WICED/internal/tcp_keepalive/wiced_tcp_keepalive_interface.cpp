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

int tcp_keepalive_enable(WiFiInterface *wifi, TCPSocket *socket, tcp_keepalive_params_t *tcp_keepalive_params)
{
    nsapi_error_t response;
    struct eth_addr *dest_mac_addr;
    struct eth_addr src_mac_addr;
    ip4_addr_t host_ip_addr;
    ip4_addr_t remote_ip_addr;
    const ip4_addr_t *dest_ip_addr;
    tcp_keep_alive_t tcp_keep_alive_offload;
    SocketAddress peer_addr;
    struct tcp_pcb *socket_tcp;
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
        printf("ipaddr_aton() failed result = %d\n", result);
        return result;
    }

    /* Get remote IP address */
    response = socket->getpeername(&peer_addr);
    if (0 != response) {
        printf("socket.getpeeranme failed result = %d\n", response);
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

    response = socket->getsockopt(0, CY_GET_TCP_PCB, (void*)&socket_tcp, 0);
    if (TCP_KEEPALIVE_SUCCESS != response) {
        printf("socket.getsockopt failed result = %d\n", response);
        return response;
    }

    DEBUG_PRINTF(("TX seq no %lu\n", socket_tcp->snd_nxt));
    DEBUG_PRINTF(("RX seq no %lu\n", socket_tcp->rcv_nxt));
    DEBUG_PRINTF(("rx window size %d\n", socket_tcp->rcv_wnd));

    /* Disable tko */
    result = tko_enable(false);
    if (TCP_KEEPALIVE_SUCCESS != result) {
        printf("tko_enable failed result = %d\n", result);
        return result;
    }

    /* Get tko params */
    result = tko_param(tcp_keepalive_params->keepalive_interval, tcp_keepalive_params->keepalive_retry_count, tcp_keepalive_params->keepalive_retry_interval);
    if (TCP_KEEPALIVE_SUCCESS != result) {
        printf("tko_param failed result = %d\n", result);
        return result;
    }

    memset(&tcp_keep_alive_offload, 0, sizeof(tcp_keep_alive_offload ));
    remote_ip_addr.addr = lwip_htonl(remote_ip_addr.addr);
    memcpy(&tcp_keep_alive_offload.dstip, &remote_ip_addr, sizeof(tcp_keep_alive_offload.dstip));
    tcp_keep_alive_offload.dstport   = tcp_keepalive_params->remote_port;
    tcp_keep_alive_offload.srcport   = tcp_keepalive_params->local_port;
    tcp_keep_alive_offload.seqnum    = socket_tcp->snd_nxt;
    tcp_keep_alive_offload.acknum    = socket_tcp->rcv_nxt;
    tcp_keep_alive_offload.rx_window = socket_tcp->rcv_wnd;

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
        printf("tcp_client_connect failed result = %d\n", result);
        return result;
    }

    result = tko_enable(WICED_TRUE);
    if (TCP_KEEPALIVE_SUCCESS != result) {
        printf("tko_enable failed result = %d\n", result);
        return result;
    }

    return TCP_KEEPALIVE_SUCCESS;
}

int tcp_keepalive_disable(void)
{
	return tko_enable(false);
}
