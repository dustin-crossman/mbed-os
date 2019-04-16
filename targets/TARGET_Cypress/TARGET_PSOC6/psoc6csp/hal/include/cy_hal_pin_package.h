/***************************************************************************//**
* \file cy_hal_pin_package.h
*
* Description:
* Provides definitions for the pinout for each supported device.
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#pragma once

#include "cy_gpio.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/* Port names */
typedef enum {
    CY_PORT_0  = 0x0,
    CY_PORT_1  = 0x1,
    CY_PORT_2  = 0x2,
    CY_PORT_3  = 0x3,
    CY_PORT_4  = 0x4,
    CY_PORT_5  = 0x5,
    CY_PORT_6  = 0x6,
    CY_PORT_7  = 0x7,
    CY_PORT_8  = 0x8,
    CY_PORT_9  = 0x9,
    CY_PORT_10 = 0xA,
    CY_PORT_11 = 0xB,
    CY_PORT_12 = 0xC,
    CY_PORT_13 = 0xD,
    CY_PORT_14 = 0xE,
    CY_PORT_15 = 0xF,
} cy_port_t;

typedef uint8_t cy_gpio_cfg_t; // 4bit hsiom, 4bit mode
#define CY_GPIO_CFG_CREATE(hsiom, mode)  ((cy_gpio_cfg_t)(((hsiom) << 4) + (mode)))
#define CY_GPIO_CFG_GET_MODE(x)  ((uint8_t)((x) & 0x0F))
#define CY_GPIO_CFG_GET_HSIOM(x) ((en_hsiom_sel_t)(((x) >> 4) & 0x0F))

#define CY_PIN_OUT_FUNCTION(hsiom)           CY_GPIO_CFG_CREATE(hsiom, CY_GPIO_DM_STRONG_IN_OFF)
#define CY_PIN_OUT_BUF_FUNCTION(hsiom)       CY_GPIO_CFG_CREATE(hsiom, CY_GPIO_DM_STRONG)
#define CY_PIN_OD_FUNCTION(hsiom)            CY_GPIO_CFG_CREATE(hsiom, CY_GPIO_DM_OD_DRIVESLOW_IN_OFF)
#define CY_PIN_IN_FUNCTION(hsiom)            CY_GPIO_CFG_CREATE(hsiom, CY_GPIO_DM_HIGHZ)
#define CY_PIN_PULLUP_FUNCTION(hsiom)        CY_GPIO_CFG_CREATE(hsiom, CY_GPIO_DM_PULLUP)
#define CY_PIN_ANALOG_FUNCTION(hsiom)        CY_GPIO_CFG_CREATE(HSIOM_SEL_GPIO, CY_GPIO_DM_ANALOG)

#include "cy_device.h"

#if defined(_GPIO_PSOC6_01_104_M_CSP_BLE_H_)
#include "pin_packages/cy_hal_psoc6_01_104_m_csp_ble.h"
#elif defined(_GPIO_PSOC6_01_104_M_CSP_BLE_USB_H_)
#include "pin_packages/cy_hal_psoc6_01_104_m_csp_ble_usb.h"
#elif defined(_GPIO_PSOC6_01_116_BGA_BLE_H_)
#include "pin_packages/cy_hal_psoc6_01_116_bga_ble.h"
#elif defined(_GPIO_PSOC6_01_116_BGA_USB_H_)
#include "pin_packages/cy_hal_psoc6_01_116_bga_usb.h"
#elif defined(_GPIO_PSOC6_01_124_BGA_H_)
#include "pin_packages/cy_hal_psoc6_01_124_bga.h"
#elif defined(_GPIO_PSOC6_01_124_BGA_SIP_H_)
#include "pin_packages/cy_hal_psoc6_01_124_bga_sip.h"
#elif defined(_GPIO_PSOC6_01_43_SMT_H_)
#include "pin_packages/cy_hal_psoc6_01_43_smt.h"
#elif defined(_GPIO_PSOC6_01_68_QFN_BLE_H_)
#include "pin_packages/cy_hal_psoc6_01_68_qfn_ble.h"
#elif defined(_GPIO_PSOC6_01_80_WLCSP_H_)
#include "pin_packages/cy_hal_psoc6_01_80_wlcsp.h"
#elif defined(_GPIO_PSOC6_02_100_WLCSP_H_)
#include "pin_packages/cy_hal_psoc6_02_100_wlcsp.h"
#elif defined(_GPIO_PSOC6_02_124_BGA_H_)
#include "pin_packages/cy_hal_psoc6_02_124_bga.h"
#elif defined(_GPIO_PSOC6_02_128_TQFP_H_)
#include "pin_packages/cy_hal_psoc6_02_128_tqfp.h"
#elif defined(_GPIO_PSOC6_02_68_QFN_H_)
#include "pin_packages/cy_hal_psoc6_02_68_qfn.h"
#elif defined(_GPIO_PSOC6_03_100_TQFP_H_)
#include "pin_packages/cy_hal_psoc6_03_100_tqfp.h"
#elif defined(_GPIO_PSOC6_03_50_WLCSP_H_)
#include "pin_packages/cy_hal_psoc6_03_50_wlcsp.h"
#elif defined(_GPIO_PSOC6_03_68_QFN_H_)
#include "pin_packages/cy_hal_psoc6_03_68_qfn.h"
#else
#error "Unhandled Device/PinPackage combination"
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */
