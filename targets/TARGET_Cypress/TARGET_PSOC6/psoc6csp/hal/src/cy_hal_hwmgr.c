/***************************************************************************//**
* \file cy_hal_hwmgr.c
*
* \brief
* Provides a high level interface for managing hardware resources. This is
* used to track what hardware blocks are already being used so they are not over
* allocated.
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

#include "cy_hal_hwmgr.h"
#include "cy_hal_system.h"
#include "cmsis_compiler.h"

/*******************************************************************************
*       Defines
*******************************************************************************/

#ifdef CY_IP_MXS40PASS_SAR_INSTANCES
    #define CY_BLOCK_COUNT_ADC  CY_IP_MXS40PASS_SAR_INSTANCES
#else
    #define CY_BLOCK_COUNT_ADC  0
#endif

#ifdef CY_IP_MXBLESS_INSTANCES
    #define CY_BLOCK_COUNT_BLE      CY_IP_MXBLESS_INSTANCES
#else
    #define CY_BLOCK_COUNT_BLE      0
#endif

#ifdef CY_IP_MXTTCANFD_INSTANCES
    #define CY_BLOCK_COUNT_CAN      CY_IP_MXTTCANFD_INSTANCES
    
    #if (CY_IP_MXTTCANFD_INSTANCES == 0)
        #define CY_CHANNEL_COUNT_CAN (0u)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 1)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 2)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 3)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 4)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 5)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 6)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 7)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR + CANFD6_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 8)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR + CANFD6_CAN_NR + CANFD7_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 9)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR + CANFD6_CAN_NR + CANFD7_CAN_NR + CANFD8_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 10)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR + CANFD6_CAN_NR + CANFD7_CAN_NR + CANFD8_CAN_NR + CANFD9_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES > 10)
        #warning Unhandled CAN instance count
    #endif
#else
    #define CY_BLOCK_COUNT_CAN      0
    #define CY_CHANNEL_COUNT_CAN    0
#endif

#define CY_BLOCK_COUNT_CLOCK    4
#define CY_CHANNEL_COUNT_CLOCK  (PERI_DIV_8_NR + PERI_DIV_16_NR + PERI_DIV_16_5_NR + PERI_DIV_24_5_NR)
    
#if defined(CY_IP_MXCRYPTO_INSTANCES)
    #define CY_BLOCK_COUNT_CRC      CY_IP_MXCRYPTO_INSTANCES
#elif defined(CPUSS_CRYPTO_PRESENT)
    #define CY_BLOCK_COUNT_CRC      1
#else
    #define CY_BLOCK_COUNT_CRC      0
#endif

#ifdef CY_IP_MXS40PASS_CTDAC_INSTANCES
    #define CY_BLOCK_COUNT_DAC      CY_IP_MXS40PASS_CTDAC_INSTANCES
#else
    #define CY_BLOCK_COUNT_DAC      0
#endif

#if defined(CY_IP_M4CPUSS_DMAC_INSTANCES) || defined(CY_IP_M4CPUSS_DMA_INSTANCES)
    #ifndef CPUSS_DMAC_CH_NR
        #define CPUSS_DMAC_CH_NR (0u)
    #endif
    
    #define CY_BLOCK_COUNT_DMA      3
    #define CY_CHANNEL_COUNT_DMA    (CPUSS_DW0_CH_NR + CPUSS_DW1_CH_NR + CPUSS_DMAC_CH_NR)
#endif

    #define CY_BLOCK_COUNT_FLASH    1
    
#ifdef IOSS_GPIO_GPIO_PORT_NR
    #define CY_BLOCK_COUNT_GPIO IOSS_GPIO_GPIO_PORT_NR
    #define CY_CHANNEL_COUNT_GPIO   (8 * IOSS_GPIO_GPIO_PORT_NR)
#else
    #define CY_BLOCK_COUNT_GPIO 0
    #define CY_CHANNEL_COUNT_GPIO   0
#endif

#ifdef CY_IP_MXAUDIOSS_INSTANCES
    #define CY_BLOCK_COUNT_I2S      CY_IP_MXAUDIOSS_INSTANCES
#else
    #define CY_BLOCK_COUNT_I2S      0
#endif

#ifdef CY_IP_MXLCD_INSTANCES
    #define CY_BLOCK_COUNT_LCD      CY_IP_MXLCD_INSTANCES
#else
    #define CY_BLOCK_COUNT_LCD      0
#endif

#ifdef CY_IP_MXLPCOMP_INSTANCES
    #define CY_BLOCK_COUNT_LPCOMP   (2 * CY_IP_MXLPCOMP_INSTANCES)
#else
    #define CY_BLOCK_COUNT_LPCOMP   0
#endif

#if defined(PASS_NR_CTBS)
    #define CY_BLOCK_COUNT_OPAMP    (2 * PASS_NR_CTBS)
#else
    #define CY_BLOCK_COUNT_OPAMP    0
#endif

#ifdef CY_IP_MXAUDIOSS_INSTANCES
    #define CY_BLOCK_COUNT_PDMPCM   CY_IP_MXAUDIOSS_INSTANCES
#else
    #define CY_BLOCK_COUNT_PDMPCM   0
#endif

#ifdef CY_IP_MXSMIF_INSTANCES
    #define CY_BLOCK_COUNT_QSPI CY_IP_MXSMIF_INSTANCES
#else
    #define CY_BLOCK_COUNT_QSPI 0
#endif

#if defined(CY_IP_MXCRYPTO_INSTANCES)
    #define CY_BLOCK_COUNT_RNG      CY_IP_MXCRYPTO_INSTANCES
#elif defined(CPUSS_CRYPTO_PRESENT)
    #define CY_BLOCK_COUNT_RNG      1
#else
    #define CY_BLOCK_COUNT_RNG      0
#endif

#ifdef CY_IP_MXS40SRSS_RTC_INSTANCES
    #define CY_BLOCK_COUNT_RTC      CY_IP_MXS40SRSS_RTC_INSTANCES
#else
    #define CY_BLOCK_COUNT_RTC      0
#endif

#ifdef CY_IP_MXSCB_INSTANCES
    #define CY_BLOCK_COUNT_SCB      CY_IP_MXSCB_INSTANCES
#else
    #define CY_BLOCK_COUNT_SCB      0
#endif

#ifdef CY_IP_MXSDHC_INSTANCES
    #define CY_BLOCK_COUNT_SDHC CY_IP_MXSDHC_INSTANCES
#else
    #define CY_BLOCK_COUNT_SDHC 0
#endif

#ifdef CY_IP_MXTCPWM_INSTANCES
    #define CY_BLOCK_COUNT_TCPWM    CY_IP_MXTCPWM_INSTANCES
    
    #if (CY_IP_MXTCPWM_INSTANCES == 0)
        #define CY_CHANNEL_COUNT_TCPWM (0u)
    #elif (CY_IP_MXTCPWM_INSTANCES == 1)
        #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR)
    #elif (CY_IP_MXTCPWM_INSTANCES == 2)
        #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR)
    #elif (CY_IP_MXTCPWM_INSTANCES == 3)
        #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR)
    #elif (CY_IP_MXTCPWM_INSTANCES == 4)
        #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR)
    #elif (CY_IP_MXTCPWM_INSTANCES == 5)
        #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR)
    #elif (CY_IP_MXTCPWM_INSTANCES == 6)
        #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR)
    #elif (CY_IP_MXTCPWM_INSTANCES == 7)
        #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR + TCPWM6_CNT_NR)
    #elif (CY_IP_MXTCPWM_INSTANCES == 8)
        #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR + TCPWM6_CNT_NR + TCPWM7_CNT_NR)
    #elif (CY_IP_MXTCPWM_INSTANCES == 9)
        #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR + TCPWM6_CNT_NR + TCPWM7_CNT_NR + TCPWM8_CNT_NR)
    #elif (CY_IP_MXTCPWM_INSTANCES == 10)
        #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR + TCPWM6_CNT_NR + TCPWM7_CNT_NR + TCPWM8_CNT_NR + TCPWM9_CNT_NR)
    #elif (CY_IP_MXTCPWM_INSTANCES > 10)
        #warning Unhandled TCPWM instance count
    #endif
#else
    #define CY_BLOCK_COUNT_TCPWM    0
    #define CY_CHANNEL_COUNT_TCPWM  0
#endif

#ifdef CY_IP_MXUDB_INSTANCES
    #define CY_BLOCK_COUNT_UDB      CY_IP_MXUDB_INSTANCES
#else
    #define CY_BLOCK_COUNT_UDB      0
#endif

#ifdef CY_IP_MXS40SRSS_MCWDT_INSTANCES
    #define CY_BLOCK_COUNT_WDT      CY_IP_MXS40SRSS_MCWDT_INSTANCES
#else
    #define CY_BLOCK_COUNT_WDT      0
#endif


/*
    All resources have an offset and a size, offsets are stored in an array
    Subsequent resource offset equals the preceding offset + size
    Offsets are bit indexes in the arrays that track used, configured etc.
    
    Channel based resources have an extra array for block offsets

    Note these are NOT offsets into the device's MMIO address space;
    they are bit offsets into arrays that are internal to the HW mgr.
*/


#define CY_OFFSET_ADC      0
#define CY_SIZE_ADC        CY_BLOCK_COUNT_ADC
#define CY_OFFSET_BLE      (CY_OFFSET_ADC + CY_BLOCK_COUNT_ADC)
#define CY_SIZE_BLE        CY_BLOCK_COUNT_BLE
#define CY_OFFSET_CAN      (CY_OFFSET_BLE + CY_BLOCK_COUNT_BLE)
#define CY_SIZE_CAN        CY_CHANNEL_COUNT_CAN
#define CY_OFFSET_CLOCK    (CY_OFFSET_CAN + CY_SIZE_CAN)
#define CY_SIZE_CLOCK      CY_CHANNEL_COUNT_CLOCK
#define CY_OFFSET_CRC      (CY_OFFSET_CLOCK + CY_SIZE_CLOCK)
#define CY_SIZE_CRC        CY_BLOCK_COUNT_CRC
#define CY_OFFSET_DAC      (CY_OFFSET_CRC + CY_SIZE_CRC)
#define CY_SIZE_DAC        CY_BLOCK_COUNT_DAC
#define CY_OFFSET_DMA      (CY_OFFSET_DAC + CY_SIZE_DAC)
#define CY_SIZE_DMA        CY_CHANNEL_COUNT_DMA
#define CY_OFFSET_FLASH    (CY_OFFSET_DMA + CY_SIZE_DMA)
#define CY_SIZE_FLASH      CY_BLOCK_COUNT_FLASH
#define CY_OFFSET_GPIO     (CY_OFFSET_FLASH + CY_SIZE_FLASH)
#define CY_SIZE_GPIO       CY_CHANNEL_COUNT_GPIO
#define CY_OFFSET_I2S      (CY_OFFSET_GPIO + CY_SIZE_GPIO)
#define CY_SIZE_I2S        CY_BLOCK_COUNT_I2S
#define CY_OFFSET_LCD      (CY_OFFSET_I2S + CY_SIZE_I2S)
#define CY_SIZE_LCD        CY_BLOCK_COUNT_LCD
#define CY_OFFSET_LPCOMP   (CY_OFFSET_LCD + CY_SIZE_LCD)
#define CY_SIZE_LPCOMP     CY_BLOCK_COUNT_LPCOMP
#define CY_OFFSET_OPAMP    (CY_OFFSET_LPCOMP + CY_SIZE_LPCOMP)
#define CY_SIZE_OPAMP      CY_BLOCK_COUNT_OPAMP
#define CY_OFFSET_PDMPCM   (CY_OFFSET_OPAMP + CY_SIZE_OPAMP)
#define CY_SIZE_PDMPCM     CY_BLOCK_COUNT_PDMPCM
#define CY_OFFSET_QSPI     (CY_OFFSET_PDMPCM + CY_SIZE_PDMPCM)
#define CY_SIZE_QSPI       CY_BLOCK_COUNT_QSPI
#define CY_OFFSET_RNG      (CY_OFFSET_QSPI + CY_SIZE_QSPI)
#define CY_SIZE_RNG        CY_BLOCK_COUNT_RNG
#define CY_OFFSET_RTC      (CY_OFFSET_RNG + CY_SIZE_RNG)
#define CY_SIZE_RTC        CY_BLOCK_COUNT_RTC
#define CY_OFFSET_SCB      (CY_OFFSET_RTC + CY_SIZE_RTC)
#define CY_SIZE_SCB        CY_BLOCK_COUNT_SCB
#define CY_OFFSET_SDHC     (CY_OFFSET_SCB + CY_SIZE_SCB)
#define CY_SIZE_SDHC       CY_BLOCK_COUNT_SDHC
#define CY_OFFSET_TCPWM    (CY_OFFSET_SDHC + CY_SIZE_SDHC)
#define CY_SIZE_TCPWM      CY_CHANNEL_COUNT_TCPWM
#define CY_OFFSET_UDB      (CY_OFFSET_TCPWM + CY_SIZE_TCPWM)
#define CY_SIZE_UDB        CY_BLOCK_COUNT_UDB
#define CY_OFFSET_WDT      (CY_OFFSET_UDB + CY_SIZE_UDB)
#define CY_SIZE_WDT        CY_BLOCK_COUNT_WDT

#define CY_TOTAL_ALLOCATABLE_ITEMS     (CY_OFFSET_WDT + CY_SIZE_WDT)

#define CY_BYTE_NUM_SHIFT (3)
#define CY_BIT_NUM_MASK (0x07)

/*******************************************************************************
*       Variables
*******************************************************************************/

static const uint8_t cy_block_offsets_clock[4] =
{
    0, // 8-bit dividers
    PERI_DIV_8_NR, // 16-bit dividers
    PERI_DIV_8_NR + PERI_DIV_16_NR, // 16.5 bit dividers
    PERI_DIV_8_NR + PERI_DIV_16_NR + PERI_DIV_16_5_NR, // 24.5 bit dividers
};

static const uint8_t cy_block_offsets_dma[] =
{
    0,
    CPUSS_DW0_CH_NR,
    CPUSS_DW0_CH_NR + CPUSS_DW1_CH_NR,
};

static const uint8_t cy_block_offsets_gpio[] =
{
    0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 
};

static const uint8_t cy_block_offsets_can[] =
{
#ifdef CY_IP_MXTTCANFD_INSTANCES
    #if (CY_IP_MXTTCANFD_INSTANCES > 0)
        0,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 1)
        CANFD0_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 2)
        CANFD0_CAN_NR + CANFD1_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 3)
        CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 4)
        CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 5)
        CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 6)
        CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 7)
        CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR + CANFD6_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 8)
        CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR + CANFD6_CAN_NR + CANFD7_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 9)
        CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR + CANFD6_CAN_NR + CANFD7_CAN_NR + CANFD8_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 10)
        #warning Unhandled CAN instance count
    #endif
#else
    0
#endif
};

static const uint8_t cy_block_offsets_tcpwm[] =
{
#ifdef CY_IP_MXTCPWM_INSTANCES
    #if (CY_IP_MXTCPWM_INSTANCES > 0)
        0,
    #endif
    #if (CY_IP_MXTCPWM_INSTANCES > 1)
        TCPWM0_CNT_NR,
    #endif
    #if (CY_IP_MXTCPWM_INSTANCES > 2)
        TCPWM0_CNT_NR + TCPWM1_CNT_NR,
    #endif
    #if (CY_IP_MXTCPWM_INSTANCES > 3)
        TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR,
    #endif
    #if (CY_IP_MXTCPWM_INSTANCES > 4)
        TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR,
    #endif
    #if (CY_IP_MXTCPWM_INSTANCES > 5)
        TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR,
    #endif
    #if (CY_IP_MXTCPWM_INSTANCES > 6)
        TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR,
    #endif
    #if (CY_IP_MXTCPWM_INSTANCES > 7)
        TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR + TCPWM6_CNT_NR,
    #endif
    #if (CY_IP_MXTCPWM_INSTANCES > 8)
        TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR + TCPWM6_CNT_NR + TCPWM7_CNT_NR,
    #endif
    #if (CY_IP_MXTCPWM_INSTANCES > 9)
        TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR + TCPWM6_CNT_NR + TCPWM7_CNT_NR + TCPWM8_CNT_NR,
    #endif
    #if (CY_IP_MXTCPWM_INSTANCES > 10)
        #warning Unhandled TCPWM instance count
    #endif
#else
    0
#endif
};  

static uint8_t cy_used[(CY_TOTAL_ALLOCATABLE_ITEMS + 7) / 8] = {0};
static uint8_t cy_configured[(CY_TOTAL_ALLOCATABLE_ITEMS + 7) / 8] = {0};

/** Array of pin to resource mappings, provided by the BSP. Must be terminated with a CY_RSC_INVALID entry */
extern cy_resource_pin_mapping_t* cy_resource_pin_mapping;

// Note: the ordering here needs to be parallel to that of cy_resource_t
static const uint16_t cy_resource_offsets[] = 
{
    CY_OFFSET_ADC,
    CY_OFFSET_BLE,
    CY_OFFSET_CAN,
    CY_OFFSET_CLOCK,
    CY_OFFSET_CRC,
    CY_OFFSET_DAC,
    CY_OFFSET_DMA,
    CY_OFFSET_FLASH,
    CY_OFFSET_GPIO,
    CY_OFFSET_I2S,
    CY_OFFSET_LCD,
    CY_OFFSET_LPCOMP,
    CY_OFFSET_OPAMP,
    CY_OFFSET_PDMPCM,
    CY_OFFSET_QSPI,
    CY_OFFSET_RNG,
    CY_OFFSET_RTC,
    CY_OFFSET_SCB,
    CY_OFFSET_SDHC,
    CY_OFFSET_TCPWM,
    CY_OFFSET_UDB,
    CY_OFFSET_WDT,
};

static const uint32_t cy_has_channels = 
    (1 << CY_RSC_CAN)   |
    (1 << CY_RSC_CLOCK) |
    (1 << CY_RSC_DMA)   |
    (1 << CY_RSC_GPIO)  |
    (1 << CY_RSC_TCPWM) ;

/*******************************************************************************
*       Utility helper functions
*******************************************************************************/

static inline uint16_t cy_uses_channels(cy_resource_t type)
{
    return (cy_has_channels & (1 << type)) > 0;
}

static inline uint16_t cy_get_resource_offset(cy_resource_t type)
{
    return cy_resource_offsets[type];
}

static inline const uint8_t* cy_get_block_offsets(cy_resource_t type)
{
    switch (type)
    {
        case CY_RSC_CAN:
            return cy_block_offsets_can;
        case CY_RSC_CLOCK:
            return cy_block_offsets_clock;
        case CY_RSC_DMA:
            return cy_block_offsets_dma;
        case CY_RSC_GPIO:
            return cy_block_offsets_gpio;
        case CY_RSC_TCPWM:
            return cy_block_offsets_tcpwm;
        default:
            CY_ASSERT(false);
            return NULL;
    }
}

// Gets the number of block offset entries, only valid for blocks which have channels.
static inline uint8_t cy_get_block_offset_length(cy_resource_t type)
{
    switch (type)
    {
        case CY_RSC_CAN:
            return sizeof(cy_block_offsets_can)/sizeof(cy_block_offsets_can[0]);
        case CY_RSC_CLOCK:
            return sizeof(cy_block_offsets_clock)/sizeof(cy_block_offsets_clock[0]);
        case CY_RSC_DMA:
            return sizeof(cy_block_offsets_dma)/sizeof(cy_block_offsets_dma[0]);
        case CY_RSC_GPIO:
            return sizeof(cy_block_offsets_gpio)/sizeof(cy_block_offsets_gpio[0]);
        case CY_RSC_TCPWM:
            return sizeof(cy_block_offsets_tcpwm)/sizeof(cy_block_offsets_tcpwm[0]);
        default:
            CY_ASSERT(false);
            return 0;
    }
}

static cy_rslt_t cy_get_bit_position(cy_resource_t type, uint8_t block, uint8_t channel, uint16_t* bitPosition)
{
    uint16_t offsetRsc = cy_get_resource_offset(type);
    // Offset that is one past the beginning of the next resource (or one past the end of the array).
    // Our offset must be strictly less than that
    uint16_t offsetEndOfRsc = ((1u + type) < sizeof(cy_resource_offsets)/sizeof(cy_resource_offsets[0]))
        ? cy_get_resource_offset((cy_resource_t)(type + 1))
        : CY_TOTAL_ALLOCATABLE_ITEMS;

    if (cy_uses_channels(type))
    {
        const uint8_t* blockOffsets = cy_get_block_offsets(type);
        *bitPosition = offsetEndOfRsc;
        if (blockOffsets != NULL)
        {
            // Offset (from the beginning of the section for this block type) that is one past the end of
            // the requested block index. The channel number must be strictly less than that.
            uint16_t blocks = cy_get_block_offset_length(type);
            if (block < blocks)
            {
                *bitPosition = offsetRsc + blockOffsets[block] + channel;
                if ((block + 1) < blocks)
                {
                    offsetEndOfRsc = offsetRsc + blockOffsets[block + 1];
                }
            }
        }
    }
    else
    {
        *bitPosition = offsetRsc + block;
    }

    return (*bitPosition < offsetEndOfRsc)
        ? CY_RSLT_SUCCESS
        : CY_RSLT_ERR_CSP_HWMGR_INVALID;
}

static inline cy_rslt_t cy_is_set(const uint8_t* used, cy_resource_t type, uint8_t block, uint8_t channel, bool* isSet)
{
    uint16_t bitPosition;
    cy_rslt_t status = cy_get_bit_position(type, block, channel, &bitPosition);
    if (status == CY_RSLT_SUCCESS)
    {
        uint8_t byte = bitPosition >> CY_BYTE_NUM_SHIFT;
        uint8_t bit = bitPosition & CY_BIT_NUM_MASK;
        *isSet = (used[byte] & (1 << bit));
    }
    return status;
}

static inline cy_rslt_t cy_set_bit(uint8_t* used, cy_resource_t type, uint8_t block, uint8_t channel)
{
    uint16_t bitPosition;
    cy_rslt_t status = cy_get_bit_position(type, block, channel, &bitPosition);
    if (status == CY_RSLT_SUCCESS)
    {
        uint8_t byte = bitPosition >> CY_BYTE_NUM_SHIFT;
        uint8_t bit = bitPosition & CY_BIT_NUM_MASK;
        used[byte] |= (1 << bit);
    }
    return status;
}

static inline cy_rslt_t cy_clear_bit(uint8_t* used, cy_resource_t type, uint8_t block, uint8_t channel)
{
    uint16_t bitPosition;
    cy_rslt_t status = cy_get_bit_position(type, block, channel, &bitPosition);
    if (status == CY_RSLT_SUCCESS)
    {
        uint8_t byte = bitPosition >> CY_BYTE_NUM_SHIFT;
        uint8_t bit = bitPosition & CY_BIT_NUM_MASK;
        used[byte] &= ~(1 << bit);
    }
    return status;
}

/*******************************************************************************
*       Hardware Manager API
*******************************************************************************/

cy_rslt_t cy_hwmgr_reserve(const cy_resource_inst_t* obj)
{
    core_util_critical_section_enter();
    bool isSet;
    cy_rslt_t rslt = cy_is_set(cy_used, obj->type, obj->block_num, obj->channel_num, &isSet);
    if (rslt == CY_RSLT_SUCCESS && isSet)
    {
        rslt = CY_RSLT_ERR_CSP_HWMGR_INUSE;
    }

    if (rslt == CY_RSLT_SUCCESS)
    {
        rslt = cy_set_bit(cy_used, obj->type, obj->block_num, obj->channel_num);
    }
    core_util_critical_section_exit();
    
    return rslt;
}

cy_rslt_t cy_hwmgr_free(const cy_resource_inst_t* obj)
{
    core_util_critical_section_enter();
    bool isUsed;
    cy_rslt_t rslt = cy_is_set(cy_used, obj->type, obj->block_num, obj->channel_num, &isUsed);
    if (rslt == CY_RSLT_SUCCESS && !isUsed)
    {
        rslt = CY_RSLT_WARN_CSP_HWMGR_UNUSED; 
    }
    if (rslt == CY_RSLT_SUCCESS)
    {
        rslt = cy_clear_bit(cy_used, obj->type, obj->block_num, obj->channel_num);
    }
    core_util_critical_section_exit();
    return rslt;
}

cy_rslt_t cy_hwmgr_allocate(cy_resource_t type, cy_resource_inst_t* obj)
{
    uint16_t offsetStartOfRsc = cy_get_resource_offset(type);
    uint16_t offsetEndOfRsc = ((1u + type) < sizeof(cy_resource_offsets)/sizeof(cy_resource_offsets[0]))
        ? cy_get_resource_offset((cy_resource_t)(type + 1))
        : CY_TOTAL_ALLOCATABLE_ITEMS;
    bool usesChannels = cy_uses_channels(type);

    uint16_t count = offsetEndOfRsc - offsetStartOfRsc;
    uint8_t block = 0;
    uint8_t channel = 0;
    for (uint16_t i = 0; i < count; i++)
    {
        cy_resource_inst_t res = { type, block, channel };
        if (CY_RSLT_SUCCESS == cy_hwmgr_reserve(&res))
        {
            obj->type = type;
            obj->block_num = block;
            obj->channel_num = channel;
            return CY_RSLT_SUCCESS;
        }
        else
        {
            if (usesChannels)
            {
                const uint8_t* blockOffsets = cy_get_block_offsets(type);
                uint16_t blocks = cy_get_block_offset_length(type);
                if ((block + 1) < blocks && blockOffsets[block + 1] == (i + 1))
                {
                    block++;
                    channel = 0;
                }
                else
                {
                    channel++;
                }
            }
            else
            {
                block++;
            }
        }
    }

    return CY_RSLT_ERR_CSP_HWMGR_NONE_FREE;
}

cy_rslt_t cy_hwmgr_allocate_clock(cy_clock_divider_t* obj, cy_clock_divider_types_t div, bool accept_larger)
{
    static uint8_t counts[] = { PERI_DIV_8_NR, PERI_DIV_16_NR, PERI_DIV_16_5_NR, PERI_DIV_24_5_NR };

    uint8_t block = (uint8_t)div;
    uint8_t count = counts[block];
    
    cy_rslt_t rslt = CY_RSLT_ERR_CSP_HWMGR_NONE_FREE;
    for (int i = 0; i < count; i++)
    {
        cy_resource_inst_t res = { CY_RSC_CLOCK, block, i };
        bool reserved = (CY_RSLT_SUCCESS == cy_hwmgr_reserve(&res));
        if (reserved)
        {
            obj->div_type = div;
            obj->div_num = i;
            return CY_RSLT_SUCCESS;
        }
    }
    if (CY_RSLT_SUCCESS != rslt && accept_larger && (div + 1u) < sizeof(counts))
    {
        rslt = cy_hwmgr_allocate_clock(obj, (cy_clock_divider_types_t)(div + 1), accept_larger);
    }

    return rslt;
}

cy_rslt_t cy_hwmgr_free_clock(cy_clock_divider_t* obj)
{
    cy_resource_inst_t res = { CY_RSC_CLOCK, obj->div_type, obj->div_num };
    return cy_hwmgr_free(&res);
}

cy_rslt_t cy_hwmgr_allocate_dma(cy_resource_inst_t* obj)
{
    return cy_hwmgr_allocate(CY_RSC_DMA, obj);
}

cy_rslt_t cy_hwmgr_set_configured(cy_resource_t type, uint8_t block, uint8_t channel)
{
    core_util_critical_section_enter();
    cy_rslt_t status = cy_set_bit(cy_configured, type, block, channel);
    core_util_critical_section_exit();
    return status;
}

cy_rslt_t cy_hwmgr_set_unconfigured(cy_resource_t type, uint8_t block, uint8_t channel)
{
    core_util_critical_section_enter();
    cy_rslt_t status = cy_clear_bit(cy_configured, type, block, channel);
    core_util_critical_section_exit();
    return status;
}

cy_rslt_t cy_hwmgr_is_configured(cy_resource_t type, uint8_t block, uint8_t channel, bool* isConfigured)
{
    // This doesn't modify anything, so no need for a critical section
    cy_rslt_t status = cy_is_set(cy_configured, type, block, channel, isConfigured);
    return status;
}
