/***************************************************************************//**
* \file cy_hw_types.h
*
* \brief
* Provides a struct definitions for configuration resources in the PDL.
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/** Resource types that the hardware manager supports */
typedef enum
{
    CY_RSC_ADC,       /*!< Analog to digital converter */
    CY_RSC_BLESS,     /*!< Bluetooth communications block */
    CY_RSC_CAN,       /*!< CAN communication block */
    CY_RSC_CLOCK,     /*!< Clock divider */
    CY_RSC_CRC,       /*!< CRC hardware accelerator */
    CY_RSC_DAC,       /*!< Digital to analog converter */
    CY_RSC_DMA,       /*!< DMA controller */
    CY_RSC_FLASH,     /*!< Flash memory */
    CY_RSC_GPIO,      /*!< General purpose I/O pin */
    CY_RSC_I2S,       /*!< I2S communications block */
    CY_RSC_LCD,       /*!< Segment LCD controller */
    CY_RSC_LPCOMP,    /*!< Low power comparator */
    CY_RSC_OPAMP,     /*!< Opamp */
    CY_RSC_PDM,       /*!< PCM/PDM communications block */
    CY_RSC_SMIF,      /*!< Quad-SPI communications block */
    CY_RSC_RNG,       /*!< Hardware random number generator */
    CY_RSC_RTC,       /*!< Real time clock */
    CY_RSC_SCB,       /*!< Serial Communications Block */
    CY_RSC_SDHC,      /*!< SD Host Controller */
    CY_RSC_TCPWM,     /*!< Timer/Counter/PWM block */
    CY_RSC_UDB,       /*!< UDB Array */
    CY_RSC_WDT,       /*!< Watchdog timer */
    CY_RSC_INVALID,   /*!< Placeholder for invalid type */
} cy_resource_t;

/** Represents a particular instance of a resource on the chip */
typedef struct
{
    cy_resource_t type;      //!< The resource block type
    uint8_t       block_num; //!< The resource block index
    /** 
      * The channel number, if the resource type defines multiple channels
      * per block instance. Otherwise, 0 */
    uint8_t       channel_num;
} cy_resource_inst_t;

#if defined(__cplusplus)
}
#endif /* __cplusplus */
