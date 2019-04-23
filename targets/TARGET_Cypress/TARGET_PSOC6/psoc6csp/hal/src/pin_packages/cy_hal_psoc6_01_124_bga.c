/***************************************************************************//**
* \file cy_hal_psoc6_01_124_bga.c
*
* \brief
* PSoC6_01 device GPIO HAL header for 124-BGA package
*
* \note
* Generator version: 1.4.7041.28135
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

#if defined(_GPIO_PSOC6_01_124_BGA_H_)
#include "pin_packages/cy_hal_psoc6_01_124_bga.h"

/* Hardware Blocks */
static const cy_resource_inst_t CY_I2S_0 = { CY_RSC_I2S, 0, 0 };
static const cy_resource_inst_t CY_PDM_0 = { CY_RSC_PDM, 0, 0 };
static const cy_resource_inst_t CY_BLESS_0 = { CY_RSC_BLESS, 0, 0 };
static const cy_resource_inst_t CY_LPCOMP_0_0 = { CY_RSC_LPCOMP, 0, 0 };
static const cy_resource_inst_t CY_OPAMP_0 = { CY_RSC_OPAMP, 0, 0 };
static const cy_resource_inst_t CY_SCB_0 = { CY_RSC_SCB, 0, 0 };
static const cy_resource_inst_t CY_SCB_1 = { CY_RSC_SCB, 1, 0 };
static const cy_resource_inst_t CY_SCB_2 = { CY_RSC_SCB, 2, 0 };
static const cy_resource_inst_t CY_SCB_3 = { CY_RSC_SCB, 3, 0 };
static const cy_resource_inst_t CY_SCB_4 = { CY_RSC_SCB, 4, 0 };
static const cy_resource_inst_t CY_SCB_5 = { CY_RSC_SCB, 5, 0 };
static const cy_resource_inst_t CY_SCB_6 = { CY_RSC_SCB, 6, 0 };
static const cy_resource_inst_t CY_SCB_7 = { CY_RSC_SCB, 7, 0 };
static const cy_resource_inst_t CY_SCB_8 = { CY_RSC_SCB, 8, 0 };
static const cy_resource_inst_t CY_SMIF_0 = { CY_RSC_SMIF, 0, 0 };
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
static const cy_resource_inst_t CY_TCPWM_1_12 = { CY_RSC_TCPWM, 1, 12 };
static const cy_resource_inst_t CY_TCPWM_1_13 = { CY_RSC_TCPWM, 1, 13 };
static const cy_resource_inst_t CY_TCPWM_1_14 = { CY_RSC_TCPWM, 1, 14 };
static const cy_resource_inst_t CY_TCPWM_1_15 = { CY_RSC_TCPWM, 1, 15 };
static const cy_resource_inst_t CY_TCPWM_1_16 = { CY_RSC_TCPWM, 1, 16 };
static const cy_resource_inst_t CY_TCPWM_1_17 = { CY_RSC_TCPWM, 1, 17 };
static const cy_resource_inst_t CY_TCPWM_1_18 = { CY_RSC_TCPWM, 1, 18 };
static const cy_resource_inst_t CY_TCPWM_1_19 = { CY_RSC_TCPWM, 1, 19 };
static const cy_resource_inst_t CY_TCPWM_1_2 = { CY_RSC_TCPWM, 1, 2 };
static const cy_resource_inst_t CY_TCPWM_1_20 = { CY_RSC_TCPWM, 1, 20 };
static const cy_resource_inst_t CY_TCPWM_1_21 = { CY_RSC_TCPWM, 1, 21 };
static const cy_resource_inst_t CY_TCPWM_1_22 = { CY_RSC_TCPWM, 1, 22 };
static const cy_resource_inst_t CY_TCPWM_1_23 = { CY_RSC_TCPWM, 1, 23 };
static const cy_resource_inst_t CY_TCPWM_1_3 = { CY_RSC_TCPWM, 1, 3 };
static const cy_resource_inst_t CY_TCPWM_1_4 = { CY_RSC_TCPWM, 1, 4 };
static const cy_resource_inst_t CY_TCPWM_1_5 = { CY_RSC_TCPWM, 1, 5 };
static const cy_resource_inst_t CY_TCPWM_1_6 = { CY_RSC_TCPWM, 1, 6 };
static const cy_resource_inst_t CY_TCPWM_1_7 = { CY_RSC_TCPWM, 1, 7 };
static const cy_resource_inst_t CY_TCPWM_1_8 = { CY_RSC_TCPWM, 1, 8 };
static const cy_resource_inst_t CY_TCPWM_1_9 = { CY_RSC_TCPWM, 1, 9 };

/* Pin connections */
/* Connections for: audioss_clk_i2s_if */
const cy_resource_pin_mapping_t cy_pin_map_audioss_clk_i2s_if[1] = {
    {P5_0, &CY_I2S_0, CY_PIN_IN_FUNCTION(P5_0_AUDIOSS_CLK_I2S_IF)},
};

/* Connections for: audioss_pdm_clk */
const cy_resource_pin_mapping_t cy_pin_map_audioss_pdm_clk[2] = {
    {P10_4, &CY_PDM_0, CY_PIN_OUT_FUNCTION(P10_4_AUDIOSS_PDM_CLK)},
    {P12_4, &CY_PDM_0, CY_PIN_OUT_FUNCTION(P12_4_AUDIOSS_PDM_CLK)},
};

/* Connections for: audioss_pdm_data */
const cy_resource_pin_mapping_t cy_pin_map_audioss_pdm_data[2] = {
    {P10_5, &CY_PDM_0, CY_PIN_IN_FUNCTION(P10_5_AUDIOSS_PDM_DATA)},
    {P12_5, &CY_PDM_0, CY_PIN_IN_FUNCTION(P12_5_AUDIOSS_PDM_DATA)},
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

/* Connections for: bless_ext_lna_rx_ctl_out */
const cy_resource_pin_mapping_t cy_pin_map_bless_ext_lna_rx_ctl_out[1] = {
    {P7_4, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P7_4_BLESS_EXT_LNA_RX_CTL_OUT)},
};

/* Connections for: bless_ext_pa_lna_chip_en_out */
const cy_resource_pin_mapping_t cy_pin_map_bless_ext_pa_lna_chip_en_out[1] = {
    {P7_6, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P7_6_BLESS_EXT_PA_LNA_CHIP_EN_OUT)},
};

/* Connections for: bless_ext_pa_tx_ctl_out */
const cy_resource_pin_mapping_t cy_pin_map_bless_ext_pa_tx_ctl_out[1] = {
    {P7_5, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P7_5_BLESS_EXT_PA_TX_CTL_OUT)},
};

/* Connections for: bless_mxd_act_bpktctl */
const cy_resource_pin_mapping_t cy_pin_map_bless_mxd_act_bpktctl[1] = {
    {P3_3, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P3_3_BLESS_MXD_ACT_BPKTCTL)},
};

/* Connections for: bless_mxd_act_dbus_rx_en */
const cy_resource_pin_mapping_t cy_pin_map_bless_mxd_act_dbus_rx_en[1] = {
    {P3_1, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P3_1_BLESS_MXD_ACT_DBUS_RX_EN)},
};

/* Connections for: bless_mxd_act_dbus_tx_en */
const cy_resource_pin_mapping_t cy_pin_map_bless_mxd_act_dbus_tx_en[1] = {
    {P3_2, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P3_2_BLESS_MXD_ACT_DBUS_TX_EN)},
};

/* Connections for: bless_mxd_act_txd_rxd */
const cy_resource_pin_mapping_t cy_pin_map_bless_mxd_act_txd_rxd[1] = {
    {P3_4, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P3_4_BLESS_MXD_ACT_TXD_RXD)},
};

/* Connections for: bless_mxd_dpslp_act_ldo_en */
const cy_resource_pin_mapping_t cy_pin_map_bless_mxd_dpslp_act_ldo_en[1] = {
    {P2_6, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P2_6_BLESS_MXD_DPSLP_ACT_LDO_EN)},
};

/* Connections for: bless_mxd_dpslp_buck_en */
const cy_resource_pin_mapping_t cy_pin_map_bless_mxd_dpslp_buck_en[1] = {
    {P2_2, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P2_2_BLESS_MXD_DPSLP_BUCK_EN)},
};

/* Connections for: bless_mxd_dpslp_clk_en */
const cy_resource_pin_mapping_t cy_pin_map_bless_mxd_dpslp_clk_en[1] = {
    {P2_4, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P2_4_BLESS_MXD_DPSLP_CLK_EN)},
};

/* Connections for: bless_mxd_dpslp_dig_ldo_en */
const cy_resource_pin_mapping_t cy_pin_map_bless_mxd_dpslp_dig_ldo_en[1] = {
    {P3_0, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P3_0_BLESS_MXD_DPSLP_DIG_LDO_EN)},
};

/* Connections for: bless_mxd_dpslp_isolate_n */
const cy_resource_pin_mapping_t cy_pin_map_bless_mxd_dpslp_isolate_n[1] = {
    {P2_5, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P2_5_BLESS_MXD_DPSLP_ISOLATE_N)},
};

/* Connections for: bless_mxd_dpslp_rcb_clk */
const cy_resource_pin_mapping_t cy_pin_map_bless_mxd_dpslp_rcb_clk[1] = {
    {P4_0, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P4_0_BLESS_MXD_DPSLP_RCB_CLK)},
};

/* Connections for: bless_mxd_dpslp_rcb_data */
const cy_resource_pin_mapping_t cy_pin_map_bless_mxd_dpslp_rcb_data[1] = {
    {P3_5, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P3_5_BLESS_MXD_DPSLP_RCB_DATA)},
};

/* Connections for: bless_mxd_dpslp_rcb_le */
const cy_resource_pin_mapping_t cy_pin_map_bless_mxd_dpslp_rcb_le[1] = {
    {P4_1, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P4_1_BLESS_MXD_DPSLP_RCB_LE)},
};

/* Connections for: bless_mxd_dpslp_reset_n */
const cy_resource_pin_mapping_t cy_pin_map_bless_mxd_dpslp_reset_n[1] = {
    {P2_3, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P2_3_BLESS_MXD_DPSLP_RESET_N)},
};

/* Connections for: bless_mxd_dpslp_ret_ldo_ol_hv */
const cy_resource_pin_mapping_t cy_pin_map_bless_mxd_dpslp_ret_ldo_ol_hv[1] = {
    {P2_1, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P2_1_BLESS_MXD_DPSLP_RET_LDO_OL_HV)},
};

/* Connections for: bless_mxd_dpslp_ret_switch_hv */
const cy_resource_pin_mapping_t cy_pin_map_bless_mxd_dpslp_ret_switch_hv[1] = {
    {P2_0, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P2_0_BLESS_MXD_DPSLP_RET_SWITCH_HV)},
};

/* Connections for: bless_mxd_dpslp_xtal_en */
const cy_resource_pin_mapping_t cy_pin_map_bless_mxd_dpslp_xtal_en[1] = {
    {P2_7, &CY_BLESS_0, CY_PIN_OUT_FUNCTION(P2_7_BLESS_MXD_DPSLP_XTAL_EN)},
};

/* Connections for: lpcomp_dsi_comp0 */
const cy_resource_pin_mapping_t cy_pin_map_lpcomp_dsi_comp0[1] = {
    {P8_2, &CY_LPCOMP_0_0, CY_PIN_OUT_FUNCTION(P8_2_LPCOMP_DSI_COMP0)},
};

/* Connections for: lpcomp_dsi_comp1 */
const cy_resource_pin_mapping_t cy_pin_map_lpcomp_dsi_comp1[1] = {
    {P8_3, &CY_LPCOMP_0_0, CY_PIN_OUT_FUNCTION(P8_3_LPCOMP_DSI_COMP1)},
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
const cy_resource_pin_mapping_t cy_pin_map_scb_i2c_scl[17] = {
    {P0_2, &CY_SCB_0, CY_PIN_OD_FUNCTION(P0_2_SCB0_I2C_SCL)},
    {P1_0, &CY_SCB_7, CY_PIN_OD_FUNCTION(P1_0_SCB7_I2C_SCL)},
    {P2_0, &CY_SCB_1, CY_PIN_OD_FUNCTION(P2_0_SCB1_I2C_SCL)},
    {P3_0, &CY_SCB_2, CY_PIN_OD_FUNCTION(P3_0_SCB2_I2C_SCL)},
    {P4_0, &CY_SCB_7, CY_PIN_OD_FUNCTION(P4_0_SCB7_I2C_SCL)},
    {P5_0, &CY_SCB_5, CY_PIN_OD_FUNCTION(P5_0_SCB5_I2C_SCL)},
    {P6_0, &CY_SCB_3, CY_PIN_OD_FUNCTION(P6_0_SCB3_I2C_SCL)},
    {P6_0, &CY_SCB_8, CY_PIN_OD_FUNCTION(P6_0_SCB8_I2C_SCL)},
    {P6_4, &CY_SCB_6, CY_PIN_OD_FUNCTION(P6_4_SCB6_I2C_SCL)},
    {P6_4, &CY_SCB_8, CY_PIN_OD_FUNCTION(P6_4_SCB8_I2C_SCL)},
    {P7_0, &CY_SCB_4, CY_PIN_OD_FUNCTION(P7_0_SCB4_I2C_SCL)},
    {P8_0, &CY_SCB_4, CY_PIN_OD_FUNCTION(P8_0_SCB4_I2C_SCL)},
    {P9_0, &CY_SCB_2, CY_PIN_OD_FUNCTION(P9_0_SCB2_I2C_SCL)},
    {P10_0, &CY_SCB_1, CY_PIN_OD_FUNCTION(P10_0_SCB1_I2C_SCL)},
    {P11_0, &CY_SCB_5, CY_PIN_OD_FUNCTION(P11_0_SCB5_I2C_SCL)},
    {P12_0, &CY_SCB_6, CY_PIN_OD_FUNCTION(P12_0_SCB6_I2C_SCL)},
    {P13_0, &CY_SCB_6, CY_PIN_OD_FUNCTION(P13_0_SCB6_I2C_SCL)},
};

/* Connections for: scb_i2c_sda */
const cy_resource_pin_mapping_t cy_pin_map_scb_i2c_sda[17] = {
    {P0_3, &CY_SCB_0, CY_PIN_OD_FUNCTION(P0_3_SCB0_I2C_SDA)},
    {P1_1, &CY_SCB_7, CY_PIN_OD_FUNCTION(P1_1_SCB7_I2C_SDA)},
    {P2_1, &CY_SCB_1, CY_PIN_OD_FUNCTION(P2_1_SCB1_I2C_SDA)},
    {P3_1, &CY_SCB_2, CY_PIN_OD_FUNCTION(P3_1_SCB2_I2C_SDA)},
    {P4_1, &CY_SCB_7, CY_PIN_OD_FUNCTION(P4_1_SCB7_I2C_SDA)},
    {P5_1, &CY_SCB_5, CY_PIN_OD_FUNCTION(P5_1_SCB5_I2C_SDA)},
    {P6_1, &CY_SCB_3, CY_PIN_OD_FUNCTION(P6_1_SCB3_I2C_SDA)},
    {P6_1, &CY_SCB_8, CY_PIN_OD_FUNCTION(P6_1_SCB8_I2C_SDA)},
    {P6_5, &CY_SCB_6, CY_PIN_OD_FUNCTION(P6_5_SCB6_I2C_SDA)},
    {P6_5, &CY_SCB_8, CY_PIN_OD_FUNCTION(P6_5_SCB8_I2C_SDA)},
    {P7_1, &CY_SCB_4, CY_PIN_OD_FUNCTION(P7_1_SCB4_I2C_SDA)},
    {P8_1, &CY_SCB_4, CY_PIN_OD_FUNCTION(P8_1_SCB4_I2C_SDA)},
    {P9_1, &CY_SCB_2, CY_PIN_OD_FUNCTION(P9_1_SCB2_I2C_SDA)},
    {P10_1, &CY_SCB_1, CY_PIN_OD_FUNCTION(P10_1_SCB1_I2C_SDA)},
    {P11_1, &CY_SCB_5, CY_PIN_OD_FUNCTION(P11_1_SCB5_I2C_SDA)},
    {P12_1, &CY_SCB_6, CY_PIN_OD_FUNCTION(P12_1_SCB6_I2C_SDA)},
    {P13_1, &CY_SCB_6, CY_PIN_OD_FUNCTION(P13_1_SCB6_I2C_SDA)},
};

/* Connections for: scb_spi_clk */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_clk[16] = {
    {P0_4, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_4_SCB0_SPI_CLK)},
    {P1_2, &CY_SCB_7, CY_PIN_OUT_FUNCTION(P1_2_SCB7_SPI_CLK)},
    {P2_2, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P2_2_SCB1_SPI_CLK)},
    {P3_2, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P3_2_SCB2_SPI_CLK)},
    {P5_2, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_2_SCB5_SPI_CLK)},
    {P6_2, &CY_SCB_3, CY_PIN_OUT_FUNCTION(P6_2_SCB3_SPI_CLK)},
    {P6_2, &CY_SCB_8, CY_PIN_OUT_FUNCTION(P6_2_SCB8_SPI_CLK)},
    {P6_6, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P6_6_SCB6_SPI_CLK)},
    {P6_6, &CY_SCB_8, CY_PIN_OUT_FUNCTION(P6_6_SCB8_SPI_CLK)},
    {P7_2, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P7_2_SCB4_SPI_CLK)},
    {P8_2, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P8_2_SCB4_SPI_CLK)},
    {P9_2, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_2_SCB2_SPI_CLK)},
    {P10_2, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_2_SCB1_SPI_CLK)},
    {P11_2, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P11_2_SCB5_SPI_CLK)},
    {P12_2, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P12_2_SCB6_SPI_CLK)},
    {P13_2, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P13_2_SCB6_SPI_CLK)},
};

/* Connections for: scb_spi_miso */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_miso[17] = {
    {P0_3, &CY_SCB_0, CY_PIN_IN_FUNCTION(P0_3_SCB0_SPI_MISO)},
    {P1_1, &CY_SCB_7, CY_PIN_IN_FUNCTION(P1_1_SCB7_SPI_MISO)},
    {P2_1, &CY_SCB_1, CY_PIN_IN_FUNCTION(P2_1_SCB1_SPI_MISO)},
    {P3_1, &CY_SCB_2, CY_PIN_IN_FUNCTION(P3_1_SCB2_SPI_MISO)},
    {P4_1, &CY_SCB_7, CY_PIN_IN_FUNCTION(P4_1_SCB7_SPI_MISO)},
    {P5_1, &CY_SCB_5, CY_PIN_IN_FUNCTION(P5_1_SCB5_SPI_MISO)},
    {P6_1, &CY_SCB_3, CY_PIN_IN_FUNCTION(P6_1_SCB3_SPI_MISO)},
    {P6_1, &CY_SCB_8, CY_PIN_IN_FUNCTION(P6_1_SCB8_SPI_MISO)},
    {P6_5, &CY_SCB_6, CY_PIN_IN_FUNCTION(P6_5_SCB6_SPI_MISO)},
    {P6_5, &CY_SCB_8, CY_PIN_IN_FUNCTION(P6_5_SCB8_SPI_MISO)},
    {P7_1, &CY_SCB_4, CY_PIN_IN_FUNCTION(P7_1_SCB4_SPI_MISO)},
    {P8_1, &CY_SCB_4, CY_PIN_IN_FUNCTION(P8_1_SCB4_SPI_MISO)},
    {P9_1, &CY_SCB_2, CY_PIN_IN_FUNCTION(P9_1_SCB2_SPI_MISO)},
    {P10_1, &CY_SCB_1, CY_PIN_IN_FUNCTION(P10_1_SCB1_SPI_MISO)},
    {P11_1, &CY_SCB_5, CY_PIN_IN_FUNCTION(P11_1_SCB5_SPI_MISO)},
    {P12_1, &CY_SCB_6, CY_PIN_IN_FUNCTION(P12_1_SCB6_SPI_MISO)},
    {P13_1, &CY_SCB_6, CY_PIN_IN_FUNCTION(P13_1_SCB6_SPI_MISO)},
};

/* Connections for: scb_spi_mosi */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_mosi[17] = {
    {P0_2, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_2_SCB0_SPI_MOSI)},
    {P1_0, &CY_SCB_7, CY_PIN_OUT_FUNCTION(P1_0_SCB7_SPI_MOSI)},
    {P2_0, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P2_0_SCB1_SPI_MOSI)},
    {P3_0, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P3_0_SCB2_SPI_MOSI)},
    {P4_0, &CY_SCB_7, CY_PIN_OUT_FUNCTION(P4_0_SCB7_SPI_MOSI)},
    {P5_0, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_0_SCB5_SPI_MOSI)},
    {P6_0, &CY_SCB_3, CY_PIN_OUT_FUNCTION(P6_0_SCB3_SPI_MOSI)},
    {P6_0, &CY_SCB_8, CY_PIN_OUT_FUNCTION(P6_0_SCB8_SPI_MOSI)},
    {P6_4, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P6_4_SCB6_SPI_MOSI)},
    {P6_4, &CY_SCB_8, CY_PIN_OUT_FUNCTION(P6_4_SCB8_SPI_MOSI)},
    {P7_0, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P7_0_SCB4_SPI_MOSI)},
    {P8_0, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P8_0_SCB4_SPI_MOSI)},
    {P9_0, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_0_SCB2_SPI_MOSI)},
    {P10_0, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_0_SCB1_SPI_MOSI)},
    {P11_0, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P11_0_SCB5_SPI_MOSI)},
    {P12_0, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P12_0_SCB6_SPI_MOSI)},
    {P13_0, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P13_0_SCB6_SPI_MOSI)},
};

/* Connections for: scb_spi_select0 */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select0[16] = {
    {P0_5, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_5_SCB0_SPI_SELECT0)},
    {P1_3, &CY_SCB_7, CY_PIN_OUT_FUNCTION(P1_3_SCB7_SPI_SELECT0)},
    {P2_3, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P2_3_SCB1_SPI_SELECT0)},
    {P3_3, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P3_3_SCB2_SPI_SELECT0)},
    {P5_3, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_3_SCB5_SPI_SELECT0)},
    {P6_3, &CY_SCB_3, CY_PIN_OUT_FUNCTION(P6_3_SCB3_SPI_SELECT0)},
    {P6_3, &CY_SCB_8, CY_PIN_OUT_FUNCTION(P6_3_SCB8_SPI_SELECT0)},
    {P6_7, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P6_7_SCB6_SPI_SELECT0)},
    {P6_7, &CY_SCB_8, CY_PIN_OUT_FUNCTION(P6_7_SCB8_SPI_SELECT0)},
    {P7_3, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P7_3_SCB4_SPI_SELECT0)},
    {P8_3, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P8_3_SCB4_SPI_SELECT0)},
    {P9_3, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_3_SCB2_SPI_SELECT0)},
    {P10_3, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_3_SCB1_SPI_SELECT0)},
    {P11_3, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P11_3_SCB5_SPI_SELECT0)},
    {P12_3, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P12_3_SCB6_SPI_SELECT0)},
    {P13_3, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P13_3_SCB6_SPI_SELECT0)},
};

/* Connections for: scb_spi_select1 */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select1[13] = {
    {P0_0, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_0_SCB0_SPI_SELECT1)},
    {P1_4, &CY_SCB_7, CY_PIN_OUT_FUNCTION(P1_4_SCB7_SPI_SELECT1)},
    {P2_4, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P2_4_SCB1_SPI_SELECT1)},
    {P3_4, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P3_4_SCB2_SPI_SELECT1)},
    {P5_4, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_4_SCB5_SPI_SELECT1)},
    {P7_4, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P7_4_SCB4_SPI_SELECT1)},
    {P7_7, &CY_SCB_3, CY_PIN_OUT_FUNCTION(P7_7_SCB3_SPI_SELECT1)},
    {P8_4, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P8_4_SCB4_SPI_SELECT1)},
    {P9_4, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_4_SCB2_SPI_SELECT1)},
    {P10_4, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_4_SCB1_SPI_SELECT1)},
    {P11_4, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P11_4_SCB5_SPI_SELECT1)},
    {P12_4, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P12_4_SCB6_SPI_SELECT1)},
    {P13_4, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P13_4_SCB6_SPI_SELECT1)},
};

/* Connections for: scb_spi_select2 */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select2[13] = {
    {P0_1, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_1_SCB0_SPI_SELECT2)},
    {P1_5, &CY_SCB_7, CY_PIN_OUT_FUNCTION(P1_5_SCB7_SPI_SELECT2)},
    {P2_5, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P2_5_SCB1_SPI_SELECT2)},
    {P3_5, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P3_5_SCB2_SPI_SELECT2)},
    {P5_5, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_5_SCB5_SPI_SELECT2)},
    {P7_5, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P7_5_SCB4_SPI_SELECT2)},
    {P8_5, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P8_5_SCB4_SPI_SELECT2)},
    {P8_7, &CY_SCB_3, CY_PIN_OUT_FUNCTION(P8_7_SCB3_SPI_SELECT2)},
    {P9_5, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_5_SCB2_SPI_SELECT2)},
    {P10_5, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_5_SCB1_SPI_SELECT2)},
    {P11_5, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P11_5_SCB5_SPI_SELECT2)},
    {P12_5, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P12_5_SCB6_SPI_SELECT2)},
    {P13_5, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P13_5_SCB6_SPI_SELECT2)},
};

/* Connections for: scb_spi_select3 */
const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select3[10] = {
    {P2_6, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P2_6_SCB1_SPI_SELECT3)},
    {P5_6, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_6_SCB5_SPI_SELECT3)},
    {P5_7, &CY_SCB_3, CY_PIN_OUT_FUNCTION(P5_7_SCB3_SPI_SELECT3)},
    {P7_6, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P7_6_SCB4_SPI_SELECT3)},
    {P8_6, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P8_6_SCB4_SPI_SELECT3)},
    {P9_6, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_6_SCB2_SPI_SELECT3)},
    {P10_6, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_6_SCB1_SPI_SELECT3)},
    {P11_6, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P11_6_SCB5_SPI_SELECT3)},
    {P12_6, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P12_6_SCB6_SPI_SELECT3)},
    {P13_6, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P13_6_SCB6_SPI_SELECT3)},
};

/* Connections for: scb_uart_cts */
const cy_resource_pin_mapping_t cy_pin_map_scb_uart_cts[14] = {
    {P0_5, &CY_SCB_0, CY_PIN_IN_FUNCTION(P0_5_SCB0_UART_CTS)},
    {P1_3, &CY_SCB_7, CY_PIN_IN_FUNCTION(P1_3_SCB7_UART_CTS)},
    {P2_3, &CY_SCB_1, CY_PIN_IN_FUNCTION(P2_3_SCB1_UART_CTS)},
    {P3_3, &CY_SCB_2, CY_PIN_IN_FUNCTION(P3_3_SCB2_UART_CTS)},
    {P5_3, &CY_SCB_5, CY_PIN_IN_FUNCTION(P5_3_SCB5_UART_CTS)},
    {P6_3, &CY_SCB_3, CY_PIN_IN_FUNCTION(P6_3_SCB3_UART_CTS)},
    {P6_7, &CY_SCB_6, CY_PIN_IN_FUNCTION(P6_7_SCB6_UART_CTS)},
    {P7_3, &CY_SCB_4, CY_PIN_IN_FUNCTION(P7_3_SCB4_UART_CTS)},
    {P8_3, &CY_SCB_4, CY_PIN_IN_FUNCTION(P8_3_SCB4_UART_CTS)},
    {P9_3, &CY_SCB_2, CY_PIN_IN_FUNCTION(P9_3_SCB2_UART_CTS)},
    {P10_3, &CY_SCB_1, CY_PIN_IN_FUNCTION(P10_3_SCB1_UART_CTS)},
    {P11_3, &CY_SCB_5, CY_PIN_IN_FUNCTION(P11_3_SCB5_UART_CTS)},
    {P12_3, &CY_SCB_6, CY_PIN_IN_FUNCTION(P12_3_SCB6_UART_CTS)},
    {P13_3, &CY_SCB_6, CY_PIN_IN_FUNCTION(P13_3_SCB6_UART_CTS)},
};

/* Connections for: scb_uart_rts */
const cy_resource_pin_mapping_t cy_pin_map_scb_uart_rts[14] = {
    {P0_4, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_4_SCB0_UART_RTS)},
    {P1_2, &CY_SCB_7, CY_PIN_OUT_FUNCTION(P1_2_SCB7_UART_RTS)},
    {P2_2, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P2_2_SCB1_UART_RTS)},
    {P3_2, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P3_2_SCB2_UART_RTS)},
    {P5_2, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_2_SCB5_UART_RTS)},
    {P6_2, &CY_SCB_3, CY_PIN_OUT_FUNCTION(P6_2_SCB3_UART_RTS)},
    {P6_6, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P6_6_SCB6_UART_RTS)},
    {P7_2, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P7_2_SCB4_UART_RTS)},
    {P8_2, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P8_2_SCB4_UART_RTS)},
    {P9_2, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_2_SCB2_UART_RTS)},
    {P10_2, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_2_SCB1_UART_RTS)},
    {P11_2, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P11_2_SCB5_UART_RTS)},
    {P12_2, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P12_2_SCB6_UART_RTS)},
    {P13_2, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P13_2_SCB6_UART_RTS)},
};

/* Connections for: scb_uart_rx */
const cy_resource_pin_mapping_t cy_pin_map_scb_uart_rx[15] = {
    {P0_2, &CY_SCB_0, CY_PIN_IN_FUNCTION(P0_2_SCB0_UART_RX)},
    {P1_0, &CY_SCB_7, CY_PIN_IN_FUNCTION(P1_0_SCB7_UART_RX)},
    {P2_0, &CY_SCB_1, CY_PIN_IN_FUNCTION(P2_0_SCB1_UART_RX)},
    {P3_0, &CY_SCB_2, CY_PIN_IN_FUNCTION(P3_0_SCB2_UART_RX)},
    {P4_0, &CY_SCB_7, CY_PIN_IN_FUNCTION(P4_0_SCB7_UART_RX)},
    {P5_0, &CY_SCB_5, CY_PIN_IN_FUNCTION(P5_0_SCB5_UART_RX)},
    {P6_0, &CY_SCB_3, CY_PIN_IN_FUNCTION(P6_0_SCB3_UART_RX)},
    {P6_4, &CY_SCB_6, CY_PIN_IN_FUNCTION(P6_4_SCB6_UART_RX)},
    {P7_0, &CY_SCB_4, CY_PIN_IN_FUNCTION(P7_0_SCB4_UART_RX)},
    {P8_0, &CY_SCB_4, CY_PIN_IN_FUNCTION(P8_0_SCB4_UART_RX)},
    {P9_0, &CY_SCB_2, CY_PIN_IN_FUNCTION(P9_0_SCB2_UART_RX)},
    {P10_0, &CY_SCB_1, CY_PIN_IN_FUNCTION(P10_0_SCB1_UART_RX)},
    {P11_0, &CY_SCB_5, CY_PIN_IN_FUNCTION(P11_0_SCB5_UART_RX)},
    {P12_0, &CY_SCB_6, CY_PIN_IN_FUNCTION(P12_0_SCB6_UART_RX)},
    {P13_0, &CY_SCB_6, CY_PIN_IN_FUNCTION(P13_0_SCB6_UART_RX)},
};

/* Connections for: scb_uart_tx */
const cy_resource_pin_mapping_t cy_pin_map_scb_uart_tx[15] = {
    {P0_3, &CY_SCB_0, CY_PIN_OUT_FUNCTION(P0_3_SCB0_UART_TX)},
    {P1_1, &CY_SCB_7, CY_PIN_OUT_FUNCTION(P1_1_SCB7_UART_TX)},
    {P2_1, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P2_1_SCB1_UART_TX)},
    {P3_1, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P3_1_SCB2_UART_TX)},
    {P4_1, &CY_SCB_7, CY_PIN_OUT_FUNCTION(P4_1_SCB7_UART_TX)},
    {P5_1, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P5_1_SCB5_UART_TX)},
    {P6_1, &CY_SCB_3, CY_PIN_OUT_FUNCTION(P6_1_SCB3_UART_TX)},
    {P6_5, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P6_5_SCB6_UART_TX)},
    {P7_1, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P7_1_SCB4_UART_TX)},
    {P8_1, &CY_SCB_4, CY_PIN_OUT_FUNCTION(P8_1_SCB4_UART_TX)},
    {P9_1, &CY_SCB_2, CY_PIN_OUT_FUNCTION(P9_1_SCB2_UART_TX)},
    {P10_1, &CY_SCB_1, CY_PIN_OUT_FUNCTION(P10_1_SCB1_UART_TX)},
    {P11_1, &CY_SCB_5, CY_PIN_OUT_FUNCTION(P11_1_SCB5_UART_TX)},
    {P12_1, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P12_1_SCB6_UART_TX)},
    {P13_1, &CY_SCB_6, CY_PIN_OUT_FUNCTION(P13_1_SCB6_UART_TX)},
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

/* Connections for: smif_spi_data4 */
const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data4[1] = {
    {P12_0, &CY_SMIF_0, CY_PIN_OUT_BUF_FUNCTION(P12_0_SMIF_SPI_DATA4)},
};

/* Connections for: smif_spi_data5 */
const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data5[1] = {
    {P12_1, &CY_SMIF_0, CY_PIN_OUT_BUF_FUNCTION(P12_1_SMIF_SPI_DATA5)},
};

/* Connections for: smif_spi_data6 */
const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data6[1] = {
    {P12_2, &CY_SMIF_0, CY_PIN_OUT_BUF_FUNCTION(P12_2_SMIF_SPI_DATA6)},
};

/* Connections for: smif_spi_data7 */
const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data7[1] = {
    {P12_3, &CY_SMIF_0, CY_PIN_OUT_BUF_FUNCTION(P12_3_SMIF_SPI_DATA7)},
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

/* Connections for: smif_spi_select3 */
const cy_resource_pin_mapping_t cy_pin_map_smif_spi_select3[1] = {
    {P12_4, &CY_SMIF_0, CY_PIN_OUT_FUNCTION(P12_4_SMIF_SPI_SELECT3)},
};

/* Connections for: tcpwm_line */
const cy_resource_pin_mapping_t cy_pin_map_tcpwm_line[98] = {
    {P0_0, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P0_0_TCPWM0_LINE0)},
    {P0_0, &CY_TCPWM_1_0, CY_PIN_OUT_FUNCTION(P0_0_TCPWM1_LINE0)},
    {P0_2, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P0_2_TCPWM0_LINE1)},
    {P0_2, &CY_TCPWM_1_1, CY_PIN_OUT_FUNCTION(P0_2_TCPWM1_LINE1)},
    {P0_4, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P0_4_TCPWM0_LINE2)},
    {P0_4, &CY_TCPWM_1_2, CY_PIN_OUT_FUNCTION(P0_4_TCPWM1_LINE2)},
    {P1_0, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P1_0_TCPWM0_LINE3)},
    {P1_0, &CY_TCPWM_1_3, CY_PIN_OUT_FUNCTION(P1_0_TCPWM1_LINE3)},
    {P1_2, &CY_TCPWM_0_4, CY_PIN_OUT_FUNCTION(P1_2_TCPWM0_LINE4)},
    {P1_2, &CY_TCPWM_1_12, CY_PIN_OUT_FUNCTION(P1_2_TCPWM1_LINE12)},
    {P1_4, &CY_TCPWM_0_5, CY_PIN_OUT_FUNCTION(P1_4_TCPWM0_LINE5)},
    {P1_4, &CY_TCPWM_1_13, CY_PIN_OUT_FUNCTION(P1_4_TCPWM1_LINE13)},
    {P2_0, &CY_TCPWM_0_6, CY_PIN_OUT_FUNCTION(P2_0_TCPWM0_LINE6)},
    {P2_0, &CY_TCPWM_1_15, CY_PIN_OUT_FUNCTION(P2_0_TCPWM1_LINE15)},
    {P2_2, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P2_2_TCPWM0_LINE7)},
    {P2_2, &CY_TCPWM_1_16, CY_PIN_OUT_FUNCTION(P2_2_TCPWM1_LINE16)},
    {P2_4, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P2_4_TCPWM0_LINE0)},
    {P2_4, &CY_TCPWM_1_17, CY_PIN_OUT_FUNCTION(P2_4_TCPWM1_LINE17)},
    {P2_6, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P2_6_TCPWM0_LINE1)},
    {P2_6, &CY_TCPWM_1_18, CY_PIN_OUT_FUNCTION(P2_6_TCPWM1_LINE18)},
    {P3_0, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P3_0_TCPWM0_LINE2)},
    {P3_0, &CY_TCPWM_1_19, CY_PIN_OUT_FUNCTION(P3_0_TCPWM1_LINE19)},
    {P3_2, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P3_2_TCPWM0_LINE3)},
    {P3_2, &CY_TCPWM_1_20, CY_PIN_OUT_FUNCTION(P3_2_TCPWM1_LINE20)},
    {P3_4, &CY_TCPWM_0_4, CY_PIN_OUT_FUNCTION(P3_4_TCPWM0_LINE4)},
    {P3_4, &CY_TCPWM_1_21, CY_PIN_OUT_FUNCTION(P3_4_TCPWM1_LINE21)},
    {P4_0, &CY_TCPWM_0_5, CY_PIN_OUT_FUNCTION(P4_0_TCPWM0_LINE5)},
    {P4_0, &CY_TCPWM_1_22, CY_PIN_OUT_FUNCTION(P4_0_TCPWM1_LINE22)},
    {P5_0, &CY_TCPWM_0_4, CY_PIN_OUT_FUNCTION(P5_0_TCPWM0_LINE4)},
    {P5_0, &CY_TCPWM_1_4, CY_PIN_OUT_FUNCTION(P5_0_TCPWM1_LINE4)},
    {P5_2, &CY_TCPWM_0_5, CY_PIN_OUT_FUNCTION(P5_2_TCPWM0_LINE5)},
    {P5_2, &CY_TCPWM_1_5, CY_PIN_OUT_FUNCTION(P5_2_TCPWM1_LINE5)},
    {P5_4, &CY_TCPWM_0_6, CY_PIN_OUT_FUNCTION(P5_4_TCPWM0_LINE6)},
    {P5_4, &CY_TCPWM_1_6, CY_PIN_OUT_FUNCTION(P5_4_TCPWM1_LINE6)},
    {P5_6, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P5_6_TCPWM0_LINE7)},
    {P5_6, &CY_TCPWM_1_7, CY_PIN_OUT_FUNCTION(P5_6_TCPWM1_LINE7)},
    {P6_0, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P6_0_TCPWM0_LINE0)},
    {P6_0, &CY_TCPWM_1_8, CY_PIN_OUT_FUNCTION(P6_0_TCPWM1_LINE8)},
    {P6_2, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P6_2_TCPWM0_LINE1)},
    {P6_2, &CY_TCPWM_1_9, CY_PIN_OUT_FUNCTION(P6_2_TCPWM1_LINE9)},
    {P6_4, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P6_4_TCPWM0_LINE2)},
    {P6_4, &CY_TCPWM_1_10, CY_PIN_OUT_FUNCTION(P6_4_TCPWM1_LINE10)},
    {P6_6, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P6_6_TCPWM0_LINE3)},
    {P6_6, &CY_TCPWM_1_11, CY_PIN_OUT_FUNCTION(P6_6_TCPWM1_LINE11)},
    {P7_0, &CY_TCPWM_0_4, CY_PIN_OUT_FUNCTION(P7_0_TCPWM0_LINE4)},
    {P7_0, &CY_TCPWM_1_12, CY_PIN_OUT_FUNCTION(P7_0_TCPWM1_LINE12)},
    {P7_2, &CY_TCPWM_0_5, CY_PIN_OUT_FUNCTION(P7_2_TCPWM0_LINE5)},
    {P7_2, &CY_TCPWM_1_13, CY_PIN_OUT_FUNCTION(P7_2_TCPWM1_LINE13)},
    {P7_4, &CY_TCPWM_0_6, CY_PIN_OUT_FUNCTION(P7_4_TCPWM0_LINE6)},
    {P7_4, &CY_TCPWM_1_14, CY_PIN_OUT_FUNCTION(P7_4_TCPWM1_LINE14)},
    {P7_6, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P7_6_TCPWM0_LINE7)},
    {P7_6, &CY_TCPWM_1_15, CY_PIN_OUT_FUNCTION(P7_6_TCPWM1_LINE15)},
    {P8_0, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P8_0_TCPWM0_LINE0)},
    {P8_0, &CY_TCPWM_1_16, CY_PIN_OUT_FUNCTION(P8_0_TCPWM1_LINE16)},
    {P8_2, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P8_2_TCPWM0_LINE1)},
    {P8_2, &CY_TCPWM_1_17, CY_PIN_OUT_FUNCTION(P8_2_TCPWM1_LINE17)},
    {P8_4, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P8_4_TCPWM0_LINE2)},
    {P8_4, &CY_TCPWM_1_18, CY_PIN_OUT_FUNCTION(P8_4_TCPWM1_LINE18)},
    {P8_6, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P8_6_TCPWM0_LINE3)},
    {P8_6, &CY_TCPWM_1_19, CY_PIN_OUT_FUNCTION(P8_6_TCPWM1_LINE19)},
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
    {P11_0, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P11_0_TCPWM0_LINE1)},
    {P11_0, &CY_TCPWM_1_1, CY_PIN_OUT_FUNCTION(P11_0_TCPWM1_LINE1)},
    {P11_2, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P11_2_TCPWM0_LINE2)},
    {P11_2, &CY_TCPWM_1_2, CY_PIN_OUT_FUNCTION(P11_2_TCPWM1_LINE2)},
    {P11_4, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P11_4_TCPWM0_LINE3)},
    {P11_4, &CY_TCPWM_1_3, CY_PIN_OUT_FUNCTION(P11_4_TCPWM1_LINE3)},
    {P12_0, &CY_TCPWM_0_4, CY_PIN_OUT_FUNCTION(P12_0_TCPWM0_LINE4)},
    {P12_0, &CY_TCPWM_1_4, CY_PIN_OUT_FUNCTION(P12_0_TCPWM1_LINE4)},
    {P12_2, &CY_TCPWM_0_5, CY_PIN_OUT_FUNCTION(P12_2_TCPWM0_LINE5)},
    {P12_2, &CY_TCPWM_1_5, CY_PIN_OUT_FUNCTION(P12_2_TCPWM1_LINE5)},
    {P12_4, &CY_TCPWM_0_6, CY_PIN_OUT_FUNCTION(P12_4_TCPWM0_LINE6)},
    {P12_4, &CY_TCPWM_1_6, CY_PIN_OUT_FUNCTION(P12_4_TCPWM1_LINE6)},
    {P12_6, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P12_6_TCPWM0_LINE7)},
    {P12_6, &CY_TCPWM_1_7, CY_PIN_OUT_FUNCTION(P12_6_TCPWM1_LINE7)},
    {P13_0, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P13_0_TCPWM0_LINE0)},
    {P13_0, &CY_TCPWM_1_8, CY_PIN_OUT_FUNCTION(P13_0_TCPWM1_LINE8)},
    {P13_2, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P13_2_TCPWM0_LINE1)},
    {P13_2, &CY_TCPWM_1_9, CY_PIN_OUT_FUNCTION(P13_2_TCPWM1_LINE9)},
    {P13_4, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P13_4_TCPWM0_LINE2)},
    {P13_4, &CY_TCPWM_1_10, CY_PIN_OUT_FUNCTION(P13_4_TCPWM1_LINE10)},
    {P13_6, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P13_6_TCPWM0_LINE3)},
    {P13_6, &CY_TCPWM_1_11, CY_PIN_OUT_FUNCTION(P13_6_TCPWM1_LINE11)},
};

/* Connections for: tcpwm_line_compl */
const cy_resource_pin_mapping_t cy_pin_map_tcpwm_line_compl[98] = {
    {P0_1, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P0_1_TCPWM0_LINE_COMPL0)},
    {P0_1, &CY_TCPWM_1_0, CY_PIN_OUT_FUNCTION(P0_1_TCPWM1_LINE_COMPL0)},
    {P0_3, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P0_3_TCPWM0_LINE_COMPL1)},
    {P0_3, &CY_TCPWM_1_1, CY_PIN_OUT_FUNCTION(P0_3_TCPWM1_LINE_COMPL1)},
    {P0_5, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P0_5_TCPWM0_LINE_COMPL2)},
    {P0_5, &CY_TCPWM_1_2, CY_PIN_OUT_FUNCTION(P0_5_TCPWM1_LINE_COMPL2)},
    {P1_1, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P1_1_TCPWM0_LINE_COMPL3)},
    {P1_1, &CY_TCPWM_1_3, CY_PIN_OUT_FUNCTION(P1_1_TCPWM1_LINE_COMPL3)},
    {P1_3, &CY_TCPWM_0_4, CY_PIN_OUT_FUNCTION(P1_3_TCPWM0_LINE_COMPL4)},
    {P1_3, &CY_TCPWM_1_12, CY_PIN_OUT_FUNCTION(P1_3_TCPWM1_LINE_COMPL12)},
    {P1_5, &CY_TCPWM_0_5, CY_PIN_OUT_FUNCTION(P1_5_TCPWM0_LINE_COMPL5)},
    {P1_5, &CY_TCPWM_1_14, CY_PIN_OUT_FUNCTION(P1_5_TCPWM1_LINE_COMPL14)},
    {P2_1, &CY_TCPWM_0_6, CY_PIN_OUT_FUNCTION(P2_1_TCPWM0_LINE_COMPL6)},
    {P2_1, &CY_TCPWM_1_15, CY_PIN_OUT_FUNCTION(P2_1_TCPWM1_LINE_COMPL15)},
    {P2_3, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P2_3_TCPWM0_LINE_COMPL7)},
    {P2_3, &CY_TCPWM_1_16, CY_PIN_OUT_FUNCTION(P2_3_TCPWM1_LINE_COMPL16)},
    {P2_5, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P2_5_TCPWM0_LINE_COMPL0)},
    {P2_5, &CY_TCPWM_1_17, CY_PIN_OUT_FUNCTION(P2_5_TCPWM1_LINE_COMPL17)},
    {P2_7, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P2_7_TCPWM0_LINE_COMPL1)},
    {P2_7, &CY_TCPWM_1_18, CY_PIN_OUT_FUNCTION(P2_7_TCPWM1_LINE_COMPL18)},
    {P3_1, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P3_1_TCPWM0_LINE_COMPL2)},
    {P3_1, &CY_TCPWM_1_19, CY_PIN_OUT_FUNCTION(P3_1_TCPWM1_LINE_COMPL19)},
    {P3_3, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P3_3_TCPWM0_LINE_COMPL3)},
    {P3_3, &CY_TCPWM_1_20, CY_PIN_OUT_FUNCTION(P3_3_TCPWM1_LINE_COMPL20)},
    {P3_5, &CY_TCPWM_0_4, CY_PIN_OUT_FUNCTION(P3_5_TCPWM0_LINE_COMPL4)},
    {P3_5, &CY_TCPWM_1_21, CY_PIN_OUT_FUNCTION(P3_5_TCPWM1_LINE_COMPL21)},
    {P4_1, &CY_TCPWM_0_5, CY_PIN_OUT_FUNCTION(P4_1_TCPWM0_LINE_COMPL5)},
    {P4_1, &CY_TCPWM_1_22, CY_PIN_OUT_FUNCTION(P4_1_TCPWM1_LINE_COMPL22)},
    {P5_1, &CY_TCPWM_0_4, CY_PIN_OUT_FUNCTION(P5_1_TCPWM0_LINE_COMPL4)},
    {P5_1, &CY_TCPWM_1_4, CY_PIN_OUT_FUNCTION(P5_1_TCPWM1_LINE_COMPL4)},
    {P5_3, &CY_TCPWM_0_5, CY_PIN_OUT_FUNCTION(P5_3_TCPWM0_LINE_COMPL5)},
    {P5_3, &CY_TCPWM_1_5, CY_PIN_OUT_FUNCTION(P5_3_TCPWM1_LINE_COMPL5)},
    {P5_5, &CY_TCPWM_0_6, CY_PIN_OUT_FUNCTION(P5_5_TCPWM0_LINE_COMPL6)},
    {P5_5, &CY_TCPWM_1_6, CY_PIN_OUT_FUNCTION(P5_5_TCPWM1_LINE_COMPL6)},
    {P5_7, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P5_7_TCPWM0_LINE_COMPL7)},
    {P5_7, &CY_TCPWM_1_7, CY_PIN_OUT_FUNCTION(P5_7_TCPWM1_LINE_COMPL7)},
    {P6_1, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P6_1_TCPWM0_LINE_COMPL0)},
    {P6_1, &CY_TCPWM_1_8, CY_PIN_OUT_FUNCTION(P6_1_TCPWM1_LINE_COMPL8)},
    {P6_3, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P6_3_TCPWM0_LINE_COMPL1)},
    {P6_3, &CY_TCPWM_1_9, CY_PIN_OUT_FUNCTION(P6_3_TCPWM1_LINE_COMPL9)},
    {P6_5, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P6_5_TCPWM0_LINE_COMPL2)},
    {P6_5, &CY_TCPWM_1_10, CY_PIN_OUT_FUNCTION(P6_5_TCPWM1_LINE_COMPL10)},
    {P6_7, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P6_7_TCPWM0_LINE_COMPL3)},
    {P6_7, &CY_TCPWM_1_11, CY_PIN_OUT_FUNCTION(P6_7_TCPWM1_LINE_COMPL11)},
    {P7_1, &CY_TCPWM_0_4, CY_PIN_OUT_FUNCTION(P7_1_TCPWM0_LINE_COMPL4)},
    {P7_1, &CY_TCPWM_1_12, CY_PIN_OUT_FUNCTION(P7_1_TCPWM1_LINE_COMPL12)},
    {P7_3, &CY_TCPWM_0_5, CY_PIN_OUT_FUNCTION(P7_3_TCPWM0_LINE_COMPL5)},
    {P7_3, &CY_TCPWM_1_13, CY_PIN_OUT_FUNCTION(P7_3_TCPWM1_LINE_COMPL13)},
    {P7_5, &CY_TCPWM_0_6, CY_PIN_OUT_FUNCTION(P7_5_TCPWM0_LINE_COMPL6)},
    {P7_5, &CY_TCPWM_1_14, CY_PIN_OUT_FUNCTION(P7_5_TCPWM1_LINE_COMPL14)},
    {P7_7, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P7_7_TCPWM0_LINE_COMPL7)},
    {P7_7, &CY_TCPWM_1_15, CY_PIN_OUT_FUNCTION(P7_7_TCPWM1_LINE_COMPL15)},
    {P8_1, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P8_1_TCPWM0_LINE_COMPL0)},
    {P8_1, &CY_TCPWM_1_16, CY_PIN_OUT_FUNCTION(P8_1_TCPWM1_LINE_COMPL16)},
    {P8_3, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P8_3_TCPWM0_LINE_COMPL1)},
    {P8_3, &CY_TCPWM_1_17, CY_PIN_OUT_FUNCTION(P8_3_TCPWM1_LINE_COMPL17)},
    {P8_5, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P8_5_TCPWM0_LINE_COMPL2)},
    {P8_5, &CY_TCPWM_1_18, CY_PIN_OUT_FUNCTION(P8_5_TCPWM1_LINE_COMPL18)},
    {P8_7, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P8_7_TCPWM0_LINE_COMPL3)},
    {P8_7, &CY_TCPWM_1_19, CY_PIN_OUT_FUNCTION(P8_7_TCPWM1_LINE_COMPL19)},
    {P9_1, &CY_TCPWM_0_4, CY_PIN_OUT_FUNCTION(P9_1_TCPWM0_LINE_COMPL4)},
    {P9_1, &CY_TCPWM_1_20, CY_PIN_OUT_FUNCTION(P9_1_TCPWM1_LINE_COMPL20)},
    {P9_3, &CY_TCPWM_0_5, CY_PIN_OUT_FUNCTION(P9_3_TCPWM0_LINE_COMPL5)},
    {P9_3, &CY_TCPWM_1_21, CY_PIN_OUT_FUNCTION(P9_3_TCPWM1_LINE_COMPL21)},
    {P9_5, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P9_5_TCPWM0_LINE_COMPL7)},
    {P9_5, &CY_TCPWM_1_0, CY_PIN_OUT_FUNCTION(P9_5_TCPWM1_LINE_COMPL0)},
    {P9_7, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P9_7_TCPWM0_LINE_COMPL0)},
    {P9_7, &CY_TCPWM_1_1, CY_PIN_OUT_FUNCTION(P9_7_TCPWM1_LINE_COMPL1)},
    {P10_1, &CY_TCPWM_0_6, CY_PIN_OUT_FUNCTION(P10_1_TCPWM0_LINE_COMPL6)},
    {P10_1, &CY_TCPWM_1_22, CY_PIN_OUT_FUNCTION(P10_1_TCPWM1_LINE_COMPL22)},
    {P10_3, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P10_3_TCPWM0_LINE_COMPL7)},
    {P10_3, &CY_TCPWM_1_23, CY_PIN_OUT_FUNCTION(P10_3_TCPWM1_LINE_COMPL23)},
    {P10_5, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P10_5_TCPWM0_LINE_COMPL0)},
    {P10_5, &CY_TCPWM_1_0, CY_PIN_OUT_FUNCTION(P10_5_TCPWM1_LINE_COMPL0)},
    {P10_7, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P10_7_TCPWM0_LINE_COMPL1)},
    {P10_7, &CY_TCPWM_1_2, CY_PIN_OUT_FUNCTION(P10_7_TCPWM1_LINE_COMPL2)},
    {P11_1, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P11_1_TCPWM0_LINE_COMPL1)},
    {P11_1, &CY_TCPWM_1_1, CY_PIN_OUT_FUNCTION(P11_1_TCPWM1_LINE_COMPL1)},
    {P11_3, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P11_3_TCPWM0_LINE_COMPL2)},
    {P11_3, &CY_TCPWM_1_2, CY_PIN_OUT_FUNCTION(P11_3_TCPWM1_LINE_COMPL2)},
    {P11_5, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P11_5_TCPWM0_LINE_COMPL3)},
    {P11_5, &CY_TCPWM_1_3, CY_PIN_OUT_FUNCTION(P11_5_TCPWM1_LINE_COMPL3)},
    {P12_1, &CY_TCPWM_0_4, CY_PIN_OUT_FUNCTION(P12_1_TCPWM0_LINE_COMPL4)},
    {P12_1, &CY_TCPWM_1_4, CY_PIN_OUT_FUNCTION(P12_1_TCPWM1_LINE_COMPL4)},
    {P12_3, &CY_TCPWM_0_5, CY_PIN_OUT_FUNCTION(P12_3_TCPWM0_LINE_COMPL5)},
    {P12_3, &CY_TCPWM_1_5, CY_PIN_OUT_FUNCTION(P12_3_TCPWM1_LINE_COMPL5)},
    {P12_5, &CY_TCPWM_0_6, CY_PIN_OUT_FUNCTION(P12_5_TCPWM0_LINE_COMPL6)},
    {P12_5, &CY_TCPWM_1_6, CY_PIN_OUT_FUNCTION(P12_5_TCPWM1_LINE_COMPL6)},
    {P12_7, &CY_TCPWM_0_7, CY_PIN_OUT_FUNCTION(P12_7_TCPWM0_LINE_COMPL7)},
    {P12_7, &CY_TCPWM_1_7, CY_PIN_OUT_FUNCTION(P12_7_TCPWM1_LINE_COMPL7)},
    {P13_1, &CY_TCPWM_0_0, CY_PIN_OUT_FUNCTION(P13_1_TCPWM0_LINE_COMPL0)},
    {P13_1, &CY_TCPWM_1_8, CY_PIN_OUT_FUNCTION(P13_1_TCPWM1_LINE_COMPL8)},
    {P13_3, &CY_TCPWM_0_1, CY_PIN_OUT_FUNCTION(P13_3_TCPWM0_LINE_COMPL1)},
    {P13_3, &CY_TCPWM_1_9, CY_PIN_OUT_FUNCTION(P13_3_TCPWM1_LINE_COMPL9)},
    {P13_5, &CY_TCPWM_0_2, CY_PIN_OUT_FUNCTION(P13_5_TCPWM0_LINE_COMPL2)},
    {P13_5, &CY_TCPWM_1_10, CY_PIN_OUT_FUNCTION(P13_5_TCPWM1_LINE_COMPL10)},
    {P13_7, &CY_TCPWM_0_3, CY_PIN_OUT_FUNCTION(P13_7_TCPWM0_LINE_COMPL3)},
    {P13_7, &CY_TCPWM_1_11, CY_PIN_OUT_FUNCTION(P13_7_TCPWM1_LINE_COMPL11)},
};

#endif
