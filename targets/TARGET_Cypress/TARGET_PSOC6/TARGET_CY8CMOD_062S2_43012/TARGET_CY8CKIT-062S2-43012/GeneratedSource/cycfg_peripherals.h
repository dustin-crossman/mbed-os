/*******************************************************************************
* File Name: cycfg_peripherals.h
*
* Description:
* Peripheral Hardware Block configuration
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

#if !defined(CYCFG_PERIPHERALS_H)
#define CYCFG_PERIPHERALS_H

#include "cycfg_notices.h"
#include "cy_sysclk.h"
#include "cy_scb_uart.h"
#include "cy_scb_ezi2c.h"
#include "cy_sd_host.h"
#include "cy_smif.h"
#include "cy_mcwdt.h"
#include "cy_rtc.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define CYBSP_CAPSENSE_ENABLED 1U
#define CYBSP_BT_UART_ENABLED 1U
#define CYBSP_BT_UART_HW SCB2
#define CYBSP_BT_UART_IRQ scb_2_interrupt_IRQn
#define CYBSP_CSD_COMM_ENABLED 1U
#define CYBSP_CSD_COMM_HW SCB3
#define CYBSP_CSD_COMM_IRQ scb_3_interrupt_IRQn
#define CYBSP_DEBUG_UART_ENABLED 1U
#define CYBSP_DEBUG_UART_HW SCB5
#define CYBSP_DEBUG_UART_IRQ scb_5_interrupt_IRQn
#define CYBSP_RADIO_ENABLED 1U
#define CYBSP_RADIO_HW SDHC0
#define CYBSP_RADIO_IRQ sdhc_0_interrupt_general_IRQn
#define CYBSP_QSPI_ENABLED 1U
#define CYBSP_QSPI_HW SMIF0
#define CYBSP_QSPI_IRQ smif_interrupt_IRQn
#define CYBSP_QSPI_MEMORY_MODE_ALIGMENT_ERROR (0UL)
#define CYBSP_QSPI_RX_DATA_FIFO_UNDERFLOW (0UL)
#define CYBSP_QSPI_TX_COMMAND_FIFO_OVERFLOW (0UL)
#define CYBSP_QSPI_TX_DATA_FIFO_OVERFLOW (0UL)
#define CYBSP_QSPI_RX_FIFO_TRIGEER_LEVEL (0UL)
#define CYBSP_QSPI_TX_FIFO_TRIGEER_LEVEL (0UL)
#define CYBSP_QSPI_DATALINES0_1 (1UL)
#define CYBSP_QSPI_DATALINES2_3 (1UL)
#define CYBSP_QSPI_DATALINES4_5 (0UL)
#define CYBSP_QSPI_DATALINES6_7 (0UL)
#define CYBSP_QSPI_SS0 (1UL)
#define CYBSP_QSPI_SS1 (0UL)
#define CYBSP_QSPI_SS2 (0UL)
#define CYBSP_QSPI_SS3 (0UL)
#define CYBSP_QSPI_DESELECT_DELAY 7
#define CYBSP_MCWDT_ENABLED 1U
#define CYBSP_MCWDT_HW MCWDT_STRUCT0
#define CYBSP_RTC_ENABLED 1U
#define CYBSP_RTC_10_MONTH_OFFSET (28U)
#define CYBSP_RTC_MONTH_OFFSET (24U)
#define CYBSP_RTC_10_DAY_OFFSET (20U)
#define CYBSP_RTC_DAY_OFFSET (16U)
#define CYBSP_RTC_1000_YEAR_OFFSET (12U)
#define CYBSP_RTC_100_YEAR_OFFSET (8U)
#define CYBSP_RTC_10_YEAR_OFFSET (4U)
#define CYBSP_RTC_YEAR_OFFSET (0U)

extern const cy_stc_scb_uart_config_t CYBSP_BT_UART_config;
extern const cy_stc_scb_ezi2c_config_t CYBSP_CSD_COMM_config;
extern const cy_stc_scb_uart_config_t CYBSP_DEBUG_UART_config;
extern cy_en_sd_host_card_capacity_t CYBSP_RADIO_cardCapacity;
extern cy_en_sd_host_card_type_t CYBSP_RADIO_cardType;
extern uint32_t CYBSP_RADIO_rca;
extern const cy_stc_sd_host_init_config_t CYBSP_RADIO_config;
extern cy_stc_sd_host_sd_card_config_t CYBSP_RADIO_card_cfg;
extern const cy_stc_smif_config_t CYBSP_QSPI_config;
extern const cy_stc_mcwdt_config_t CYBSP_MCWDT_config;
extern const cy_stc_rtc_config_t CYBSP_RTC_config;

void init_cycfg_peripherals(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PERIPHERALS_H */
