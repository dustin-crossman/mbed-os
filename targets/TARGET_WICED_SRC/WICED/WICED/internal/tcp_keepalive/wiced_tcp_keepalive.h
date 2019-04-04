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
#pragma once

#include "wiced_management.h"
#include "wiced_wifi.h"
#include "wwd_wifi.h"
#include "wwd_network_constants.h"
#include "wwd_wlioctl.h"
#include "wwd_constants.h"
#include "wwd_structures.h"
#include "wwd_bcmendian.h"
#include "wwd_sdpcm.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

#ifndef OFFSETOF
#define OFFSETOF( type, member )  ( (uintptr_t)&((type *)0)->member )
#endif /* OFFSETOF */

//Uncomment below line to debug TCP KEEPALIVE
//#define TCP_KEEPALIVE_DEBUG

#ifdef TCP_KEEPALIVE_DEBUG
#define DEBUG_PRINTF(x) printf x
#else
#define DEBUG_PRINTF(x)
#endif /* TCP_KEEPALIVE_DEBUG */

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

typedef struct tcp_pseudo_hdr
{
  uint8_t  src_ip[IPV4_ADDR_LEN];  /* Source IP Address */
  uint8_t  dst_ip[IPV4_ADDR_LEN];  /* Destination IP Address */
  uint8_t  zero;
  uint8_t  prot;
  uint16_t tcp_size;
} tcp_pseudo_hdr_t;

typedef struct tcp_keepalive
{
  uint8_t index;
  uint8_t ipaddr_type;
  wiced_mac_t src_mac;
  wiced_mac_t dst_mac;
  uint32_t srcip;
  uint32_t dstip;
  uint16_t srcport;
  uint16_t dstport;
  uint32_t seqnum;
  uint32_t acknum;
  uint16_t rx_window;
} tcp_keep_alive_t;

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

int tko_enable (bool enable);
int tko_param (uint16_t keepalive_interval, uint16_t keepalive_retry_count, uint16_t keepalive_retry_interval);
int tko_connect_init(wwd_tko_connect_t* connect, tcp_keep_alive_t* keep_alive_offload );
int tcp_client_connect ( tcp_keep_alive_t* keep_alive_offload);

/* TCP socket callback */
void tcp_socket_snd_rcv_callback (void* params);


#ifdef __cplusplus
} /*extern "C" */
#endif
