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

/** @file
 *
 */
#include <string.h>
#include <stdbool.h>
#include "wiced_tcp_keepalive.h"


/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

static uint16_t ipv4_hdr_cksum( uint8_t *ip, int ip_len);
static uint32_t ip_cksum_partial (uint32_t sum, uint8_t* val8, uint32_t count);
static uint16_t ip_cksum (uint32_t sum, uint8_t *val8, uint32_t count );
static uint16_t ipv4_tcp_hdr_cksum(uint8_t *ip, uint8_t *tcp, uint16_t tcp_len);
static uint16_t tcp_hdr_chksum (uint32_t sum, uint8_t *tcp_hdr, uint16_t tcp_len);



/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/

int tko_enable (bool enable )
{
    uint32_t len = 0;
    uint8_t *data = NULL;
    wwd_tko_t *tko = NULL;
    wiced_buffer_t buffer;
    wwd_tko_enable_t *tko_enable = NULL;

    wwd_result_t result = WWD_SUCCESS;

    len = (int)(WICED_PAYLOAD_MTU - strlen(IOVAR_STR_TKO) - 1);
    data = (uint8_t* ) wwd_sdpcm_get_iovar_buffer(&buffer, (uint16_t)len, IOVAR_STR_TKO );

    tko = (wwd_tko_t *)data;

    tko->subcmd_id = WWD_TKO_SUBCMD_ENABLE;
    tko->len = offsetof(wwd_tko_t, data);

    tko_enable = (wwd_tko_enable_t *)tko->data;
    tko_enable->enable = enable;

    tko->len += sizeof(wwd_tko_enable_t);

    tko->subcmd_id = htod16(tko->subcmd_id);
    tko->len = htod16(tko->len);

    /* invoke SET iovar */
    result = wwd_sdpcm_send_iovar( SDPCM_SET, buffer, NULL, WWD_STA_INTERFACE );
    if ( result != WWD_SUCCESS )
    {
        TCP_KA_ERROR_PRINTF(("tko %s failed..result:%d\n", (enable == WICED_TRUE ? "enable" : "disable"), result ));
        return result;
    }
    else
    {
        if(enable == false)
        {
            TCP_KA_DEBUG_PRINTF((" tko disable success\n" ));
        }
        else
        {
            TCP_KA_DEBUG_PRINTF((" tko enable success\n" ));
        }
    }

    return WWD_SUCCESS;
}

int tcp_keepalive_pkt(uint8_t* buf, tcp_keepalive_offload_internal_data_t* keep_alive_offload )
{
    wiced_ether_header_t *eth;
    ipv4_hdr_t *ip;
    bcmtcp_hdr_t *tcp;

    if (buf == NULL)
    {
       return WWD_BADARG;
    }

    eth = (wiced_ether_header_t *)buf;
    memcpy(eth->ether_dhost, keep_alive_offload->dst_mac.octet, ETHER_ADDR_LEN);
    memcpy(eth->ether_shost, keep_alive_offload->src_mac.octet, ETHER_ADDR_LEN);

    eth->ether_type = hton16(ETHER_TYPE_IP);
    ip = (ipv4_hdr_t *)(buf + sizeof(wiced_ether_header_t));

    keep_alive_offload->dstip = hton32(keep_alive_offload->dstip);
    keep_alive_offload->srcip = hton32(keep_alive_offload->srcip);
    memcpy(ip->dst_ip, &keep_alive_offload->dstip, IPV4_ADDR_LEN);
    memcpy(ip->src_ip, &keep_alive_offload->srcip, IPV4_ADDR_LEN);


    ip->version_ihl = (IP_VER_4 << IPV4_VER_SHIFT) | (IPV4_MIN_HEADER_LEN / 4);
    ip->tos = 0;
    ip->tot_len = hton16(sizeof(ipv4_hdr_t) + sizeof(bcmtcp_hdr_t));
    ip->id = hton16(0);
    ip->frag = 0;
    ip->ttl = 32;
    ip->prot = IP_PROTO_TCP;
    ip->hdr_chksum = 0;
    ip->hdr_chksum = ipv4_hdr_cksum((uint8_t *)ip, IPV4_HLEN(ip));
    tcp = (bcmtcp_hdr_t *)(buf + sizeof(wiced_ether_header_t) +
                sizeof(ipv4_hdr_t));
    tcp->src_port = hton16(keep_alive_offload->srcport);
    tcp->dst_port = hton16(keep_alive_offload->dstport);
    tcp->seq_num  = hton32(keep_alive_offload->seqnum);
    tcp->ack_num  = hton32(keep_alive_offload->acknum);
    tcp->tcpwin   = hton16(keep_alive_offload->rx_window);
    tcp->chksum   = 0;
    tcp->urg_ptr  = 0;
    tcp->hdrlen_rsvd_flags = (TCP_FLAG_ACK << 8) |
             ((sizeof(bcmtcp_hdr_t) >> 2) << TCP_HDRLEN_SHIFT);

    /* calculate TCP header checksum */
    tcp->chksum = ipv4_tcp_hdr_cksum((uint8_t *)ip, (uint8_t *)tcp, sizeof(*tcp));

    if (tcp->chksum != 0xffff)
    {
    return (sizeof(wiced_ether_header_t) + sizeof(ipv4_hdr_t) +
                sizeof(bcmtcp_hdr_t));
    }
    else
    {
        return WWD_BADARG;
    }
}

/* calculate IPv4 header checksum
 * - input ip points to IP header in network order
 * - output cksum is in network order
 */
static uint16_t ipv4_hdr_cksum( uint8_t *ip, int ip_len)
{
   uint32_t sum = 0;
   uint8_t *ptr = ip;

   if ((ip == NULL) || (ip_len < IPV4_MIN_HEADER_LEN))
   {
	   return WWD_BADARG;
   }

   /* partial cksum skipping the hdr_chksum field */
   sum = ip_cksum_partial(sum, ptr, OFFSETOF(ipv4_hdr_t, hdr_chksum));
   ptr += OFFSETOF(ipv4_hdr_t, hdr_chksum) + 2;

   /* return calculated chksum */
   return ip_cksum(sum, ptr, ip_len - OFFSETOF(ipv4_hdr_t, src_ip));
}

/* calculate partial checksum */
static uint32_t ip_cksum_partial (uint32_t sum, uint8_t* val8, uint32_t count)
{
   uint32_t i;
   uint16_t *val16 = (uint16_t *)val8;

   if((val8 == NULL) || ((count % 2) != 0 ))
   {
	   return WWD_BADARG;
   }

   count /= 2;

   for (i = 0; i < count; i++)
   {
      sum += *val16++;
   }
   return sum;
}

/* calculate IP checksum */
static uint16_t ip_cksum (uint32_t sum, uint8_t *val8, uint32_t count )
{
   uint16_t *val16 = (uint16_t* )val8;

   if(val8 == NULL)
   {
	   return WWD_BADARG;
   }

   while (count > 1)
   {
       sum += *val16++;
       count -= 2;
   }

   /*  add left-over byte, if any */
   if (count > 0)
   {
       sum += (*(uint8_t *)val16);
   }

   /*  fold 32-bit sum to 16 bits */
   sum = (sum >> 16) + (sum & 0xffff);
   sum += (sum >> 16);
   return ((uint16_t)~sum);
}


/* calculate IPv4 TCP header checksum
 * - input ip and tcp points to IP and TCP header in network order
 * - output cksum is in network order
 */
static uint16_t ipv4_tcp_hdr_cksum(uint8_t *ip, uint8_t *tcp, uint16_t tcp_len)
{
   ipv4_hdr_t *ip_hdr = (ipv4_hdr_t *)ip;
   tcp_pseudo_hdr_t tcp_ps;
   uint32_t sum = 0;

   if ( tcp_len < TCP_MIN_HEADER_LEN )
   {
       return WWD_BADARG;
   }

   /* pseudo header cksum */
   memset(&tcp_ps, 0, sizeof(tcp_ps));
   memcpy(&tcp_ps.dst_ip, ip_hdr->dst_ip, IPV4_ADDR_LEN);
   memcpy(&tcp_ps.src_ip, ip_hdr->src_ip, IPV4_ADDR_LEN);
   tcp_ps.zero = 0;
   tcp_ps.prot = ip_hdr->prot;
   tcp_ps.tcp_size = hton16(tcp_len);
   sum = ip_cksum_partial(sum, (uint8_t *)&tcp_ps, sizeof(tcp_ps));

   /* return calculated TCP header chksum */
   return tcp_hdr_chksum(sum, tcp, tcp_len);
}

/* calculate TCP header checksum using partial sum */
static uint16_t tcp_hdr_chksum (uint32_t sum, uint8_t *tcp_hdr, uint16_t tcp_len)
{
   uint8_t *ptr = tcp_hdr;

   if ( tcp_len < TCP_MIN_HEADER_LEN )
   {
      return WWD_BADARG;
   }

   /* partial TCP cksum skipping the chksum field */
   sum = ip_cksum_partial(sum, ptr, offsetof(bcmtcp_hdr_t, chksum));
   ptr += offsetof(bcmtcp_hdr_t, chksum) + 2;

   /* return calculated chksum */
   return ip_cksum(sum, ptr, tcp_len - offsetof(bcmtcp_hdr_t, urg_ptr));
}


int tko_connect_init(wwd_tko_connect_t* connect, tcp_keepalive_offload_internal_data_t* keep_alive_offload )
{
    int datalen = 0;
    connect->index = keep_alive_offload->index;
    connect->ip_addr_type = 0;
    uint32_t srcip;
    uint32_t destip;

    connect->local_port  = htod16(keep_alive_offload->srcport);
    connect->remote_port = htod16(keep_alive_offload->dstport);
    connect->local_seq   = htod32(keep_alive_offload->seqnum);
    connect->remote_seq  = htod32(keep_alive_offload->acknum);

    destip = hton32(keep_alive_offload->dstip);
    srcip = hton32(keep_alive_offload->srcip);
    memcpy(&connect->data[datalen], &srcip, sizeof(keep_alive_offload->srcip));
    datalen += sizeof(keep_alive_offload->srcip );
    memcpy(&connect->data[datalen], &destip, sizeof(keep_alive_offload->dstip ));
    datalen += sizeof(keep_alive_offload->dstip);

    keep_alive_offload->seqnum = keep_alive_offload->seqnum - 1;

    TCP_KA_DEBUG_PRINTF(("keep alive Seq num:%lu\n",    keep_alive_offload->seqnum ));
    TCP_KA_DEBUG_PRINTF(("keep alive Ack num:%lu\n",    keep_alive_offload->acknum ));
    TCP_KA_DEBUG_PRINTF(("keep alive Rcv Window:%lu\n", keep_alive_offload->rx_window ));


    connect->request_len = tcp_keepalive_pkt(&connect->data[datalen], keep_alive_offload );
    if(connect->request_len == 0xffff)
    {
        return WWD_BADARG;
    }

    datalen += connect->request_len;
    keep_alive_offload->seqnum = keep_alive_offload->seqnum + 1;
    connect->response_len = tcp_keepalive_pkt(&connect->data[datalen],keep_alive_offload );

    if(connect->request_len == 0xffff)
    {
        return WWD_BADARG;
    }

    datalen += connect->response_len;
    return (offsetof(wwd_tko_connect_t, data) + datalen);
}

int tcp_client_connect ( tcp_keepalive_offload_internal_data_t* keep_alive_offload)
{
   uint32_t len = 0;
   uint8_t *data = NULL;
   wwd_tko_t *tko = NULL;
   wwd_tko_connect_t *connect = NULL;
   wwd_result_t result = WWD_SUCCESS;
   int tko_len;
   wiced_buffer_t buffer;

   len = (int)(WICED_PAYLOAD_MTU - strlen(IOVAR_STR_TKO) - 1);
   data = (uint8_t* ) wwd_sdpcm_get_iovar_buffer(&buffer, (uint16_t)len, IOVAR_STR_TKO );

   tko = (wwd_tko_t *)data;

   tko->subcmd_id = WWD_TKO_SUBCMD_CONNECT;
   tko->len = offsetof(wwd_tko_t, data);
   connect = (wwd_tko_connect_t *)tko->data;

   tko_len = tko_connect_init(connect, keep_alive_offload);
   if (tko_len == WWD_BADARG)
   {
       return tko_len;
   }

   tko->len += tko_len;
   /* invoke SET iovar */
   len = offsetof(wwd_tko_t, data) + tko->len;

   tko->subcmd_id = htod16(tko->subcmd_id);
   tko->len = htod16(tko->len);

   result = wwd_sdpcm_send_iovar( SDPCM_SET, buffer, NULL, WWD_STA_INTERFACE );
   if ( result != WWD_SUCCESS )
   {
       TCP_KA_ERROR_PRINTF(("tko connect failed..result:%d\n", result ));
   }
   else
   {
       TCP_KA_DEBUG_PRINTF((" tko connect success\n"));
   }

   return result;
}

int tko_param (uint16_t keepalive_interval, uint16_t keepalive_retry_count, uint16_t keepalive_retry_interval)
{
    uint32_t len = 0;
    uint8_t *data = NULL;
    wwd_tko_t *tko = NULL;
    wiced_buffer_t buffer;
    wwd_tko_param_t *tko_param = NULL;

    wwd_result_t result = WWD_SUCCESS;

    len = (int)(WICED_PAYLOAD_MTU - strlen(IOVAR_STR_TKO) - 1);
    data = (uint8_t* ) wwd_sdpcm_get_iovar_buffer(&buffer, (uint16_t)len, IOVAR_STR_TKO );

    tko = (wwd_tko_t *)data;

    tko->subcmd_id = WWD_TKO_SUBCMD_PARAM;
    tko->len = offsetof(wwd_tko_t, data);

    tko_param = (wwd_tko_param_t *)tko->data;
    tko_param->interval = keepalive_interval;
    tko_param->retry_count = keepalive_retry_count;
    tko_param->retry_interval = keepalive_retry_interval;

    tko->len += sizeof(wwd_tko_param_t);

    tko->subcmd_id = htod16(tko->subcmd_id);
    tko->len = htod16(tko->len);

    /* invoke SET iovar */
    result = wwd_sdpcm_send_iovar( SDPCM_SET, buffer, NULL, WWD_STA_INTERFACE );

    if ( result != WWD_SUCCESS )
    {
        TCP_KA_ERROR_PRINTF(("tko param failed..result:%d\n", result ));
    }
    else
    {
        TCP_KA_DEBUG_PRINTF((" tko param success\n" ));
    }

    return result;
}
