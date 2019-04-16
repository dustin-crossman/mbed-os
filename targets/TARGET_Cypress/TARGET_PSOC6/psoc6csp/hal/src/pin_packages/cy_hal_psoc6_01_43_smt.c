/***************************************************************************//**
* \file cy_hal_psoc6_01_43_smt.c
*
* \brief
* PSoC6_01 device GPIO HAL header for 43-SMT package
*
* \note
* Generator version: 1.4.7041.28135
*
********************************************************************************
* \copyright
* Copyright 2016-2019, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_device_headers.h"
#include "cy_hal_hw_types.h"

#if defined(_GPIO_PSOC6_01_43_SMT_H_)
#include "pin_packages/cy_hal_psoc6_01_43_smt.h"

/* Hardware Blocks */
static const cy_resource_inst_t CY_I2S_0 = { CY_RSC_I2S, 0, 0 };
static const cy_resource_inst_t CY_PDM_0 = { CY_RSC_PDM, 0, 0 };
static const cy_resource_inst_t CY_OPAMP_0 = { CY_RSC_OPAMP, 0, 0 };
static const cy_resource_inst_t CY_SCB_0 = { CY_RSC_SCB, 0, 0 };
static const cy_resource_inst_t CY_SCB_1 = { CY_RSC_SCB, 1, 0 };
static const cy_resource_inst_t CY_SCB_2 = { CY_RSC_SCB, 2, 0 };
static const cy_resource_inst_t CY_SCB_3 = { CY_RSC_SCB, 3, 0 };
static const cy_resource_inst_t CY_SCB_4 = { CY_RSC_SCB, 4, 0 };
static const cy_resource_inst_t CY_SCB_5 = { CY_RSC_SCB, 5, 0 };
static const cy_resource_inst_t CY_SCB_6 = { CY_RSC_SCB, 6, 0 };
static const cy_resource_inst_t CY_SCB_8 = { CY_RSC_SCB, 8, 0 };
static const cy_resource_inst_t CY_TCPWM_0_0 = { CY_RSC_TCPWM, 0, 0 };
static const cy_resource_inst_t CY_TCPWM_0_1 = { CY_RSC_TCPWM, 0, 1 };
static const cy_resource_inst_t CY_TCPWM_0_2 = { CY_RSC_TCPWM, 0, 2 };
static const cy_resource_inst_t CY_TCPWM_0_3 = { CY_RSC_TCPWM, 0, 3 };
static const cy_resource_inst_t CY_TCPWM_0_4 = { CY_RSC_TCPWM, 0, 4 };
static const cy_resource_inst_t CY_TCPWM_0_5 = { CY_RSC_TCPWM, 0, 5 };
static const cy_resource_inst_t CY_TCPWM_0_6 = { CY_RSC_TCPWM, 0, 6 };
static const cy_resource_inst_t CY_TCPWM_0_7 = { CY_RSC_TCPWM, 0, 7 };
static const cy_resource_inst_t CY_TCPWM_1_0 = { CY_RSC_TCPWM, 1, 0 };
static const cy_resource_inst_t CY_TCPWM_1_1 = { CY_RSC_TCPWM, 1, 1 };
static const cy_resource_inst_t CY_TCPWM_1_10 = { CY_RSC_TCPWM, 1, 10 };
static const cy_resource_inst_t CY_TCPWM_1_11 = { CY_RSC_TCPWM, 1, 11 };
static const cy_resource_inst_t CY_TCPWM_1_13 = { CY_RSC_TCPWM, 1, 13 };
static const cy_resource_inst_t CY_TCPWM_1_2 = { CY_RSC_TCPWM, 1, 2 };
static const cy_resource_inst_t CY_TCPWM_1_20 = { CY_RSC_TCPWM, 1, 20 };
static const cy_resource_inst_t CY_TCPWM_1_21 = { CY_RSC_TCPWM, 1, 21 };
static const cy_resource_inst_t CY_TCPWM_1_22 = { CY_RSC_TCPWM, 1, 22 };
static const cy_resource_inst_t CY_TCPWM_1_23 = { CY_RSC_TCPWM, 1, 23 };
static const cy_resource_inst_t CY_TCPWM_1_4 = { CY_RSC_TCPWM, 1, 4 };
static const cy_resource_inst_t CY_TCPWM_1_5 = { CY_RSC_TCPWM, 1, 5 };
static const cy_resource_inst_t CY_TCPWM_1_6 = { CY_RSC_TCPWM, 1, 6 };
static const cy_resource_inst_t CY_TCPWM_1_7 = { CY_RSC_TCPWM, 1, 7 };
static const cy_resource_inst_t CY_TCPWM_1_9 = { CY_RSC_TCPWM, 1, 9 };
static const cy_resource_inst_t CY_TCPWM_1_12 = { CY_RSC_TCPWM, 1, 12 };
static const cy_resource_inst_t CY_TCPWM_1_15 = { CY_RSC_TCPWM, 1, 15 };

/* Pin connections */
/* Connections for: audioss_clk_i2s_if */
const cy_resource_pin_mapping_t cy_pin_map_audioss_clk_i2s_if[1] = {
    {P5_0, &CY_I2S_0, CY_PIN_IN_FUNCTION(P5_0_AUDIOSS_CLK_I2S_IF)},
};

/* Connections for: audioss_pdm_clk */
const cy_resource_pin_mapping_t cy_pin_map_audioss_pdm_clk[1] = {
    {P10_4, &CY_PDM_0, CY_PIN_OUT_FUNCTION(P10_4_AUDIOSS_PDM_CLK)},
};

/* Connections for: audioss_pdm_data */
const cy_resource_pin_mapping_t cy_pin_map_audioss_pdm_data[1] = {
    {P10_5, &CY_PDM_0, CY_PIN_IN_FUNCTION(P10_5_AUDIOSS_PDM_DATA)},
};

/* Connections for: audioss_rx_sck */
const cy_resource_pin_mapping_t cy_pin_map_audioss_rx_sck[1] = {
    {P5_4, &CY_I2S_0, CY_PIN_OUT_FUNCTION(P5_4_AUDIOSS_RX_SCK)},
};

/* Connections for: audioss_rx_sdi */
const cy_resource_pin_mapping_t cy_pin_map_audioss_rx_sdi[1] = {
    {P5_6, &CY_I2S_0, CY_PIN_IN_FUNCTION(P5_6_AUDIOSS_RX_SDI)},
};

/* Connections for: audioss_rx_ws */
const cy_resource_pin_mapping_t cy_pin_map_audioss_rx_ws[1] = {
    {P5_5, &CY_I2S_0, CY_PIN_OUT_FUNCTION(P5_5_AUDIOSS_RX_WS)},
};

/* Connections for: audioss_tx_sck */
const cy_resource_pin_mapping_t cy_pin_map_audioss_tx_sck[1] = {
    {P5_1, &CY_I2S_0, CY_PIN_OUT_FUNCTION(P5_1_AUDIOSS_TX_SCK)},
};

/* Connections for: audioss_tx_sdo */
const cy_resource_pin_mapping_t cy_pin_map_audioss_tx_sdo[1] = {
    {P5_3, &CY_I2S_0, CY_PIN_OUT_FUNCTION(P5_3_AUDIOSS_TX_SDO)},
};

/* Connections for: audioss_tx_ws */
const cy_resource_pin_mapping_t cy_pin_map_audioss_tx_ws[1] = {
    {P5_2, &CY_I2S_0, CY_PIN_OUT_FUNCTION(P5_2_AUDIOSS_TX_WS)},
};

/* Connections for: pass_dsi_ctb_cmp0 */
const cy_resource_pin_mapping_t cy_pin_map_pass_dsi_ctb_cmp0[1] = {
    {P9_2, &CY_OPAMP_0, CY_PIN_OUT_FUNCTION(P9_2_PASS_DSI_CTB_CMP0)},
};

/* Connections for: pass_dsi_ctb_cmp1 */
const cy_resource_pin_mapping_t cy_pin_map_pass_dsi_ctb_cmp1[1] = {
    {P9_3, &CY_OPAMP_0, CY_PIN_OUT_FUNCTION(P9_3_PASS_DSI_CTB_CMP1)},
};

/* Connections for: scb_i2c_scl */
const cy_resource_pin_mapping_t cy_pin_map_scb_i2c_scl[5] = {
    {P5_0, &CY_SCB_5, CY_PIN_OD_FUNCTION(P5_0_SCB5_I2C_SCL)},
    {P6_4, &CY_SCB_6, CY_PIN_OD_FUNCTION(P6_4_SCB6_I2C_SCL)},
    {P6_4, &CY_SCB_8, CY_PIN_OD_FUNCTION(P6_4_SCB8_I2C_SCL)},
    {P9_0, &CY_SCB_2, CY_PIN_OD_FUNCTION(P9_0_SCB2_I2C_SCL)},
    {P10_0, &CY_SCB_1, CY_PIN_OD_FUNCTION(P10_0_SCB1_I2C_SCL)},
};

/* Connections for: scb_i2c_sda */
const cy_resource_pin_mapping_t cy_pin_map_scb_i2c_sda[6] = {
    {P5_1, &CY_SCB_5, CY_PIN_OD_FUNCTION(P5_1_SCB5_I2C_SDA)},
    {P6_5, &CY_SCB_6, CY_PIN_OD_FUNCTION(P6_5_SCB6_I2C_SDA)},
    {P6_5, &CY_SCB_8, CY_PIN_OD_FUNCTION(P6_5_SCB8_I2C_SDA)},
    {P7_1, &CY_SCB_4, CY_PIN_OD_FUNCTION(P7_1_SCB4_I2C_SDA)},
    {P9_1, &CY_SCB_2, CY_PIN_OD_FUNCTION(P9_1_SCB2_I2C_SDA)},
    {P10_1, &CY_SCB_1, CY_PIN_OD_FUNCTION(P10_1_SCB1_I2C_SDA)},
};

/* Connections for: scb_spi_clk */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_clk[9] = {
    {P0_4, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_4_SCB0_SPI_CLK)},
    {P5_2, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_2_SCB5_SPI_CLK)},
    {P6_2, &CY_SCB_3, CY_PIN_OUT_FUNCTION(P6_2_SCB3_SPI_CLK)},
    {P6_2, &CY_SCB_8, CY_PIN_OUT_FUNCTION(P6_2_SCB8_SPI_CLK)},
    {P6_6, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P6_6_SCB6_SPI_CLK)},
    {P6_6, &CY_SCB_8, CY_PIN_OUT_FUNCTION(P6_6_SCB8_SPI_CLK)},
    {P7_2, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P7_2_SCB4_SPI_CLK)},
    {P9_2, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_2_SCB2_SPI_CLK)},
    {P10_2, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_2_SCB1_SPI_CLK)},
};

/* Connections for: scb_spi_miso */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_miso[6] = {
    {P5_1, &CY_SCB_5, CY_PIN_IN_FUNCTION(P5_1_SCB5_SPI_MISO)},
    {P6_5, &CY_SCB_6, CY_PIN_IN_FUNCTION(P6_5_SCB6_SPI_MISO)},
    {P6_5, &CY_SCB_8, CY_PIN_IN_FUNCTION(P6_5_SCB8_SPI_MISO)},
    {P7_1, &CY_SCB_4, CY_PIN_IN_FUNCTION(P7_1_SCB4_SPI_MISO)},
    {P9_1, &CY_SCB_2, CY_PIN_IN_FUNCTION(P9_1_SCB2_SPI_MISO)},
    {P10_1, &CY_SCB_1, CY_PIN_IN_FUNCTION(P10_1_SCB1_SPI_MISO)},
};

/* Connections for: scb_spi_mosi */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_mosi[5] = {
    {P5_0, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_0_SCB5_SPI_MOSI)},
    {P6_4, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P6_4_SCB6_SPI_MOSI)},
    {P6_4, &CY_SCB_8, CY_PIN_OUT_FUNCTION(P6_4_SCB8_SPI_MOSI)},
    {P9_0, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_0_SCB2_SPI_MOSI)},
    {P10_0, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_0_SCB1_SPI_MOSI)},
};

/* Connections for: scb_spi_select0 */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select0[8] = {
    {P0_5, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_5_SCB0_SPI_SELECT0)},
    {P5_3, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_3_SCB5_SPI_SELECT0)},
    {P6_3, &CY_SCB_3, CY_PIN_OUT_FUNCTION(P6_3_SCB3_SPI_SELECT0)},
    {P6_3, &CY_SCB_8, CY_PIN_OUT_FUNCTION(P6_3_SCB8_SPI_SELECT0)},
    {P6_7, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P6_7_SCB6_SPI_SELECT0)},
    {P6_7, &CY_SCB_8, CY_PIN_OUT_FUNCTION(P6_7_SCB8_SPI_SELECT0)},
    {P9_3, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_3_SCB2_SPI_SELECT0)},
    {P10_3, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_3_SCB1_SPI_SELECT0)},
};

/* Connections for: scb_spi_select1 */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select1[5] = {
    {P0_0, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_0_SCB0_SPI_SELECT1)},
    {P5_4, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_4_SCB5_SPI_SELECT1)},
    {P7_7, &CY_SCB_3, CY_PIN_OUT_FUNCTION(P7_7_SCB3_SPI_SELECT1)},
    {P9_4, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_4_SCB2_SPI_SELECT1)},
    {P10_4, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_4_SCB1_SPI_SELECT1)},
};

/* Connections for: scb_spi_select2 */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select2[4] = {
    {P0_1, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_1_SCB0_SPI_SELECT2)},
    {P5_5, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_5_SCB5_SPI_SELECT2)},
    {P9_5, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_5_SCB2_SPI_SELECT2)},
    {P10_5, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_5_SCB1_SPI_SELECT2)},
};

/* Connections for: scb_spi_select3 */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select3[4] = {
    {P5_6, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_6_SCB5_SPI_SELECT3)},
    {P9_6, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_6_SCB2_SPI_SELECT3)},
    {P10_6, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_6_SCB1_SPI_SELECT3)},
    {P12_6, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P12_6_SCB6_SPI_SELECT3)},
};

/* Connections for: scb_uart_cts */
const cy_resource_pin_mapping_t cy_pin_map_scb_uart_cts[6] = {
    {P0_5, &CY_SCB_0, CY_PIN_IN_FUNCTION(P0_5_SCB0_UART_CTS)},
    {P5_3, &CY_SCB_5, CY_PIN_IN_FUNCTION(P5_3_SCB5_UART_CTS)},
    {P6_3, &CY_SCB_3, CY_PIN_IN_FUNCTION(P6_3_SCB3_UART_CTS)},
    {P6_7, &CY_SCB_6, CY_PIN_IN_FUNCTION(P6_7_SCB6_UART_CTS)},
    {P9_3, &CY_SCB_2, CY_PIN_IN_FUNCTION(P9_3_SCB2_UART_CTS)},
    {P10_3, &CY_SCB_1, CY_PIN_IN_FUNCTION(P10_3_SCB1_UART_CTS)},
};

/* Connections for: scb_uart_rts */
const cy_resource_pin_mapping_t cy_pin_map_scb_uart_rts[7] = {
    {P0_4, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_4_SCB0_UART_RTS)},
    {P5_2, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_2_SCB5_UART_RTS)},
    {P6_2, &CY_SCB_3, CY_PIN_OUT_FUNCTION(P6_2_SCB3_UART_RTS)},
    {P6_6, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P6_6_SCB6_UART_RTS)},
    {P7_2, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P7_2_SCB4_UART_RTS)},
    {P9_2, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_2_SCB2_UART_RTS)},
    {P10_2, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_2_SCB1_UART_RTS)},
};

/* Connections for: scb_uart_rx */
const cy_resource_pin_mapping_t cy_pin_map_scb_uart_rx[4] = {
    {P5_0, &CY_SCB_5, CY_PIN_IN_FUNCTION(P5_0_SCB5_UART_RX)},
    {P6_4, &CY_SCB_6, CY_PIN_IN_FUNCTION(P6_4_SCB6_UART_RX)},
    {P9_0, &CY_SCB_2, CY_PIN_IN_FUNCTION(P9_0_SCB2_UART_RX)},
    {P10_0, &CY_SCB_1, CY_PIN_IN_FUNCTION(P10_0_SCB1_UART_RX)},
};

/* Connections for: scb_uart_tx */
const cy_resource_pin_mapping_t cy_pin_map_scb_uart_tx[5] = {
    {P5_1, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_1_SCB5_UART_TX)},
    {P6_5, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P6_5_SCB6_UART_TX)},
    {P7_1, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P7_1_SCB4_UART_TX)},
    {P9_1, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_1_SCB2_UART_TX)},
    {P10_1, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_1_SCB1_UART_TX)},
};

/* Connections for: tcpwm_line */
const cy_resource_pin_mapping_t cy_pin_map_tcpwm_line[38] = {
    {P0_0, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P0_0_TCPWM0_LINE0)},
    {P0_0, &CY_TCPWM_1_0, CY_PIN_OUT_FUNCTION(P0_0_TCPWM1_LINE0)},
    {P0_4, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P0_4_TCPWM0_LINE2)},
    {P0_4, &CY_TCPWM_1_2, CY_PIN_OUT_FUNCTION(P0_4_TCPWM1_LINE2)},
    {P5_0, &CY_TCPWM_0_4, CY_PIN_OUT_FUNCTION(P5_0_TCPWM0_LINE4)},
    {P5_0, &CY_TCPWM_1_4, CY_PIN_OUT_FUNCTION(P5_0_TCPWM1_LINE4)},
    {P5_2, &CY_TCPWM_0_5, CY_PIN_OUT_FUNCTION(P5_2_TCPWM0_LINE5)},
    {P5_2, &CY_TCPWM_1_5, CY_PIN_OUT_FUNCTION(P5_2_TCPWM1_LINE5)},
    {P5_4, &CY_TCPWM_0_6, CY_PIN_OUT_FUNCTION(P5_4_TCPWM0_LINE6)},
    {P5_4, &CY_TCPWM_1_6, CY_PIN_OUT_FUNCTION(P5_4_TCPWM1_LINE6)},
    {P5_6, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P5_6_TCPWM0_LINE7)},
    {P5_6, &CY_TCPWM_1_7, CY_PIN_OUT_FUNCTION(P5_6_TCPWM1_LINE7)},
    {P6_2, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P6_2_TCPWM0_LINE1)},
    {P6_2, &CY_TCPWM_1_9, CY_PIN_OUT_FUNCTION(P6_2_TCPWM1_LINE9)},
    {P6_4, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P6_4_TCPWM0_LINE2)},
    {P6_4, &CY_TCPWM_1_10, CY_PIN_OUT_FUNCTION(P6_4_TCPWM1_LINE10)},
    {P6_6, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P6_6_TCPWM0_LINE3)},
    {P6_6, &CY_TCPWM_1_11, CY_PIN_OUT_FUNCTION(P6_6_TCPWM1_LINE11)},
    {P7_2, &CY_TCPWM_0_5, CY_PIN_OUT_FUNCTION(P7_2_TCPWM0_LINE5)},
    {P7_2, &CY_TCPWM_1_13, CY_PIN_OUT_FUNCTION(P7_2_TCPWM1_LINE13)},
    {P9_0, &CY_TCPWM_0_4, CY_PIN_OUT_FUNCTION(P9_0_TCPWM0_LINE4)},
    {P9_0, &CY_TCPWM_1_20, CY_PIN_OUT_FUNCTION(P9_0_TCPWM1_LINE20)},
    {P9_2, &CY_TCPWM_0_5, CY_PIN_OUT_FUNCTION(P9_2_TCPWM0_LINE5)},
    {P9_2, &CY_TCPWM_1_21, CY_PIN_OUT_FUNCTION(P9_2_TCPWM1_LINE21)},
    {P9_4, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P9_4_TCPWM0_LINE7)},
    {P9_4, &CY_TCPWM_1_0, CY_PIN_OUT_FUNCTION(P9_4_TCPWM1_LINE0)},
    {P9_6, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P9_6_TCPWM0_LINE0)},
    {P9_6, &CY_TCPWM_1_1, CY_PIN_OUT_FUNCTION(P9_6_TCPWM1_LINE1)},
    {P10_0, &CY_TCPWM_0_6, CY_PIN_OUT_FUNCTION(P10_0_TCPWM0_LINE6)},
    {P10_0, &CY_TCPWM_1_22, CY_PIN_OUT_FUNCTION(P10_0_TCPWM1_LINE22)},
    {P10_2, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P10_2_TCPWM0_LINE7)},
    {P10_2, &CY_TCPWM_1_23, CY_PIN_OUT_FUNCTION(P10_2_TCPWM1_LINE23)},
    {P10_4, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P10_4_TCPWM0_LINE0)},
    {P10_4, &CY_TCPWM_1_0, CY_PIN_OUT_FUNCTION(P10_4_TCPWM1_LINE0)},
    {P10_6, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P10_6_TCPWM0_LINE1)},
    {P10_6, &CY_TCPWM_1_2, CY_PIN_OUT_FUNCTION(P10_6_TCPWM1_LINE2)},
    {P12_6, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P12_6_TCPWM0_LINE7)},
    {P12_6, &CY_TCPWM_1_7, CY_PIN_OUT_FUNCTION(P12_6_TCPWM1_LINE7)},
};

/* Connections for: tcpwm_line_compl */
const cy_resource_pin_mapping_t cy_pin_map_tcpwm_line_compl[34] = {
    {P0_1, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P0_1_TCPWM0_LINE_COMPL0)},
    {P0_1, &CY_TCPWM_1_0, CY_PIN_OUT_FUNCTION(P0_1_TCPWM1_LINE_COMPL0)},
    {P0_5, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P0_5_TCPWM0_LINE_COMPL2)},
    {P0_5, &CY_TCPWM_1_2, CY_PIN_OUT_FUNCTION(P0_5_TCPWM1_LINE_COMPL2)},
    {P5_1, &CY_TCPWM_0_4, CY_PIN_OUT_FUNCTION(P5_1_TCPWM0_LINE_COMPL4)},
    {P5_1, &CY_TCPWM_1_4, CY_PIN_OUT_FUNCTION(P5_1_TCPWM1_LINE_COMPL4)},
    {P5_3, &CY_TCPWM_0_5, CY_PIN_OUT_FUNCTION(P5_3_TCPWM0_LINE_COMPL5)},
    {P5_3, &CY_TCPWM_1_5, CY_PIN_OUT_FUNCTION(P5_3_TCPWM1_LINE_COMPL5)},
    {P5_5, &CY_TCPWM_0_6, CY_PIN_OUT_FUNCTION(P5_5_TCPWM0_LINE_COMPL6)},
    {P5_5, &CY_TCPWM_1_6, CY_PIN_OUT_FUNCTION(P5_5_TCPWM1_LINE_COMPL6)},
    {P6_3, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P6_3_TCPWM0_LINE_COMPL1)},
    {P6_3, &CY_TCPWM_1_9, CY_PIN_OUT_FUNCTION(P6_3_TCPWM1_LINE_COMPL9)},
    {P6_5, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P6_5_TCPWM0_LINE_COMPL2)},
    {P6_5, &CY_TCPWM_1_10, CY_PIN_OUT_FUNCTION(P6_5_TCPWM1_LINE_COMPL10)},
    {P6_7, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P6_7_TCPWM0_LINE_COMPL3)},
    {P6_7, &CY_TCPWM_1_11, CY_PIN_OUT_FUNCTION(P6_7_TCPWM1_LINE_COMPL11)},
    {P7_1, &CY_TCPWM_0_4, CY_PIN_OUT_FUNCTION(P7_1_TCPWM0_LINE_COMPL4)},
    {P7_1, &CY_TCPWM_1_12, CY_PIN_OUT_FUNCTION(P7_1_TCPWM1_LINE_COMPL12)},
    {P7_7, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P7_7_TCPWM0_LINE_COMPL7)},
    {P7_7, &CY_TCPWM_1_15, CY_PIN_OUT_FUNCTION(P7_7_TCPWM1_LINE_COMPL15)},
    {P9_1, &CY_TCPWM_0_4, CY_PIN_OUT_FUNCTION(P9_1_TCPWM0_LINE_COMPL4)},
    {P9_1, &CY_TCPWM_1_20, CY_PIN_OUT_FUNCTION(P9_1_TCPWM1_LINE_COMPL20)},
    {P9_3, &CY_TCPWM_0_5, CY_PIN_OUT_FUNCTION(P9_3_TCPWM0_LINE_COMPL5)},
    {P9_3, &CY_TCPWM_1_21, CY_PIN_OUT_FUNCTION(P9_3_TCPWM1_LINE_COMPL21)},
    {P9_5, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P9_5_TCPWM0_LINE_COMPL7)},
    {P9_5, &CY_TCPWM_1_0, CY_PIN_OUT_FUNCTION(P9_5_TCPWM1_LINE_COMPL0)},
    {P10_1, &CY_TCPWM_0_6, CY_PIN_OUT_FUNCTION(P10_1_TCPWM0_LINE_COMPL6)},
    {P10_1, &CY_TCPWM_1_22, CY_PIN_OUT_FUNCTION(P10_1_TCPWM1_LINE_COMPL22)},
    {P10_3, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P10_3_TCPWM0_LINE_COMPL7)},
    {P10_3, &CY_TCPWM_1_23, CY_PIN_OUT_FUNCTION(P10_3_TCPWM1_LINE_COMPL23)},
    {P10_5, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P10_5_TCPWM0_LINE_COMPL0)},
    {P10_5, &CY_TCPWM_1_0, CY_PIN_OUT_FUNCTION(P10_5_TCPWM1_LINE_COMPL0)},
    {P12_7, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P12_7_TCPWM0_LINE_COMPL7)},
    {P12_7, &CY_TCPWM_1_7, CY_PIN_OUT_FUNCTION(P12_7_TCPWM1_LINE_COMPL7)},
};

#endif
