/***************************************************************************//**
* \file cyhal_psoc6_01_124_bga.h
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

#ifndef _CY_HAL_PSOC6_01_124_BGA_H_
#define _CY_HAL_PSOC6_01_124_BGA_H_

#include "cyhal_hw_resources.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#define CY_GET_GPIO(port, pin) (((port) << 16) + (pin))

/* Pin names */
typedef enum {
    NC = (int)0xFFFFFFFF,

    P0_0 = CY_GET_GPIO(CY_PORT_0, 0),
    P0_1 = CY_GET_GPIO(CY_PORT_0, 1),
    P0_2 = CY_GET_GPIO(CY_PORT_0, 2),
    P0_3 = CY_GET_GPIO(CY_PORT_0, 3),
    P0_4 = CY_GET_GPIO(CY_PORT_0, 4),
    P0_5 = CY_GET_GPIO(CY_PORT_0, 5),

    P1_0 = CY_GET_GPIO(CY_PORT_1, 0),
    P1_1 = CY_GET_GPIO(CY_PORT_1, 1),
    P1_2 = CY_GET_GPIO(CY_PORT_1, 2),
    P1_3 = CY_GET_GPIO(CY_PORT_1, 3),
    P1_4 = CY_GET_GPIO(CY_PORT_1, 4),
    P1_5 = CY_GET_GPIO(CY_PORT_1, 5),

    P2_0 = CY_GET_GPIO(CY_PORT_2, 0),
    P2_1 = CY_GET_GPIO(CY_PORT_2, 1),
    P2_2 = CY_GET_GPIO(CY_PORT_2, 2),
    P2_3 = CY_GET_GPIO(CY_PORT_2, 3),
    P2_4 = CY_GET_GPIO(CY_PORT_2, 4),
    P2_5 = CY_GET_GPIO(CY_PORT_2, 5),
    P2_6 = CY_GET_GPIO(CY_PORT_2, 6),
    P2_7 = CY_GET_GPIO(CY_PORT_2, 7),

    P3_0 = CY_GET_GPIO(CY_PORT_3, 0),
    P3_1 = CY_GET_GPIO(CY_PORT_3, 1),
    P3_2 = CY_GET_GPIO(CY_PORT_3, 2),
    P3_3 = CY_GET_GPIO(CY_PORT_3, 3),
    P3_4 = CY_GET_GPIO(CY_PORT_3, 4),
    P3_5 = CY_GET_GPIO(CY_PORT_3, 5),

    P4_0 = CY_GET_GPIO(CY_PORT_4, 0),
    P4_1 = CY_GET_GPIO(CY_PORT_4, 1),

    P5_0 = CY_GET_GPIO(CY_PORT_5, 0),
    P5_1 = CY_GET_GPIO(CY_PORT_5, 1),
    P5_2 = CY_GET_GPIO(CY_PORT_5, 2),
    P5_3 = CY_GET_GPIO(CY_PORT_5, 3),
    P5_4 = CY_GET_GPIO(CY_PORT_5, 4),
    P5_5 = CY_GET_GPIO(CY_PORT_5, 5),
    P5_6 = CY_GET_GPIO(CY_PORT_5, 6),
    P5_7 = CY_GET_GPIO(CY_PORT_5, 7),

    P6_0 = CY_GET_GPIO(CY_PORT_6, 0),
    P6_1 = CY_GET_GPIO(CY_PORT_6, 1),
    P6_2 = CY_GET_GPIO(CY_PORT_6, 2),
    P6_3 = CY_GET_GPIO(CY_PORT_6, 3),
    P6_4 = CY_GET_GPIO(CY_PORT_6, 4),
    P6_5 = CY_GET_GPIO(CY_PORT_6, 5),
    P6_6 = CY_GET_GPIO(CY_PORT_6, 6),
    P6_7 = CY_GET_GPIO(CY_PORT_6, 7),

    P7_0 = CY_GET_GPIO(CY_PORT_7, 0),
    P7_1 = CY_GET_GPIO(CY_PORT_7, 1),
    P7_2 = CY_GET_GPIO(CY_PORT_7, 2),
    P7_3 = CY_GET_GPIO(CY_PORT_7, 3),
    P7_4 = CY_GET_GPIO(CY_PORT_7, 4),
    P7_5 = CY_GET_GPIO(CY_PORT_7, 5),
    P7_6 = CY_GET_GPIO(CY_PORT_7, 6),
    P7_7 = CY_GET_GPIO(CY_PORT_7, 7),

    P8_0 = CY_GET_GPIO(CY_PORT_8, 0),
    P8_1 = CY_GET_GPIO(CY_PORT_8, 1),
    P8_2 = CY_GET_GPIO(CY_PORT_8, 2),
    P8_3 = CY_GET_GPIO(CY_PORT_8, 3),
    P8_4 = CY_GET_GPIO(CY_PORT_8, 4),
    P8_5 = CY_GET_GPIO(CY_PORT_8, 5),
    P8_6 = CY_GET_GPIO(CY_PORT_8, 6),
    P8_7 = CY_GET_GPIO(CY_PORT_8, 7),

    P9_0 = CY_GET_GPIO(CY_PORT_9, 0),
    P9_1 = CY_GET_GPIO(CY_PORT_9, 1),
    P9_2 = CY_GET_GPIO(CY_PORT_9, 2),
    P9_3 = CY_GET_GPIO(CY_PORT_9, 3),
    P9_4 = CY_GET_GPIO(CY_PORT_9, 4),
    P9_5 = CY_GET_GPIO(CY_PORT_9, 5),
    P9_6 = CY_GET_GPIO(CY_PORT_9, 6),
    P9_7 = CY_GET_GPIO(CY_PORT_9, 7),

    P10_0 = CY_GET_GPIO(CY_PORT_10, 0),
    P10_1 = CY_GET_GPIO(CY_PORT_10, 1),
    P10_2 = CY_GET_GPIO(CY_PORT_10, 2),
    P10_3 = CY_GET_GPIO(CY_PORT_10, 3),
    P10_4 = CY_GET_GPIO(CY_PORT_10, 4),
    P10_5 = CY_GET_GPIO(CY_PORT_10, 5),
    P10_6 = CY_GET_GPIO(CY_PORT_10, 6),
    P10_7 = CY_GET_GPIO(CY_PORT_10, 7),

    P11_0 = CY_GET_GPIO(CY_PORT_11, 0),
    P11_1 = CY_GET_GPIO(CY_PORT_11, 1),
    P11_2 = CY_GET_GPIO(CY_PORT_11, 2),
    P11_3 = CY_GET_GPIO(CY_PORT_11, 3),
    P11_4 = CY_GET_GPIO(CY_PORT_11, 4),
    P11_5 = CY_GET_GPIO(CY_PORT_11, 5),
    P11_6 = CY_GET_GPIO(CY_PORT_11, 6),
    P11_7 = CY_GET_GPIO(CY_PORT_11, 7),

    P12_0 = CY_GET_GPIO(CY_PORT_12, 0),
    P12_1 = CY_GET_GPIO(CY_PORT_12, 1),
    P12_2 = CY_GET_GPIO(CY_PORT_12, 2),
    P12_3 = CY_GET_GPIO(CY_PORT_12, 3),
    P12_4 = CY_GET_GPIO(CY_PORT_12, 4),
    P12_5 = CY_GET_GPIO(CY_PORT_12, 5),
    P12_6 = CY_GET_GPIO(CY_PORT_12, 6),
    P12_7 = CY_GET_GPIO(CY_PORT_12, 7),

    P13_0 = CY_GET_GPIO(CY_PORT_13, 0),
    P13_1 = CY_GET_GPIO(CY_PORT_13, 1),
    P13_2 = CY_GET_GPIO(CY_PORT_13, 2),
    P13_3 = CY_GET_GPIO(CY_PORT_13, 3),
    P13_4 = CY_GET_GPIO(CY_PORT_13, 4),
    P13_5 = CY_GET_GPIO(CY_PORT_13, 5),
    P13_6 = CY_GET_GPIO(CY_PORT_13, 6),
    P13_7 = CY_GET_GPIO(CY_PORT_13, 7),

    USBDP = CY_GET_GPIO(CY_PORT_14, 0),
    USBDM = CY_GET_GPIO(CY_PORT_14, 1),
} cyhal_gpio_t;

/* Connection type definition */
/** Represents an association between a pin and a resource */
typedef struct
{
    cyhal_gpio_t                  pin;  //!< The GPIO pin
    const cyhal_resource_inst_t  *inst; //!< The associated resource instance
    cyhal_gpio_cfg_t              cfg;  //!< The DriveMode and HSIOM configuration value
} cyhal_resource_pin_mapping_t;

/* Pin connections */
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_clk_i2s_if[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_pdm_clk[2];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_pdm_data[2];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_sck[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_sdi[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_rx_ws[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_sck[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_sdo[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_audioss_tx_ws[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_ext_lna_rx_ctl_out[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_ext_pa_lna_chip_en_out[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_ext_pa_tx_ctl_out[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_act_bpktctl[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_act_dbus_rx_en[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_act_dbus_tx_en[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_act_txd_rxd[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_act_ldo_en[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_buck_en[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_clk_en[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_dig_ldo_en[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_isolate_n[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_rcb_clk[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_rcb_data[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_rcb_le[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_reset_n[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_ret_ldo_ol_hv[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_ret_switch_hv[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_bless_mxd_dpslp_xtal_en[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_dsi_comp0[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_lpcomp_dsi_comp1[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_pass_dsi_ctb_cmp0[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_pass_dsi_ctb_cmp1[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_scl[17];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_i2c_sda[17];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_clk[16];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_miso[17];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_mosi[17];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_select0[16];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_select1[13];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_select2[13];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_spi_select3[10];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_cts[14];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rts[14];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_rx[15];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_scb_uart_tx[15];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_clk[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data0[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data1[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data2[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data3[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data4[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data5[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data6[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_data7[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select0[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select1[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select2[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_smif_spi_select3[1];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line[98];
extern const cyhal_resource_pin_mapping_t cyhal_pin_map_tcpwm_line_compl[98];

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _CY_HAL_PSOC6_01_124_BGA_H_ */


/* [] END OF FILE */
