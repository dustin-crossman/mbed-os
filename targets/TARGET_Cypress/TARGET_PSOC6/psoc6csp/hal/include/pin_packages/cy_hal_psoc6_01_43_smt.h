/***************************************************************************//**
* \file cy_hal_psoc6_01_43_smt.h
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

#ifndef _CY_HAL_PSOC6_01_43_SMT_H_
#define _CY_HAL_PSOC6_01_43_SMT_H_

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
    P0_4 = CY_GET_GPIO(CY_PORT_0, 4),
    P0_5 = CY_GET_GPIO(CY_PORT_0, 5),

    P5_0 = CY_GET_GPIO(CY_PORT_5, 0),
    P5_1 = CY_GET_GPIO(CY_PORT_5, 1),
    P5_2 = CY_GET_GPIO(CY_PORT_5, 2),
    P5_3 = CY_GET_GPIO(CY_PORT_5, 3),
    P5_4 = CY_GET_GPIO(CY_PORT_5, 4),
    P5_5 = CY_GET_GPIO(CY_PORT_5, 5),
    P5_6 = CY_GET_GPIO(CY_PORT_5, 6),

    P6_2 = CY_GET_GPIO(CY_PORT_6, 2),
    P6_3 = CY_GET_GPIO(CY_PORT_6, 3),
    P6_4 = CY_GET_GPIO(CY_PORT_6, 4),
    P6_5 = CY_GET_GPIO(CY_PORT_6, 5),
    P6_6 = CY_GET_GPIO(CY_PORT_6, 6),
    P6_7 = CY_GET_GPIO(CY_PORT_6, 7),

    P7_1 = CY_GET_GPIO(CY_PORT_7, 1),
    P7_2 = CY_GET_GPIO(CY_PORT_7, 2),
    P7_7 = CY_GET_GPIO(CY_PORT_7, 7),

    P9_0 = CY_GET_GPIO(CY_PORT_9, 0),
    P9_1 = CY_GET_GPIO(CY_PORT_9, 1),
    P9_2 = CY_GET_GPIO(CY_PORT_9, 2),
    P9_3 = CY_GET_GPIO(CY_PORT_9, 3),
    P9_4 = CY_GET_GPIO(CY_PORT_9, 4),
    P9_5 = CY_GET_GPIO(CY_PORT_9, 5),
    P9_6 = CY_GET_GPIO(CY_PORT_9, 6),

    P10_0 = CY_GET_GPIO(CY_PORT_10, 0),
    P10_1 = CY_GET_GPIO(CY_PORT_10, 1),
    P10_2 = CY_GET_GPIO(CY_PORT_10, 2),
    P10_3 = CY_GET_GPIO(CY_PORT_10, 3),
    P10_4 = CY_GET_GPIO(CY_PORT_10, 4),
    P10_5 = CY_GET_GPIO(CY_PORT_10, 5),
    P10_6 = CY_GET_GPIO(CY_PORT_10, 6),

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
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_clk_i2s_if[1];
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_pdm_clk[1];
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_pdm_data[1];
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_rx_sck[1];
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_rx_sdi[1];
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_rx_ws[1];
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_tx_sck[1];
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_tx_sdo[1];
extern const cy_resource_pin_mapping_t cy_pin_map_audioss_tx_ws[1];
extern const cy_resource_pin_mapping_t cy_pin_map_pass_dsi_ctb_cmp0[1];
extern const cy_resource_pin_mapping_t cy_pin_map_pass_dsi_ctb_cmp1[1];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_i2c_scl[5];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_i2c_sda[6];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_clk[9];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_miso[6];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_mosi[5];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select0[8];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select1[5];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select2[4];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_spi_select3[4];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_uart_cts[6];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_uart_rts[7];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_uart_rx[4];
extern const cy_resource_pin_mapping_t cy_pin_map_scb_uart_tx[5];
extern const cy_resource_pin_mapping_t cy_pin_map_tcpwm_line[38];
extern const cy_resource_pin_mapping_t cy_pin_map_tcpwm_line_compl[34];

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _CY_HAL_PSOC6_01_43_SMT_H_ */


/* [] END OF FILE */
