/*******************************************************************************
* File Name: cycfg_routing.h
*
* Description:
* Establishes all necessary connections between hardware elements.
* This file was automatically generated and should not be modified.
* 
********************************************************************************
* Copyright 2017-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
********************************************************************************/

#if !defined(CYCFG_ROUTING_H)
#define CYCFG_ROUTING_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "cycfg_notices.h"
void init_cycfg_routing(void);
#define init_cycfg_connectivity() init_cycfg_routing()
#define ioss_0_port_0_pin_0_ANALOG P0_0_SRSS_WCO_IN
#define ioss_0_port_0_pin_1_ANALOG P0_1_SRSS_WCO_OUT
#define ioss_0_port_12_pin_0_HSIOM P12_0_DSI_GPIO
#define ioss_0_port_12_pin_1_HSIOM P12_1_DSI_DSI
#define ioss_0_port_12_pin_2_HSIOM P12_2_DSI_DSI
#define ioss_0_port_12_pin_3_HSIOM P12_3_DSI_DSI
#define ioss_0_port_12_pin_4_HSIOM P12_4_DSI_DSI
#define ioss_0_port_12_pin_5_HSIOM P12_5_DSI_DSI
#define ioss_0_port_5_pin_0_HSIOM P5_0_SCB5_UART_RX
#define ioss_0_port_5_pin_1_HSIOM P5_1_SCB5_UART_TX
#define ioss_0_port_6_pin_6_HSIOM P6_6_CPUSS_SWJ_SWDIO_TMS
#define ioss_0_port_6_pin_7_HSIOM P6_7_CPUSS_SWJ_SWCLK_TCLK

#define CYBSP_SDIO_out_p_117_TRIGGER_IN_0 TRIG0_IN_TR_GROUP14_OUTPUT1
#define CYBSP_SDIO_out_p_117_TRIGGER_IN_1 TRIG14_IN_UDB_TR_UDB1
#define CYBSP_SDIO_out_p_123_TRIGGER_IN_0 TRIG14_IN_UDB_TR_UDB7
#define CYBSP_SDIO_out_p_123_TRIGGER_IN_1 TRIG1_IN_TR_GROUP14_OUTPUT0
#define cpuss_0_dw0_0_chan_1_tr_in_0_TRIGGER_OUT TRIG0_OUT_CPUSS_DW0_TR_IN1
#define cpuss_0_dw1_0_chan_3_tr_in_0_TRIGGER_OUT TRIG1_OUT_CPUSS_DW1_TR_IN3

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_ROUTING_H */
