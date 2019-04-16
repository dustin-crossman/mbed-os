/***************************************************************************//**
* \file cy_hal_psoc6_02_128_tqfp.h
*
* \brief
* PSoC6_02 device GPIO HAL header for 128-TQFP package
*
* \note
* Generator version: 1.4.7041.28143
*
********************************************************************************
* \copyright
* Copyright 2016-2019, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CY_HAL_PSOC6_02_128_TQFP_H_
#define _CY_HAL_PSOC6_02_128_TQFP_H_

#include "cy_hal_hw_resources.h"

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
    P4_2 = CY_GET_GPIO(CY_PORT_4, 2),
    P4_3 = CY_GET_GPIO(CY_PORT_4, 3),

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
} cy_gpio_t;

/* Connection type definition */
/** Represents an association between a pin and a resource */
typedef struct
{
    cy_gpio_t                  pin;  //!< The GPIO pin
    const cy_resource_inst_t  *inst; //!< The associated resource instance
    cy_gpio_cfg_t              cfg;  //!< The DriveMode and HSIOM configuration value
} cy_resource_pin_mapping_t;

/* Pin connections */
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_clk_i2s_if[4];
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_pdm_clk[2];
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_pdm_data[2];
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_rx_sck[4];
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_rx_sdi[4];
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_rx_ws[4];
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_tx_sck[4];
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_tx_sdo[4];
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_tx_ws[4];
extern const cy_resource_pin_mapping_t cy_pin_map_lpcomp_dsi_comp0[1];
extern const cy_resource_pin_mapping_t cy_pin_map_lpcomp_dsi_comp1[1];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_i2c_scl[21];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_i2c_sda[21];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_clk[17];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_miso[17];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_mosi[17];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select0[17];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select1[13];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select2[13];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select3[10];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_uart_cts[19];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_uart_rts[19];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_uart_rx[19];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_uart_tx[19];
extern const cy_resource_pin_mapping_t cy_pin_map_sdhc_card_cmd[2];
extern const cy_resource_pin_mapping_t cy_pin_map_sdhc_card_dat_3to0[8];
extern const cy_resource_pin_mapping_t cy_pin_map_sdhc_card_dat_7to4[4];
extern const cy_resource_pin_mapping_t cy_pin_map_sdhc_card_detect_n[2];
extern const cy_resource_pin_mapping_t cy_pin_map_sdhc_card_emmc_reset_n[1];
extern const cy_resource_pin_mapping_t cy_pin_map_sdhc_card_if_pwr_en[2];
extern const cy_resource_pin_mapping_t cy_pin_map_sdhc_card_mech_write_prot[2];
extern const cy_resource_pin_mapping_t cy_pin_map_sdhc_clk_card[2];
extern const cy_resource_pin_mapping_t cy_pin_map_sdhc_io_volt_sel[2];
extern const cy_resource_pin_mapping_t cy_pin_map_sdhc_led_ctrl[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_clk[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data0[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data1[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data2[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data3[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data4[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data5[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data6[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data7[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_select0[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_select1[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_select2[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_select3[1];
extern const cy_resource_pin_mapping_t cy_pin_map_tcpwm_line[100];
extern const cy_resource_pin_mapping_t cy_pin_map_tcpwm_line_compl[100];

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _CY_HAL_PSOC6_02_128_TQFP_H_ */


/* [] END OF FILE */
