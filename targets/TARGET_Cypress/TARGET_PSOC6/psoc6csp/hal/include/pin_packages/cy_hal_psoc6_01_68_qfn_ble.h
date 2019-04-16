/***************************************************************************//**
* \file cy_hal_psoc6_01_68_qfn_ble.h
*
* \brief
* PSoC6_01 device GPIO HAL header for 68-QFN-BLE package
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

#ifndef _CY_HAL_PSOC6_01_68_QFN_BLE_H_
#define _CY_HAL_PSOC6_01_68_QFN_BLE_H_

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

    P9_0 = CY_GET_GPIO(CY_PORT_9, 0),
    P9_1 = CY_GET_GPIO(CY_PORT_9, 1),
    P9_2 = CY_GET_GPIO(CY_PORT_9, 2),
    P9_3 = CY_GET_GPIO(CY_PORT_9, 3),

    P10_0 = CY_GET_GPIO(CY_PORT_10, 0),
    P10_1 = CY_GET_GPIO(CY_PORT_10, 1),

    P11_0 = CY_GET_GPIO(CY_PORT_11, 0),
    P11_1 = CY_GET_GPIO(CY_PORT_11, 1),
    P11_2 = CY_GET_GPIO(CY_PORT_11, 2),
    P11_3 = CY_GET_GPIO(CY_PORT_11, 3),
    P11_4 = CY_GET_GPIO(CY_PORT_11, 4),
    P11_5 = CY_GET_GPIO(CY_PORT_11, 5),
    P11_6 = CY_GET_GPIO(CY_PORT_11, 6),
    P11_7 = CY_GET_GPIO(CY_PORT_11, 7),

    P12_6 = CY_GET_GPIO(CY_PORT_12, 6),
    P12_7 = CY_GET_GPIO(CY_PORT_12, 7),
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
extern const cy_resource_pin_mapping_t cy_pin_map_bless_ext_lna_rx_ctl_out[1];
extern const cy_resource_pin_mapping_t cy_pin_map_bless_ext_pa_lna_chip_en_out[1];
extern const cy_resource_pin_mapping_t cy_pin_map_bless_ext_pa_tx_ctl_out[1];
extern const cy_resource_pin_mapping_t cy_pin_map_lpcomp_dsi_comp0[1];
extern const cy_resource_pin_mapping_t cy_pin_map_pass_dsi_ctb_cmp0[1];
extern const cy_resource_pin_mapping_t cy_pin_map_pass_dsi_ctb_cmp1[1];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_i2c_scl[10];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_i2c_sda[10];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_clk[9];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_miso[10];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_mosi[10];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select0[8];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select1[4];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select2[3];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select3[3];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_uart_cts[6];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_uart_rts[7];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_uart_rx[8];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_uart_tx[8];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_clk[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data0[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data1[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data2[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_data3[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_select0[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_select1[1];
extern const cy_resource_pin_mapping_t cy_pin_map_smif_spi_select2[1];
extern const cy_resource_pin_mapping_t cy_pin_map_tcpwm_line[40];
extern const cy_resource_pin_mapping_t cy_pin_map_tcpwm_line_compl[38];

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _CY_HAL_PSOC6_01_68_QFN_BLE_H_ */


/* [] END OF FILE */
