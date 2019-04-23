/***************************************************************************//**
* \file cy_hal_psoc6_03_68_qfn.c
*
* \brief
* PSoC6_03 device GPIO HAL header for 68-QFN package
*
* \note
* Generator version: 1.4.7041.28123
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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
*******************************************************************************/

#include "cy_device_headers.h"
#include "cy_hal_hw_types.h"

#if defined(_GPIO_PSOC6_03_68_QFN_H_)
#include "pin_packages/cy_hal_psoc6_03_68_qfn.h"

/* Hardware Blocks */
static const cy_resource_inst_t CY_CANFD_0 = { CY_RSC_CANFD, 0, 0 };
static const cy_resource_inst_t CY_SCB_0 = { CY_RSC_SCB, 0, 0 };
static const cy_resource_inst_t CY_SCB_1 = { CY_RSC_SCB, 1, 0 };
static const cy_resource_inst_t CY_SCB_2 = { CY_RSC_SCB, 2, 0 };
static const cy_resource_inst_t CY_SCB_3 = { CY_RSC_SCB, 3, 0 };
static const cy_resource_inst_t CY_SCB_4 = { CY_RSC_SCB, 4, 0 };
static const cy_resource_inst_t CY_SCB_5 = { CY_RSC_SCB, 5, 0 };
static const cy_resource_inst_t CY_SCB_6 = { CY_RSC_SCB, 6, 0 };
static const cy_resource_inst_t CY_SDHC_0 = { CY_RSC_SDHC, 0, 0 };
static const cy_resource_inst_t CY_SMIF_0 = { CY_RSC_SMIF, 0, 0 };
static const cy_resource_inst_t CY_TCPWM_0_0 = { CY_RSC_TCPWM, 0, 0 };
static const cy_resource_inst_t CY_TCPWM_0_1 = { CY_RSC_TCPWM, 0, 1 };
static const cy_resource_inst_t CY_TCPWM_0_2 = { CY_RSC_TCPWM, 0, 2 };
static const cy_resource_inst_t CY_TCPWM_0_3 = { CY_RSC_TCPWM, 0, 3 };
static const cy_resource_inst_t CY_TCPWM_1_0 = { CY_RSC_TCPWM, 1, 0 };
static const cy_resource_inst_t CY_TCPWM_1_1 = { CY_RSC_TCPWM, 1, 1 };
static const cy_resource_inst_t CY_TCPWM_1_2 = { CY_RSC_TCPWM, 1, 2 };
static const cy_resource_inst_t CY_TCPWM_1_3 = { CY_RSC_TCPWM, 1, 3 };
static const cy_resource_inst_t CY_TCPWM_1_4 = { CY_RSC_TCPWM, 1, 4 };
static const cy_resource_inst_t CY_TCPWM_1_5 = { CY_RSC_TCPWM, 1, 5 };
static const cy_resource_inst_t CY_TCPWM_1_6 = { CY_RSC_TCPWM, 1, 6 };
static const cy_resource_inst_t CY_TCPWM_1_7 = { CY_RSC_TCPWM, 1, 7 };

/* Pin connections */
/* Connections for: canfd_ttcan_rx */
const cy_resource_pin_mapping_t cy_pin_map_canfd_ttcan_rx[1] = {
    {P5_0, &CY_CANFD_0, CY_PIN_IN_FUNCTION(P5_0_CANFD0_TTCAN_RX0)},
};

/* Connections for: canfd_ttcan_tx */
const cy_resource_pin_mapping_t cy_pin_map_canfd_ttcan_tx[1] = {
    {P5_1, &CY_CANFD_0, CY_PIN_OUT_FUNCTION(P5_1_CANFD0_TTCAN_TX0)},
};

/* Connections for: scb_i2c_scl */
const cy_resource_pin_mapping_t cy_pin_map_scb_i2c_scl[10] = {
    {P0_2, &CY_SCB_0, CY_PIN_OD_FUNCTION(P0_2_SCB0_I2C_SCL)},
    {P2_0, &CY_SCB_1, CY_PIN_OD_FUNCTION(P2_0_SCB1_I2C_SCL)},
    {P3_0, &CY_SCB_2, CY_PIN_OD_FUNCTION(P3_0_SCB2_I2C_SCL)},
    {P5_0, &CY_SCB_5, CY_PIN_OD_FUNCTION(P5_0_SCB5_I2C_SCL)},
    {P6_4, &CY_SCB_6, CY_PIN_OD_FUNCTION(P6_4_SCB6_I2C_SCL)},
    {P7_0, &CY_SCB_4, CY_PIN_OD_FUNCTION(P7_0_SCB4_I2C_SCL)},
    {P8_0, &CY_SCB_4, CY_PIN_OD_FUNCTION(P8_0_SCB4_I2C_SCL)},
    {P9_0, &CY_SCB_2, CY_PIN_OD_FUNCTION(P9_0_SCB2_I2C_SCL)},
    {P10_0, &CY_SCB_1, CY_PIN_OD_FUNCTION(P10_0_SCB1_I2C_SCL)},
    {P11_0, &CY_SCB_5, CY_PIN_OD_FUNCTION(P11_0_SCB5_I2C_SCL)},
};

/* Connections for: scb_i2c_sda */
const cy_resource_pin_mapping_t cy_pin_map_scb_i2c_sda[10] = {
    {P0_3, &CY_SCB_0, CY_PIN_OD_FUNCTION(P0_3_SCB0_I2C_SDA)},
    {P2_1, &CY_SCB_1, CY_PIN_OD_FUNCTION(P2_1_SCB1_I2C_SDA)},
    {P3_1, &CY_SCB_2, CY_PIN_OD_FUNCTION(P3_1_SCB2_I2C_SDA)},
    {P5_1, &CY_SCB_5, CY_PIN_OD_FUNCTION(P5_1_SCB5_I2C_SDA)},
    {P6_5, &CY_SCB_6, CY_PIN_OD_FUNCTION(P6_5_SCB6_I2C_SDA)},
    {P7_1, &CY_SCB_4, CY_PIN_OD_FUNCTION(P7_1_SCB4_I2C_SDA)},
    {P8_1, &CY_SCB_4, CY_PIN_OD_FUNCTION(P8_1_SCB4_I2C_SDA)},
    {P9_1, &CY_SCB_2, CY_PIN_OD_FUNCTION(P9_1_SCB2_I2C_SDA)},
    {P10_1, &CY_SCB_1, CY_PIN_OD_FUNCTION(P10_1_SCB1_I2C_SDA)},
    {P11_1, &CY_SCB_5, CY_PIN_OD_FUNCTION(P11_1_SCB5_I2C_SDA)},
};

/* Connections for: scb_spi_clk */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_clk[8] = {
    {P0_4, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_4_SCB0_SPI_CLK)},
    {P2_2, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P2_2_SCB1_SPI_CLK)},
    {P6_2, &CY_SCB_3, CY_PIN_OUT_FUNCTION(P6_2_SCB3_SPI_CLK)},
    {P6_6, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P6_6_SCB6_SPI_CLK)},
    {P7_2, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P7_2_SCB4_SPI_CLK)},
    {P9_2, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_2_SCB2_SPI_CLK)},
    {P10_2, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_2_SCB1_SPI_CLK)},
    {P11_2, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P11_2_SCB5_SPI_CLK)},
};

/* Connections for: scb_spi_miso */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_miso[10] = {
    {P0_3, &CY_SCB_0, CY_PIN_IN_FUNCTION(P0_3_SCB0_SPI_MISO)},
    {P2_1, &CY_SCB_1, CY_PIN_IN_FUNCTION(P2_1_SCB1_SPI_MISO)},
    {P3_1, &CY_SCB_2, CY_PIN_IN_FUNCTION(P3_1_SCB2_SPI_MISO)},
    {P5_1, &CY_SCB_5, CY_PIN_IN_FUNCTION(P5_1_SCB5_SPI_MISO)},
    {P6_5, &CY_SCB_6, CY_PIN_IN_FUNCTION(P6_5_SCB6_SPI_MISO)},
    {P7_1, &CY_SCB_4, CY_PIN_IN_FUNCTION(P7_1_SCB4_SPI_MISO)},
    {P8_1, &CY_SCB_4, CY_PIN_IN_FUNCTION(P8_1_SCB4_SPI_MISO)},
    {P9_1, &CY_SCB_2, CY_PIN_IN_FUNCTION(P9_1_SCB2_SPI_MISO)},
    {P10_1, &CY_SCB_1, CY_PIN_IN_FUNCTION(P10_1_SCB1_SPI_MISO)},
    {P11_1, &CY_SCB_5, CY_PIN_IN_FUNCTION(P11_1_SCB5_SPI_MISO)},
};

/* Connections for: scb_spi_mosi */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_mosi[10] = {
    {P0_2, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_2_SCB0_SPI_MOSI)},
    {P2_0, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P2_0_SCB1_SPI_MOSI)},
    {P3_0, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P3_0_SCB2_SPI_MOSI)},
    {P5_0, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_0_SCB5_SPI_MOSI)},
    {P6_4, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P6_4_SCB6_SPI_MOSI)},
    {P7_0, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P7_0_SCB4_SPI_MOSI)},
    {P8_0, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P8_0_SCB4_SPI_MOSI)},
    {P9_0, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_0_SCB2_SPI_MOSI)},
    {P10_0, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_0_SCB1_SPI_MOSI)},
    {P11_0, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P11_0_SCB5_SPI_MOSI)},
};

/* Connections for: scb_spi_select0 */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select0[8] = {
    {P0_5, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_5_SCB0_SPI_SELECT0)},
    {P2_3, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P2_3_SCB1_SPI_SELECT0)},
    {P6_3, &CY_SCB_3, CY_PIN_OUT_FUNCTION(P6_3_SCB3_SPI_SELECT0)},
    {P6_7, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P6_7_SCB6_SPI_SELECT0)},
    {P7_3, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P7_3_SCB4_SPI_SELECT0)},
    {P9_3, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_3_SCB2_SPI_SELECT0)},
    {P10_3, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_3_SCB1_SPI_SELECT0)},
    {P11_3, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P11_3_SCB5_SPI_SELECT0)},
};

/* Connections for: scb_spi_select1 */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select1[4] = {
    {P0_0, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_0_SCB0_SPI_SELECT1)},
    {P2_4, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P2_4_SCB1_SPI_SELECT1)},
    {P10_4, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_4_SCB1_SPI_SELECT1)},
    {P11_4, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P11_4_SCB5_SPI_SELECT1)},
};

/* Connections for: scb_spi_select2 */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select2[4] = {
    {P0_1, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_1_SCB0_SPI_SELECT2)},
    {P2_5, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P2_5_SCB1_SPI_SELECT2)},
    {P10_5, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_5_SCB1_SPI_SELECT2)},
    {P11_5, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P11_5_SCB5_SPI_SELECT2)},
};

/* Connections for: scb_spi_select3 */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select3[2] = {
    {P2_6, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P2_6_SCB1_SPI_SELECT3)},
    {P11_6, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P11_6_SCB5_SPI_SELECT3)},
};

/* Connections for: scb_uart_cts */
const cy_resource_pin_mapping_t cy_pin_map_scb_uart_cts[7] = {
    {P0_5, &CY_SCB_0, CY_PIN_IN_FUNCTION(P0_5_SCB0_UART_CTS)},
    {P2_3, &CY_SCB_1, CY_PIN_IN_FUNCTION(P2_3_SCB1_UART_CTS)},
    {P6_3, &CY_SCB_3, CY_PIN_IN_FUNCTION(P6_3_SCB3_UART_CTS)},
    {P7_3, &CY_SCB_4, CY_PIN_IN_FUNCTION(P7_3_SCB4_UART_CTS)},
    {P9_3, &CY_SCB_2, CY_PIN_IN_FUNCTION(P9_3_SCB2_UART_CTS)},
    {P10_3, &CY_SCB_1, CY_PIN_IN_FUNCTION(P10_3_SCB1_UART_CTS)},
    {P11_3, &CY_SCB_5, CY_PIN_IN_FUNCTION(P11_3_SCB5_UART_CTS)},
};

/* Connections for: scb_uart_rts */
const cy_resource_pin_mapping_t cy_pin_map_scb_uart_rts[7] = {
    {P0_4, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_4_SCB0_UART_RTS)},
    {P2_2, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P2_2_SCB1_UART_RTS)},
    {P6_2, &CY_SCB_3, CY_PIN_OUT_FUNCTION(P6_2_SCB3_UART_RTS)},
    {P7_2, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P7_2_SCB4_UART_RTS)},
    {P9_2, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_2_SCB2_UART_RTS)},
    {P10_2, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_2_SCB1_UART_RTS)},
    {P11_2, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P11_2_SCB5_UART_RTS)},
};

/* Connections for: scb_uart_rx */
const cy_resource_pin_mapping_t cy_pin_map_scb_uart_rx[9] = {
    {P0_2, &CY_SCB_0, CY_PIN_IN_FUNCTION(P0_2_SCB0_UART_RX)},
    {P2_0, &CY_SCB_1, CY_PIN_IN_FUNCTION(P2_0_SCB1_UART_RX)},
    {P3_0, &CY_SCB_2, CY_PIN_IN_FUNCTION(P3_0_SCB2_UART_RX)},
    {P5_0, &CY_SCB_5, CY_PIN_IN_FUNCTION(P5_0_SCB5_UART_RX)},
    {P7_0, &CY_SCB_4, CY_PIN_IN_FUNCTION(P7_0_SCB4_UART_RX)},
    {P8_0, &CY_SCB_4, CY_PIN_IN_FUNCTION(P8_0_SCB4_UART_RX)},
    {P9_0, &CY_SCB_2, CY_PIN_IN_FUNCTION(P9_0_SCB2_UART_RX)},
    {P10_0, &CY_SCB_1, CY_PIN_IN_FUNCTION(P10_0_SCB1_UART_RX)},
    {P11_0, &CY_SCB_5, CY_PIN_IN_FUNCTION(P11_0_SCB5_UART_RX)},
};

/* Connections for: scb_uart_tx */
const cy_resource_pin_mapping_t cy_pin_map_scb_uart_tx[9] = {
    {P0_3, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_3_SCB0_UART_TX)},
    {P2_1, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P2_1_SCB1_UART_TX)},
    {P3_1, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P3_1_SCB2_UART_TX)},
    {P5_1, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_1_SCB5_UART_TX)},
    {P7_1, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P7_1_SCB4_UART_TX)},
    {P8_1, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P8_1_SCB4_UART_TX)},
    {P9_1, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_1_SCB2_UART_TX)},
    {P10_1, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_1_SCB1_UART_TX)},
    {P11_1, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P11_1_SCB5_UART_TX)},
};

/* Connections for: sdhc_card_cmd */
const cy_resource_pin_mapping_t cy_pin_map_sdhc_card_cmd[1] = {
    {P2_4, &CY_SDHC_0, CY_PIN_OUT_BUF_FUNCTION(P2_4_SDHC0_CARD_CMD)},
};

/* Connections for: sdhc_card_dat_3to0 */
const cy_resource_pin_mapping_t cy_pin_map_sdhc_card_dat_3to0[4] = {
    {P2_0, &CY_SDHC_0, CY_PIN_OUT_BUF_FUNCTION(P2_0_SDHC0_CARD_DAT_3TO00)},
    {P2_1, &CY_SDHC_0, CY_PIN_OUT_BUF_FUNCTION(P2_1_SDHC0_CARD_DAT_3TO01)},
    {P2_2, &CY_SDHC_0, CY_PIN_OUT_BUF_FUNCTION(P2_2_SDHC0_CARD_DAT_3TO02)},
    {P2_3, &CY_SDHC_0, CY_PIN_OUT_BUF_FUNCTION(P2_3_SDHC0_CARD_DAT_3TO03)},
};

/* Connections for: sdhc_card_detect_n */
const cy_resource_pin_mapping_t cy_pin_map_sdhc_card_detect_n[1] = {
    {P2_6, &CY_SDHC_0, CY_PIN_IN_FUNCTION(P2_6_SDHC0_CARD_DETECT_N)},
};

/* Connections for: sdhc_card_if_pwr_en */
const cy_resource_pin_mapping_t cy_pin_map_sdhc_card_if_pwr_en[1] = {
    {P3_1, &CY_SDHC_0, CY_PIN_OUT_FUNCTION(P3_1_SDHC0_CARD_IF_PWR_EN)},
};

/* Connections for: sdhc_card_mech_write_prot */
const cy_resource_pin_mapping_t cy_pin_map_sdhc_card_mech_write_prot[1] = {
    {P2_7, &CY_SDHC_0, CY_PIN_IN_FUNCTION(P2_7_SDHC0_CARD_MECH_WRITE_PROT)},
};

/* Connections for: sdhc_clk_card */
const cy_resource_pin_mapping_t cy_pin_map_sdhc_clk_card[1] = {
    {P2_5, &CY_SDHC_0, CY_PIN_OUT_BUF_FUNCTION(P2_5_SDHC0_CLK_CARD)},
};

/* Connections for: sdhc_io_volt_sel */
const cy_resource_pin_mapping_t cy_pin_map_sdhc_io_volt_sel[1] = {
    {P3_0, &CY_SDHC_0, CY_PIN_OUT_FUNCTION(P3_0_SDHC0_IO_VOLT_SEL)},
};

/* Connections for: smif_spi_clk */
const cy_resource_pin_mapping_t cy_pin_map_smif_spi_clk[1] = {
    {P11_7, &CY_SMIF_0, CY_PIN_OUT_BUF_FUNCTION(P11_7_SMIF_SPI_CLK)},
};

/* Connections for: smif_spi_data0 */
const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data0[1] = {
    {P11_6, &CY_SMIF_0, CY_PIN_OUT_BUF_FUNCTION(P11_6_SMIF_SPI_DATA0)},
};

/* Connections for: smif_spi_data1 */
const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data1[1] = {
    {P11_5, &CY_SMIF_0, CY_PIN_OUT_BUF_FUNCTION(P11_5_SMIF_SPI_DATA1)},
};

/* Connections for: smif_spi_data2 */
const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data2[1] = {
    {P11_4, &CY_SMIF_0, CY_PIN_OUT_BUF_FUNCTION(P11_4_SMIF_SPI_DATA2)},
};

/* Connections for: smif_spi_data3 */
const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data3[1] = {
    {P11_3, &CY_SMIF_0, CY_PIN_OUT_BUF_FUNCTION(P11_3_SMIF_SPI_DATA3)},
};

/* Connections for: smif_spi_select0 */
const cy_resource_pin_mapping_t cy_pin_map_smif_spi_select0[1] = {
    {P11_2, &CY_SMIF_0, CY_PIN_OUT_FUNCTION(P11_2_SMIF_SPI_SELECT0)},
};

/* Connections for: smif_spi_select1 */
const cy_resource_pin_mapping_t cy_pin_map_smif_spi_select1[1] = {
    {P11_1, &CY_SMIF_0, CY_PIN_OUT_FUNCTION(P11_1_SMIF_SPI_SELECT1)},
};

/* Connections for: smif_spi_select2 */
const cy_resource_pin_mapping_t cy_pin_map_smif_spi_select2[1] = {
    {P11_0, &CY_SMIF_0, CY_PIN_OUT_FUNCTION(P11_0_SMIF_SPI_SELECT2)},
};

/* Connections for: tcpwm_line */
const cy_resource_pin_mapping_t cy_pin_map_tcpwm_line[52] = {
    {P0_0, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P0_0_TCPWM0_LINE0)},
    {P0_0, &CY_TCPWM_1_0, CY_PIN_OUT_FUNCTION(P0_0_TCPWM1_LINE0)},
    {P0_2, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P0_2_TCPWM0_LINE1)},
    {P0_2, &CY_TCPWM_1_1, CY_PIN_OUT_FUNCTION(P0_2_TCPWM1_LINE1)},
    {P0_4, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P0_4_TCPWM0_LINE2)},
    {P0_4, &CY_TCPWM_1_2, CY_PIN_OUT_FUNCTION(P0_4_TCPWM1_LINE2)},
    {P2_0, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P2_0_TCPWM0_LINE3)},
    {P2_0, &CY_TCPWM_1_3, CY_PIN_OUT_FUNCTION(P2_0_TCPWM1_LINE3)},
    {P2_2, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P2_2_TCPWM0_LINE0)},
    {P2_2, &CY_TCPWM_1_4, CY_PIN_OUT_FUNCTION(P2_2_TCPWM1_LINE4)},
    {P2_4, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P2_4_TCPWM0_LINE1)},
    {P2_4, &CY_TCPWM_1_5, CY_PIN_OUT_FUNCTION(P2_4_TCPWM1_LINE5)},
    {P2_6, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P2_6_TCPWM0_LINE2)},
    {P2_6, &CY_TCPWM_1_6, CY_PIN_OUT_FUNCTION(P2_6_TCPWM1_LINE6)},
    {P3_0, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P3_0_TCPWM0_LINE3)},
    {P3_0, &CY_TCPWM_1_7, CY_PIN_OUT_FUNCTION(P3_0_TCPWM1_LINE7)},
    {P5_0, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P5_0_TCPWM0_LINE0)},
    {P5_0, &CY_TCPWM_1_0, CY_PIN_OUT_FUNCTION(P5_0_TCPWM1_LINE0)},
    {P5_6, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P5_6_TCPWM0_LINE1)},
    {P5_6, &CY_TCPWM_1_1, CY_PIN_OUT_FUNCTION(P5_6_TCPWM1_LINE1)},
    {P6_2, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P6_2_TCPWM0_LINE3)},
    {P6_2, &CY_TCPWM_1_3, CY_PIN_OUT_FUNCTION(P6_2_TCPWM1_LINE3)},
    {P6_4, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P6_4_TCPWM0_LINE0)},
    {P6_4, &CY_TCPWM_1_4, CY_PIN_OUT_FUNCTION(P6_4_TCPWM1_LINE4)},
    {P6_6, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P6_6_TCPWM0_LINE1)},
    {P6_6, &CY_TCPWM_1_5, CY_PIN_OUT_FUNCTION(P6_6_TCPWM1_LINE5)},
    {P7_0, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P7_0_TCPWM0_LINE2)},
    {P7_0, &CY_TCPWM_1_6, CY_PIN_OUT_FUNCTION(P7_0_TCPWM1_LINE6)},
    {P7_2, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P7_2_TCPWM0_LINE3)},
    {P7_2, &CY_TCPWM_1_7, CY_PIN_OUT_FUNCTION(P7_2_TCPWM1_LINE7)},
    {P8_0, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P8_0_TCPWM0_LINE2)},
    {P8_0, &CY_TCPWM_1_2, CY_PIN_OUT_FUNCTION(P8_0_TCPWM1_LINE2)},
    {P9_0, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P9_0_TCPWM0_LINE0)},
    {P9_0, &CY_TCPWM_1_4, CY_PIN_OUT_FUNCTION(P9_0_TCPWM1_LINE4)},
    {P9_2, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P9_2_TCPWM0_LINE1)},
    {P9_2, &CY_TCPWM_1_5, CY_PIN_OUT_FUNCTION(P9_2_TCPWM1_LINE5)},
    {P10_0, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P10_0_TCPWM0_LINE2)},
    {P10_0, &CY_TCPWM_1_6, CY_PIN_OUT_FUNCTION(P10_0_TCPWM1_LINE6)},
    {P10_2, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P10_2_TCPWM0_LINE3)},
    {P10_2, &CY_TCPWM_1_7, CY_PIN_OUT_FUNCTION(P10_2_TCPWM1_LINE7)},
    {P10_4, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P10_4_TCPWM0_LINE0)},
    {P10_4, &CY_TCPWM_1_0, CY_PIN_OUT_FUNCTION(P10_4_TCPWM1_LINE0)},
    {P11_0, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P11_0_TCPWM0_LINE2)},
    {P11_0, &CY_TCPWM_1_2, CY_PIN_OUT_FUNCTION(P11_0_TCPWM1_LINE2)},
    {P11_2, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P11_2_TCPWM0_LINE3)},
    {P11_2, &CY_TCPWM_1_3, CY_PIN_OUT_FUNCTION(P11_2_TCPWM1_LINE3)},
    {P11_4, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P11_4_TCPWM0_LINE0)},
    {P11_4, &CY_TCPWM_1_4, CY_PIN_OUT_FUNCTION(P11_4_TCPWM1_LINE4)},
    {P11_6, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P11_6_TCPWM0_LINE1)},
    {P11_6, &CY_TCPWM_1_5, CY_PIN_OUT_FUNCTION(P11_6_TCPWM1_LINE5)},
    {P12_6, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P12_6_TCPWM0_LINE3)},
    {P12_6, &CY_TCPWM_1_7, CY_PIN_OUT_FUNCTION(P12_6_TCPWM1_LINE7)},
};

/* Connections for: tcpwm_line_compl */
const cy_resource_pin_mapping_t cy_pin_map_tcpwm_line_compl[54] = {
    {P0_1, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P0_1_TCPWM0_LINE_COMPL0)},
    {P0_1, &CY_TCPWM_1_0, CY_PIN_OUT_FUNCTION(P0_1_TCPWM1_LINE_COMPL0)},
    {P0_3, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P0_3_TCPWM0_LINE_COMPL1)},
    {P0_3, &CY_TCPWM_1_1, CY_PIN_OUT_FUNCTION(P0_3_TCPWM1_LINE_COMPL1)},
    {P0_5, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P0_5_TCPWM0_LINE_COMPL2)},
    {P0_5, &CY_TCPWM_1_2, CY_PIN_OUT_FUNCTION(P0_5_TCPWM1_LINE_COMPL2)},
    {P2_1, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P2_1_TCPWM0_LINE_COMPL3)},
    {P2_1, &CY_TCPWM_1_3, CY_PIN_OUT_FUNCTION(P2_1_TCPWM1_LINE_COMPL3)},
    {P2_3, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P2_3_TCPWM0_LINE_COMPL0)},
    {P2_3, &CY_TCPWM_1_4, CY_PIN_OUT_FUNCTION(P2_3_TCPWM1_LINE_COMPL4)},
    {P2_5, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P2_5_TCPWM0_LINE_COMPL1)},
    {P2_5, &CY_TCPWM_1_5, CY_PIN_OUT_FUNCTION(P2_5_TCPWM1_LINE_COMPL5)},
    {P2_7, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P2_7_TCPWM0_LINE_COMPL2)},
    {P2_7, &CY_TCPWM_1_6, CY_PIN_OUT_FUNCTION(P2_7_TCPWM1_LINE_COMPL6)},
    {P3_1, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P3_1_TCPWM0_LINE_COMPL3)},
    {P3_1, &CY_TCPWM_1_7, CY_PIN_OUT_FUNCTION(P3_1_TCPWM1_LINE_COMPL7)},
    {P5_1, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P5_1_TCPWM0_LINE_COMPL0)},
    {P5_1, &CY_TCPWM_1_0, CY_PIN_OUT_FUNCTION(P5_1_TCPWM1_LINE_COMPL0)},
    {P5_7, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P5_7_TCPWM0_LINE_COMPL1)},
    {P5_7, &CY_TCPWM_1_1, CY_PIN_OUT_FUNCTION(P5_7_TCPWM1_LINE_COMPL1)},
    {P6_3, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P6_3_TCPWM0_LINE_COMPL3)},
    {P6_3, &CY_TCPWM_1_3, CY_PIN_OUT_FUNCTION(P6_3_TCPWM1_LINE_COMPL3)},
    {P6_5, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P6_5_TCPWM0_LINE_COMPL0)},
    {P6_5, &CY_TCPWM_1_4, CY_PIN_OUT_FUNCTION(P6_5_TCPWM1_LINE_COMPL4)},
    {P6_7, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P6_7_TCPWM0_LINE_COMPL1)},
    {P6_7, &CY_TCPWM_1_5, CY_PIN_OUT_FUNCTION(P6_7_TCPWM1_LINE_COMPL5)},
    {P7_1, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P7_1_TCPWM0_LINE_COMPL2)},
    {P7_1, &CY_TCPWM_1_6, CY_PIN_OUT_FUNCTION(P7_1_TCPWM1_LINE_COMPL6)},
    {P7_3, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P7_3_TCPWM0_LINE_COMPL3)},
    {P7_3, &CY_TCPWM_1_7, CY_PIN_OUT_FUNCTION(P7_3_TCPWM1_LINE_COMPL7)},
    {P7_7, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P7_7_TCPWM0_LINE_COMPL1)},
    {P7_7, &CY_TCPWM_1_1, CY_PIN_OUT_FUNCTION(P7_7_TCPWM1_LINE_COMPL1)},
    {P8_1, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P8_1_TCPWM0_LINE_COMPL2)},
    {P8_1, &CY_TCPWM_1_2, CY_PIN_OUT_FUNCTION(P8_1_TCPWM1_LINE_COMPL2)},
    {P9_1, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P9_1_TCPWM0_LINE_COMPL0)},
    {P9_1, &CY_TCPWM_1_4, CY_PIN_OUT_FUNCTION(P9_1_TCPWM1_LINE_COMPL4)},
    {P9_3, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P9_3_TCPWM0_LINE_COMPL1)},
    {P9_3, &CY_TCPWM_1_5, CY_PIN_OUT_FUNCTION(P9_3_TCPWM1_LINE_COMPL5)},
    {P10_1, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P10_1_TCPWM0_LINE_COMPL2)},
    {P10_1, &CY_TCPWM_1_6, CY_PIN_OUT_FUNCTION(P10_1_TCPWM1_LINE_COMPL6)},
    {P10_3, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P10_3_TCPWM0_LINE_COMPL3)},
    {P10_3, &CY_TCPWM_1_7, CY_PIN_OUT_FUNCTION(P10_3_TCPWM1_LINE_COMPL7)},
    {P10_5, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P10_5_TCPWM0_LINE_COMPL0)},
    {P10_5, &CY_TCPWM_1_0, CY_PIN_OUT_FUNCTION(P10_5_TCPWM1_LINE_COMPL0)},
    {P11_1, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P11_1_TCPWM0_LINE_COMPL2)},
    {P11_1, &CY_TCPWM_1_2, CY_PIN_OUT_FUNCTION(P11_1_TCPWM1_LINE_COMPL2)},
    {P11_3, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P11_3_TCPWM0_LINE_COMPL3)},
    {P11_3, &CY_TCPWM_1_3, CY_PIN_OUT_FUNCTION(P11_3_TCPWM1_LINE_COMPL3)},
    {P11_5, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P11_5_TCPWM0_LINE_COMPL0)},
    {P11_5, &CY_TCPWM_1_4, CY_PIN_OUT_FUNCTION(P11_5_TCPWM1_LINE_COMPL4)},
    {P11_7, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P11_7_TCPWM0_LINE_COMPL1)},
    {P11_7, &CY_TCPWM_1_5, CY_PIN_OUT_FUNCTION(P11_7_TCPWM1_LINE_COMPL5)},
    {P12_7, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P12_7_TCPWM0_LINE_COMPL3)},
    {P12_7, &CY_TCPWM_1_7, CY_PIN_OUT_FUNCTION(P12_7_TCPWM1_LINE_COMPL7)},
};

#endif
