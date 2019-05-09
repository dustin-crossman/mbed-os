/*******************************************************************************
* File Name: cyhal_sdhc.c
*
* Description:
* Provides a high level interface for interacting with the Cypress SDHC. This 
* is a wrapper around the lower level PDL API.
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

#include <string.h> /* For memcpy */
#include "cyhal_sdhc.h"
#include "cyhal_sdio.h"
#include "cy_sysint.h"
#include "cy_sd_host.h"
#include "cy_gpio.h"
#include "cy_device.h"
#include "cy_result.h"
#include "cyhal_pin_package.h"
#include "cyhal_hwmgr.h"
#include "cyhal_utils.h"
#include "cyhal_interconnect.h"

#ifdef CY_IP_MXSDHC

#define CY_HAL_SDHC_RETRY_TIME               (1000UL)  /* The number loops to make the timeout in msec. */
#define CY_HAL_SDHC_FUJE_TIMEOUT_MS          (1000U)   /* The Fuje timeout for one block. */
#define CY_HAL_SDHC_READ_WRITE_TIMEOUT_MS    (500U)    /* The Read/Write timeout for one block. */
#define CY_HAL_SDHC_CMD_TIMEOUT_MS           (5U)      /* The Command timeout. */
#define CY_HAL_SDHC_TRANSFER_TIMEOUT         (0xCUL)   /* The transfer timeout. */
#define CY_HAL_SDHC_EMMC_TRIM_TIMEOUT        (100U)    /* The EMMC TRIM timeout. */
#ifndef SDIO_ENUMERATION_TIMEOUT_MS
    #define SDIO_ENUMERATION_TIMEOUT_MS      (500)
#endif

#define CY_SD_HOST_CLK_400K                 (400UL * 1000UL)    /* 400 kHz. */
#define SDHC_SDIO_64B_BLOCK                 (64U)

#define SDHC_IRQ_PRIORITY                   (3UL)
#define SDIO_IRQ_PRIORITY                   (3UL)

#define ENABLE_ALL_INTERRUPT_MASK           (0x0000FFFFUL)
#define ENABLE_ALL_INTERRUPT_ENABLE         (0x0000FFFFUL)
#define DISABLE_ALL_INTERRUPT_MASK          (0x0UL)
#define DISABLE_ALL_INTERRUPT_ENABLE        (0x0UL)

#define BIT_MASK( x )                       (( 1 << x ) - 1 )


/* List of available SDHC instances */
static SDHC_Type * sdhc_base_addresses[CY_IP_MXSDHC_INSTANCES] =
{
#ifdef SDHC0
    SDHC0,
#endif /* ifdef SMIF0 */
#ifdef SDHC1
    SDHC1,
#endif /* ifdef SMIF1 */
};

/* List of available SDHC interrupt sources */
static IRQn_Type CY_SDHC_IRQ_N[CY_IP_MXSDHC_INSTANCES] =
{
#ifdef SDHC0
    sdhc_0_interrupt_general_IRQn,
#endif /* ifdef SDHC0 */
#ifdef SDHC1
    sdhc_1_interrupt_general_IRQn,
#endif /* ifdef SDHC1 */
};


static cy_en_sd_host_status_t sdio_delay_milliseconds( uint32_t num_ms );
static cy_en_sd_host_status_t sdio_delay_milliseconds( uint32_t num_ms )
{
    Cy_SysLib_DelayUs(num_ms);

    return CY_SD_HOST_SUCCESS;
}

static cy_en_sd_host_bus_width_t convert_buswidth(uint8_t stopbits);
static cy_en_sd_host_bus_width_t convert_buswidth(uint8_t stopbits)
{
    switch (stopbits)
    {
        case 1:
            return CY_SD_HOST_BUS_WIDTH_1_BIT;
        case 4:
            return CY_SD_HOST_BUS_WIDTH_4_BIT;
        case 8:
            return CY_SD_HOST_BUS_WIDTH_8_BIT;
        default:
            CY_ASSERT(false);
            return CY_SD_HOST_BUS_WIDTH_1_BIT;
    }
}

static cy_en_sd_host_status_t Cy_SD_Host_SdCardChangeClock(SDHC_Type *base, uint32_t frequency);
/*******************************************************************************
* Function Name: Cy_SD_Host_SdCardChangeClock
****************************************************************************//**
*
*  Changes the Host controller SD clock.
*
* \param *base
*     The SD host registers structure pointer.
*
* \param frequency
*     The frequency in Hz.
*
* \return \ref cy_en_sd_host_status_t
*
*******************************************************************************/
static cy_en_sd_host_status_t Cy_SD_Host_SdCardChangeClock(SDHC_Type *base, uint32_t frequency)
{
    cy_en_sd_host_status_t ret = CY_SD_HOST_ERROR_INVALID_PARAMETER;
    uint32_t               clkDiv;
    uint32_t               clockInput = 100000000UL;

    if (NULL != base)
    {
        clkDiv = (clockInput / frequency) >> 1UL;
        Cy_SD_Host_DisableSdClk(base);
        ret = Cy_SD_Host_SetSdClkDiv(base, (uint16_t)clkDiv);
        Cy_SD_Host_EnableSdClk(base);
    }

    return ret;
}

static cy_en_sd_host_status_t Cy_SD_Host_PollCmdComplete(SDHC_Type *base);
/*******************************************************************************
* Function Name: Cy_SD_Host_PollCmdComplete
****************************************************************************//**
*
*  Waits for the command complete event.
*
* \param *base
*     The SD host registers structure pointer.
*
* \return \ref cy_en_sd_host_status_t
*
*******************************************************************************/
static cy_en_sd_host_status_t Cy_SD_Host_PollCmdComplete(SDHC_Type *base)
{
    cy_en_sd_host_status_t ret = CY_SD_HOST_ERROR_TIMEOUT;
    uint32_t               retry = CY_HAL_SDHC_RETRY_TIME;

    while (retry > 0UL)
    {
        /* Command complete */
        if (CY_SD_HOST_CMD_COMPLETE == (CY_SD_HOST_CMD_COMPLETE & Cy_SD_Host_GetNormalInterruptStatus(base)))
        {
            /* Clear interrupt flag */
            Cy_SD_Host_ClearNormalInterruptStatus(base, CY_SD_HOST_CMD_COMPLETE);

            ret = CY_SD_HOST_SUCCESS;
            break;
        }

        Cy_SysLib_DelayUs(CY_HAL_SDHC_CMD_TIMEOUT_MS);
        retry--;
    }

    return ret;
}

static cy_en_sd_host_status_t Cy_SD_Host_PollTransferComplete(SDHC_Type *base);
/*******************************************************************************
* Function Name: Cy_SD_Host_PollTransferComplete
****************************************************************************//**
*
*  Waits for the command complete event.
*
* \param *base
*     The SD host registers structure pointer.
*
* \return cy_en_sd_host_status_t
*     *base checking result.
*      If the pointer is NULL, returns error.
*
*******************************************************************************/
static cy_en_sd_host_status_t Cy_SD_Host_PollTransferComplete(SDHC_Type *base)
{
    cy_en_sd_host_status_t ret = CY_SD_HOST_ERROR_TIMEOUT;
    uint32_t               retry = CY_HAL_SDHC_RETRY_TIME;
    uint32_t               status = 0UL;

    while ((CY_SD_HOST_ERROR_TIMEOUT == ret) && (retry-- > 0u))
    {
        status = Cy_SD_Host_GetNormalInterruptStatus(base);

        /* Transfer complete */
        if (CY_SD_HOST_XFER_COMPLETE == (CY_SD_HOST_XFER_COMPLETE & status))
        {
            /* Clear interrupt flag */
            Cy_SD_Host_ClearNormalInterruptStatus(base, CY_SD_HOST_XFER_COMPLETE);

            ret = CY_SD_HOST_SUCCESS;
            break;
        }

        Cy_SysLib_DelayUs(CY_HAL_SDHC_READ_WRITE_TIMEOUT_MS);
    }

    return ret;
}

/* Disconnect (interconnect) pin and free it (hwmgr) as resource */
static cy_rslt_t cyhal_sdhc_free_pin(cyhal_gpio_t pin)
{
    cyhal_resource_inst_t rsc;
    cy_rslt_t result = cyhal_disconnect_pin(pin);
    if (CY_RSLT_SUCCESS == result)
    {
        rsc = cyhal_utils_get_gpio_resource(pin);
        result = cyhal_hwmgr_free(&rsc);
    }
    return result;
}

cy_rslt_t cyhal_sdhc_init(cyhal_sdhc_t *obj,
                    cyhal_gpio_t cmd,
                    cyhal_gpio_t clk,
                    cyhal_gpio_t data0,
                    cyhal_gpio_t data1,
                    cyhal_gpio_t data2,
                    cyhal_gpio_t data3,
                    cyhal_gpio_t data4,
                    cyhal_gpio_t data5,
                    cyhal_gpio_t data6,
                    cyhal_gpio_t data7,
                    cyhal_gpio_t cardDetect,
                    cyhal_gpio_t ioVoltSel,
                    cyhal_gpio_t cardIfPwrEn,
                    cyhal_gpio_t cardMechWriteProt,
                    cyhal_gpio_t ledCtrl,
                    cyhal_gpio_t cardEmmcReset)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    cy_stc_sd_host_init_config_t   hostConfig;
    cyhal_resource_inst_t pin_rsc;
    const cyhal_resource_pin_mapping_t *map;

    /* Reserve SDHC */
    map = CY_UTILS_GET_RESOURCE(cmd, cyhal_pin_map_sdhc_card_cmd);
    if (NULL != map)
    {
        obj->resource = *map->inst;
        result = cyhal_hwmgr_reserve(&obj->resource);
    }

    /* Reserve and configure GPIO pins. */
    if (result == CY_RSLT_SUCCESS)
    {
        pin_rsc = cyhal_utils_get_gpio_resource(cmd);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {    
            map = CY_UTILS_GET_RESOURCE(cmd, cyhal_pin_map_sdhc_card_cmd);
            result = cyhal_connect_pin(map);
        }
    }

    if (result == CY_RSLT_SUCCESS)
    {
        pin_rsc = cyhal_utils_get_gpio_resource(clk);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {    
            map = CY_UTILS_GET_RESOURCE(clk, cyhal_pin_map_sdhc_clk_card);
            result = cyhal_connect_pin(map);
        }
    }
    
    if (result == CY_RSLT_SUCCESS)
    {
        pin_rsc = cyhal_utils_get_gpio_resource(data0);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {    
            map = CY_UTILS_GET_RESOURCE(data0, cyhal_pin_map_sdhc_card_dat_3to0);
            result = cyhal_connect_pin(map);
        }
    }
    
    if ((NC != data1) && (CY_RSLT_SUCCESS == result))
    {
        pin_rsc = cyhal_utils_get_gpio_resource(data1);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {    
            map = CY_UTILS_GET_RESOURCE(data1, cyhal_pin_map_sdhc_card_dat_3to0);
            result = cyhal_connect_pin(map);
        }
    }

    if ((NC != data2) && (CY_RSLT_SUCCESS == result))
    {
        pin_rsc = cyhal_utils_get_gpio_resource(data2);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {    
            map = CY_UTILS_GET_RESOURCE(data2, cyhal_pin_map_sdhc_card_dat_3to0);
            result = cyhal_connect_pin(map);
        }
    }

    if ((NC != data3) && (CY_RSLT_SUCCESS == result))
    {
        pin_rsc = cyhal_utils_get_gpio_resource(data3);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {    
            map = CY_UTILS_GET_RESOURCE(data3, cyhal_pin_map_sdhc_card_dat_3to0);
            result = cyhal_connect_pin(map);
        }
    }

    if ((NC != data4) && (CY_RSLT_SUCCESS == result))
    {
        pin_rsc = cyhal_utils_get_gpio_resource(data4);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {    
            map = CY_UTILS_GET_RESOURCE(data4, cyhal_pin_map_sdhc_card_dat_7to4);
            result = cyhal_connect_pin(map);
        }
    }

    if ((NC != data5) && (CY_RSLT_SUCCESS == result))
    {
        pin_rsc = cyhal_utils_get_gpio_resource(data5);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {    
            map = CY_UTILS_GET_RESOURCE(data5, cyhal_pin_map_sdhc_card_dat_7to4);
            result = cyhal_connect_pin(map);
        }
    }

    if ((NC != data6) && (CY_RSLT_SUCCESS == result))
    {
        pin_rsc = cyhal_utils_get_gpio_resource(data6);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {    
            map = CY_UTILS_GET_RESOURCE(data6, cyhal_pin_map_sdhc_card_dat_7to4);
            result = cyhal_connect_pin(map);
        }
    }

    if ((NC != data7) && (CY_RSLT_SUCCESS == result))
    {
        pin_rsc = cyhal_utils_get_gpio_resource(data7);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {    
            map = CY_UTILS_GET_RESOURCE(data7, cyhal_pin_map_sdhc_card_dat_7to4);
            result = cyhal_connect_pin(map);
        }
    }
    
    if ((NC != cardDetect) && (CY_RSLT_SUCCESS == result))
    {
        pin_rsc = cyhal_utils_get_gpio_resource(cardDetect);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {    
            map = CY_UTILS_GET_RESOURCE(cardDetect, cyhal_pin_map_sdhc_card_detect_n);
            result = cyhal_connect_pin(map);
        }
    }
    
    if ((NC != ioVoltSel) && (CY_RSLT_SUCCESS == result))
    {
        pin_rsc = cyhal_utils_get_gpio_resource(ioVoltSel);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {    
            map = CY_UTILS_GET_RESOURCE(ioVoltSel, cyhal_pin_map_sdhc_io_volt_sel);
            result = cyhal_connect_pin(map);
        }
    }
    
    if ((NC != cardIfPwrEn) && (CY_RSLT_SUCCESS == result))
    {
        pin_rsc = cyhal_utils_get_gpio_resource(cardIfPwrEn);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {    
            map = CY_UTILS_GET_RESOURCE(cardIfPwrEn, cyhal_pin_map_sdhc_card_if_pwr_en);
            result = cyhal_connect_pin(map);
        }
    }
    
    if ((NC != cardMechWriteProt) && (CY_RSLT_SUCCESS == result))
    {
        pin_rsc = cyhal_utils_get_gpio_resource(cardMechWriteProt);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {    
            map = CY_UTILS_GET_RESOURCE(cardMechWriteProt, cyhal_pin_map_sdhc_card_mech_write_prot);
            result = cyhal_connect_pin(map);
        }
    }
    
    if ((NC != ledCtrl) && (CY_RSLT_SUCCESS == result))
    {
        pin_rsc = cyhal_utils_get_gpio_resource(ledCtrl);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {    
            map = CY_UTILS_GET_RESOURCE(ledCtrl, cyhal_pin_map_sdhc_led_ctrl);
            result = cyhal_connect_pin(map);
        }
    }
    
    if ((NC != cardEmmcReset) && (CY_RSLT_SUCCESS == result))
    {
        pin_rsc = cyhal_utils_get_gpio_resource(cardEmmcReset);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {    
            map = CY_UTILS_GET_RESOURCE(cardEmmcReset, cyhal_pin_map_sdhc_card_emmc_reset_n);
            result = cyhal_connect_pin(map);
        }
    }
    
    if (CY_RSLT_SUCCESS == result)
    {
        /* Enable the SDHC block. */
        Cy_SD_Host_Enable(obj->base);
    
        /* Configure SD Host to operate */
        hostConfig.dmaType = obj->dmaType;
        hostConfig.enableLedControl = obj->enableLedControl;
        hostConfig.emmc = obj->emmc;
        obj->context.cardType = CY_SD_HOST_NOT_EMMC;

        result = Cy_SD_Host_Init(obj->base, &hostConfig, &obj->context);
    }

    if (CY_SD_HOST_SUCCESS != result)
        {
            result = CY_RSLT_TYPE_ERROR;
        }
    return result;
}

cy_rslt_t cyhal_sdhc_free(cyhal_sdhc_t *obj)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    bool configured = false;

    /* Free SDHC */
    if (CYHAL_RSC_INVALID != obj->resource.type)
    {
        result = cyhal_hwmgr_free(&obj->resource);
    }

    /* Free pins */
    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_free_pin(obj->pin_cmd);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_free_pin(obj->pin_clk);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_free_pin(obj->pin_data0);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_free_pin(obj->pin_data1);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_free_pin(obj->pin_data2);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_free_pin(obj->pin_data3);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_free_pin(obj->pin_data4);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_free_pin(obj->pin_data5);
    }
    
    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_free_pin(obj->pin_data6);
    }
    
    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_free_pin(obj->pin_data7);
    }
    
    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_free_pin(obj->pin_cardDetect);
    }
    
    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_free_pin(obj->pin_ioVoltSel);
    }
    
    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_free_pin(obj->pin_cardIfPwrEn);
    }
    
    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_free_pin(obj->pin_cardMechWriteProt);
    }
    
    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_free_pin(obj->pin_ledCtrl);
    }
    
    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_free_pin(obj->pin_cardEmmcReset);
    }

    Cy_SD_Host_DeInit(obj->base);

    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_hwmgr_is_configured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num, &configured);
        if ((CY_RSLT_SUCCESS == result) && configured)
        {
            result = cyhal_hwmgr_set_unconfigured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
        }
    }

    return result;
}

cy_rslt_t cyhal_sdhc_configure(cyhal_sdhc_t *obj, const cyhal_sdhc_config_t *config)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet = CY_SD_HOST_SUCCESS;
    cy_stc_sd_host_sd_card_config_t stcSdcardCfg;
    cy_en_sd_host_card_capacity_t cardCapacity;
    uint32_t rca = 0UL;
    cy_en_sd_host_card_type_t cardType = CY_SD_HOST_UNUSABLE;
    bool configured = false;

    ret = cyhal_hwmgr_is_configured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num, &configured);
    
    if ((CY_RSLT_SUCCESS == ret) && !configured)
    {
        /** Initialize the SDIO/SD/eMMC card configuration structure */
        stcSdcardCfg.lowVoltageSignaling = config->lowVoltageSignaling;
        stcSdcardCfg.busWidth = convert_buswidth(config->busWidth);
        stcSdcardCfg.cardType = &cardType;
        stcSdcardCfg.rca = &rca;
        stcSdcardCfg.cardCapacity = &cardCapacity;

        /** Initialize the card */
        driverRet = Cy_SD_Host_InitCard(obj->base, &stcSdcardCfg, &obj->context);
        if ((CY_SD_HOST_SUCCESS != driverRet) && (ret == CY_RSLT_SUCCESS))
        {
            ret = CY_RSLT_TYPE_ERROR;
        }
        else
        {
            ret = cyhal_hwmgr_set_configured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
        }
    }
    return ret;
}

cy_rslt_t cyhal_sdhc_read(const cyhal_sdhc_t *obj, uint32_t address, uint8_t *data, size_t *length)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet;
    cy_stc_sd_host_write_read_config_t dataConfig;

    dataConfig.data = (uint32_t*)data;   /* The pointer to data. */
    dataConfig.address = address;        /* The address to write/read data on the card or eMMC. */
    dataConfig.numberOfBlocks = (uint32_t)*length;  /* The number of blocks to write/read. */
    if (1UL == (uint32_t)*length)
    {
        dataConfig.autoCommand = CY_SD_HOST_AUTO_CMD_NONE;  /* Selects which auto commands are used if any. */
    }
    else
    {
        dataConfig.autoCommand = CY_SD_HOST_AUTO_CMD_AUTO;  /* Selects which auto commands are used if any. */
    }
    dataConfig.dataTimeout = CY_HAL_SDHC_TRANSFER_TIMEOUT;      /* The timeout value for the transfer. */
    dataConfig.enReliableWrite = false; /* For EMMC cards enable reliable write. */
    dataConfig.enableDma = true;

    driverRet = Cy_SD_Host_Read(obj->base, &dataConfig, &obj->context);

    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    driverRet = Cy_SD_Host_PollTransferComplete(obj->base);

    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    return ret;
}

cy_rslt_t cyhal_sdhc_write(const cyhal_sdhc_t *obj, uint32_t address, const uint8_t *data, size_t *length)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet;
    cy_stc_sd_host_write_read_config_t dataConfig;

    dataConfig.data = (uint32_t*)data;   /* The pointer to data. */
    dataConfig.address = address;        /* The address to write/read data on the card or eMMC. */
    dataConfig.numberOfBlocks = (uint32_t)*length;  /* The number of blocks to write/read. */
    if (1UL == (uint32_t)*length)
    {
        dataConfig.autoCommand = CY_SD_HOST_AUTO_CMD_NONE;  /* Selects which auto commands are used if any. */
    }
    else
    {
        dataConfig.autoCommand = CY_SD_HOST_AUTO_CMD_AUTO;  /* Selects which auto commands are used if any. */
    }
    dataConfig.dataTimeout = CY_HAL_SDHC_TRANSFER_TIMEOUT;  /* The timeout value for the transfer. */
    dataConfig.enReliableWrite = false; /* For EMMC cards enable reliable write. */
    dataConfig.enableDma = true;
    
    driverRet = Cy_SD_Host_Write(obj->base, &dataConfig, &obj->context);

    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    driverRet = Cy_SD_Host_PollTransferComplete(obj->base);

    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    return ret;
}

cy_rslt_t cyhal_sdhc_erase(const cyhal_sdhc_t *obj, uint32_t startAddr, size_t length)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet;
    cy_en_sd_host_erase_type_t eraseType = CY_SD_HOST_ERASE_ERASE;
    uint32_t i = CY_HAL_SDHC_RETRY_TIME;
    uint32_t cardStatus;

    if (obj->emmc)
    {
         eraseType = CY_SD_HOST_ERASE_TRIM;
    }

    driverRet = Cy_SD_Host_Erase(obj->base, startAddr,
            (startAddr + length), eraseType, &obj->context);

    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    driverRet = Cy_SD_Host_PollCmdComplete(obj->base);

    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    if (false == obj->emmc)
    {
        /* polling result */
        driverRet = CY_SD_HOST_ERROR;

        while (i-- > 0UL)
        {
            cardStatus = Cy_SD_Host_GetCardStatus(obj->base, &obj->context);
            if (((CY_SD_HOST_CARD_TRAN << CY_SD_HOST_CMD13_CURRENT_STATE) |
                (1UL << CY_SD_HOST_CMD13_READY_FOR_DATA)) == cardStatus)
            {
                driverRet = CY_SD_HOST_SUCCESS;
                break;
            }

            Cy_SysLib_DelayUs(CY_HAL_SDHC_FUJE_TIMEOUT_MS); /* The Fuje timeout for one block. */
        }

        if (CY_SD_HOST_SUCCESS != driverRet)
        {
            ret = CY_RSLT_TYPE_ERROR;
        }
    }
    else
    {
        Cy_SysLib_Delay(CY_HAL_SDHC_EMMC_TRIM_TIMEOUT);
    }

    return ret;
}

cy_rslt_t cyhal_sdhc_read_async(const cyhal_sdhc_t *obj, uint32_t address, uint8_t *data, size_t *length)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    //TODO: implement
    return ret;
}

cy_rslt_t cyhal_sdhc_write_async(const cyhal_sdhc_t *obj, uint32_t address, const uint8_t *data, size_t *length)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    //TODO: implement
    return ret;
}

cy_rslt_t cyhal_sdhc_is_busy(const cyhal_sdhc_t *obj, bool *busy)
{
    *busy = (CY_SD_HOST_XFER_COMPLETE != (CY_SD_HOST_XFER_COMPLETE & Cy_SD_Host_GetNormalInterruptStatus(obj->base)));

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_sdhc_abort_async(const cyhal_sdhc_t *obj)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet;

    driverRet = Cy_SD_Host_AbortTransfer(obj->base, &obj->context);
    
    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    return ret;
}

cy_rslt_t cyhal_sdhc_register_irq(cyhal_sdhc_t *obj, cyhal_sdhc_irq_handler handler, void *handler_arg)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    IRQn_Type irqn = CY_SDHC_IRQ_N[obj->resource.block_num];

    /* Only enable if it's not already enabled */
    if (NVIC_GetEnableIRQ(irqn) == 0)
    {
        cy_stc_sysint_t irqCfg = {irqn, SDHC_IRQ_PRIORITY};

        Cy_SysInt_Init(&irqCfg, (cy_israddress)handler);
        NVIC_EnableIRQ(irqn);
    }
    else
    {
        NVIC_SetPriority(irqn, SDHC_IRQ_PRIORITY);
    }   

    return ret;
}

cy_rslt_t cyhal_sdhc_irq_enable(cyhal_sdhc_t *obj, cyhal_sdhc_irq_event_t event, bool enable)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    uint32_t interruptMask;
    uint32_t interruptEnable;

    interruptEnable = Cy_SD_Host_GetNormalInterruptEnable(obj->base);
    interruptMask = Cy_SD_Host_GetNormalInterruptMask(obj->base);

    if(enable == true) 
    {
        /* Enable specific interrupt */
        if(event < CYHAL_SDHC_ERR_INTERRUPT)
        {
            interruptEnable |= (1 << event);
            interruptMask |= (1 << event);
        }
        /* Enable error interrupts */
        else if (event == CYHAL_SDHC_ERR_INTERRUPT)
        {
            Cy_SD_Host_SetErrorInterruptEnable(obj->base, ENABLE_ALL_INTERRUPT_ENABLE);
            Cy_SD_Host_SetErrorInterruptMask(obj->base, ENABLE_ALL_INTERRUPT_MASK);
        }
        /* Enable all interrupts */
        else if (event == CYHAL_SDHC_ALL_INTERRUPTS)
        {
            interruptEnable = ENABLE_ALL_INTERRUPT_ENABLE;
            interruptMask = ENABLE_ALL_INTERRUPT_MASK;
            Cy_SD_Host_SetErrorInterruptEnable(obj->base, ENABLE_ALL_INTERRUPT_ENABLE);
            Cy_SD_Host_SetErrorInterruptMask(obj->base, ENABLE_ALL_INTERRUPT_MASK);
        }
    }
    /* Disable interrupt */
    else
    {
        /* Disable specific interrupt */
        if(event < CYHAL_SDHC_ERR_INTERRUPT)
        {
            interruptEnable &= ~(1 << event);
            interruptMask &= ~(1 << event);
        }
        /* Disable error interrupts */
        else if (event == CYHAL_SDHC_ERR_INTERRUPT)
        {
            Cy_SD_Host_SetErrorInterruptEnable(obj->base, DISABLE_ALL_INTERRUPT_ENABLE);
            Cy_SD_Host_SetErrorInterruptMask(obj->base, DISABLE_ALL_INTERRUPT_MASK);
        }
        /* Disable all interrupts */
        else if (event == CYHAL_SDHC_ALL_INTERRUPTS)
        {
            interruptEnable = DISABLE_ALL_INTERRUPT_ENABLE;
            interruptMask = DISABLE_ALL_INTERRUPT_MASK;
            Cy_SD_Host_SetErrorInterruptEnable(obj->base, DISABLE_ALL_INTERRUPT_ENABLE);
            Cy_SD_Host_SetErrorInterruptMask(obj->base, DISABLE_ALL_INTERRUPT_MASK);
        }
    }

    Cy_SD_Host_SetNormalInterruptMask(obj->base, interruptMask);
    Cy_SD_Host_SetNormalInterruptEnable(obj->base, interruptEnable);

    return ret;
}

cy_rslt_t cyhal_sdio_init(cyhal_sdio_t *obj, cyhal_gpio_t cmd, cyhal_gpio_t clk, cyhal_gpio_t data0, cyhal_gpio_t data1, cyhal_gpio_t data2, cyhal_gpio_t data3)
{
    cy_stc_sd_host_init_config_t hostConfig;
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    GPIO_PRT_Type *port;
    const uint32_t outputVal = 0UL;
    cy_stc_sd_host_context_t context;

    /* TODO need to take it from manager? */
    obj->base = sdhc_base_addresses[0];

      /*This is for WiFi reset pin, and will put the part in the reset */
    port = Cy_GPIO_PortToAddr(CYHAL_GET_PORT(obj->resetPin));
    Cy_GPIO_Pin_FastInit(port, CYHAL_GET_PIN(obj->resetPin), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_GPIO);

    /* Enable the SDHC block. */
    Cy_SD_Host_Enable(obj->base);

    /* Configure GPIO pins. */
    port = Cy_GPIO_PortToAddr(CYHAL_GET_PORT(cmd));
    Cy_GPIO_Pin_FastInit(port, CYHAL_GET_PIN(cmd), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);

    port = Cy_GPIO_PortToAddr(CYHAL_GET_PORT(clk));
    Cy_GPIO_Pin_FastInit(port, CYHAL_GET_PIN(clk), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);

    port = Cy_GPIO_PortToAddr(CYHAL_GET_PORT(data0));
    Cy_GPIO_Pin_FastInit(port, CYHAL_GET_PIN(data0), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);

    if (NC != data1)
    {
        port = Cy_GPIO_PortToAddr(CYHAL_GET_PORT(data1));
        Cy_GPIO_Pin_FastInit(port, CYHAL_GET_PIN(data1), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);
    }

    if (NC != data2)
    {
        port = Cy_GPIO_PortToAddr(CYHAL_GET_PORT(data2));
        Cy_GPIO_Pin_FastInit(port, CYHAL_GET_PIN(data2), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);
    }

    if (NC != data3)
    {
        port = Cy_GPIO_PortToAddr(CYHAL_GET_PORT(data3));
        Cy_GPIO_Pin_FastInit(port, CYHAL_GET_PIN(data3), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);
    }

    hostConfig.dmaType = CY_SD_HOST_DMA_SDMA;
    hostConfig.emmc = false;
    hostConfig.enableLedControl = false;
    context.cardType = CY_SD_HOST_SDIO;

    /* Configure SD Host to operate */
    (void) Cy_SD_Host_Init(obj->base, &hostConfig, &context);

    /*Only enable the Card and SDMA interrupt*/
    Cy_SD_Host_SetNormalInterruptMask(obj->base, (CY_SD_HOST_CARD_INTERRUPT | CY_SD_HOST_DMA_INTERRUPT));

    /*Don't enable any error interrupts for now*/
    Cy_SD_Host_SetErrorInterruptMask(obj->base, 0UL);

    /*Clear all interrupts*/
    Cy_SD_Host_ClearErrorInterruptStatus(obj->base, 0xffff);
    Cy_SD_Host_ClearNormalInterruptStatus(obj->base, 0xffff);

    (void)Cy_SD_Host_SetHostBusWidth(obj->base, CY_SD_HOST_BUS_WIDTH_4_BIT);

    /* Change the host SD clock to 400 kHz */
    (void)Cy_SD_Host_SdCardChangeClock(obj->base, CY_SD_HOST_CLK_400K);

    return ret;
}

cy_rslt_t cyhal_sdio_free(cyhal_sdio_t *obj)
{
    Cy_SD_Host_DeInit(obj->base);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_sdio_configure(cyhal_sdio_t *obj, const cyhal_sdio_cfg_t *config)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t result;
    uint32_t    rca, loop_count = 0UL;
    cy_stc_sd_host_cmd_config_t cmd;

    rca = obj->context.RCA;

    do
    {
        /* Send CMD0 to set it to idle state */
        cmd.commandIndex                    = (uint32_t)CYHAL_SDIO_CMD_GO_IDLE_STATE;
        cmd.commandArgument                 = (uint32_t)0;
        cmd.enableCrcCheck                  = false;
        cmd.enableAutoResponseErrorCheck    = false;
        cmd.respType                        = CY_SD_HOST_RESPONSE_NONE;
        cmd.enableIdxCheck                  = false;
        cmd.dataPresent                     = false;
        cmd.cmdType                         = CY_SD_HOST_CMD_NORMAL;

        (void)Cy_SD_Host_SendCommand(obj->base, &cmd);
        (void)Cy_SD_Host_PollCmdComplete(obj->base);

        /* CMD5 */
        cmd.commandIndex                    = (uint32_t)CYHAL_SDIO_CMD_IO_SEND_OP_COND;
        cmd.commandArgument                 = (uint32_t)0;
        cmd.enableCrcCheck                  = false;
        cmd.enableAutoResponseErrorCheck    = false;
        cmd.respType                        = CY_SD_HOST_RESPONSE_LEN_48;
        cmd.enableIdxCheck                  = false;
        cmd.dataPresent                     = false;
        cmd.cmdType                         = CY_SD_HOST_CMD_NORMAL;

        (void)Cy_SD_Host_SendCommand(obj->base, &cmd);
        (void)Cy_SD_Host_PollCmdComplete(obj->base);

        /* Send CMD3 to get RCA. */
        cmd.commandIndex                    = (uint32_t)CYHAL_SDIO_CMD_SEND_RELATIVE_ADDR;
        cmd.commandArgument                 = (uint32_t)0;
        cmd.enableCrcCheck                  = true;
        cmd.enableAutoResponseErrorCheck    = false;
        cmd.respType                        = CY_SD_HOST_RESPONSE_LEN_48;
        cmd.enableIdxCheck                  = true;
        cmd.dataPresent                     = false;
        cmd.cmdType                         = CY_SD_HOST_CMD_NORMAL;

        result = Cy_SD_Host_SendCommand(obj->base, &cmd);

        if (CY_SD_HOST_SUCCESS == result)
        {
            result = Cy_SD_Host_PollCmdComplete(obj->base);

            (void)Cy_SD_Host_GetResponse(obj->base, &rca, false);
        }

        loop_count++;
        if ( loop_count >= (uint32_t) SDIO_ENUMERATION_TIMEOUT_MS )
        {
            result = CY_SD_HOST_ERROR_TIMEOUT;
            break;
        }
    } while ( ( CY_SD_HOST_SUCCESS != result ) && ( sdio_delay_milliseconds( (uint32_t) 1 ), ( 1 == 1 ) ) );

    if (CY_SD_HOST_SUCCESS == result)
    {
        /* Send CMD7 with the returned RCA to select the card */
        cmd.commandIndex                    = (uint32_t)CYHAL_SDIO_CMD_SELECT_CARD;
        cmd.commandArgument                 = (uint32_t)rca;
        cmd.enableCrcCheck                  = true;
        cmd.enableAutoResponseErrorCheck    = false;
        cmd.respType                        = CY_SD_HOST_RESPONSE_LEN_48B;
        cmd.enableIdxCheck                  = true;
        cmd.dataPresent                     = false;
        cmd.cmdType                         = CY_SD_HOST_CMD_NORMAL;

        result = Cy_SD_Host_SendCommand(obj->base, &cmd);

        if (CY_SD_HOST_SUCCESS == result)
        {
            result = Cy_SD_Host_PollCmdComplete(obj->base);
        }
    }

    if (CY_SD_HOST_SUCCESS != result)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    return ret;
}

cy_rslt_t cyhal_sdio_send_cmd(const cyhal_sdio_t *obj, cyhal_transfer_t direction, cyhal_sdio_command_t command, uint32_t argument, uint32_t* response)
{
    cy_rslt_t                   ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t      result = CY_SD_HOST_ERROR_TIMEOUT;
    cy_stc_sd_host_cmd_config_t cmd;
    uint32_t                    retry = 50UL;

    /* Clear out the response */
    if ( response != NULL )
    {
        *response = 0UL;
    }

    /* TODO Need to have already pulled correct obj->base element */
    while ((CY_SD_HOST_SUCCESS != result) && (retry-- > 0UL))
    {
        /* First clear out the command complete and transfer complete statuses */
        Cy_SD_Host_ClearNormalInterruptStatus(obj->base, (CY_SD_HOST_XFER_COMPLETE | CY_SD_HOST_CMD_COMPLETE));

        /* Check if an error occurred on any previous transactions */
        if( Cy_SD_Host_GetNormalInterruptStatus(obj->base) & CY_SD_HOST_ERR_INTERRUPT )
        {
            /* Reset the block if there was an error.  Note a full reset usually
             * requires more time, but this short version is working quite well and
             * successfully clears out the error state.
             */
            Cy_SD_Host_ClearErrorInterruptStatus(obj->base, 0xffffU);
            Cy_SD_Host_SoftwareReset(obj->base, CY_SD_HOST_RESET_CMD_LINE);
            Cy_SD_Host_SoftwareReset(obj->base, CY_SD_HOST_RESET_DATALINE);
            Cy_SysLib_DelayUs(1);
            obj->base->CORE.SW_RST_R = 0x00;
        }

        cmd.commandIndex                    = (uint32_t)command;
        cmd.commandArgument                 = argument;
        cmd.enableCrcCheck                  = true;
        cmd.enableAutoResponseErrorCheck    = false;
        cmd.respType                        = CY_SD_HOST_RESPONSE_LEN_48;
        cmd.enableIdxCheck                  = true;
        cmd.dataPresent                     = false;
        cmd.cmdType                         = CY_SD_HOST_CMD_NORMAL;

        result = Cy_SD_Host_SendCommand(obj->base, &cmd);

        result = Cy_SD_Host_PollCmdComplete(obj->base);
    }

    (void)Cy_SD_Host_GetResponse(obj->base, response, false);

    if (CY_SD_HOST_SUCCESS != result)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    /* This interrupt is disabled in interrupt handler so need to enable it here */
    Cy_SD_Host_SetNormalInterruptEnable(obj->base, (Cy_SD_Host_GetNormalInterruptEnable(obj->base) | CY_SD_HOST_CARD_INTERRUPT));
    return ret;
}

cy_rslt_t cyhal_sdio_bulk_transfer(cyhal_sdio_t *obj, cyhal_transfer_t direction, uint32_t argument, const uint32_t* data, uint16_t length, uint32_t* response)
{
    cy_rslt_t                    ret = CY_RSLT_SUCCESS;
    uint32_t                     retry = 50UL;
    const uint32_t               block_size = SDHC_SDIO_64B_BLOCK;
    cy_stc_sd_host_cmd_config_t  cmd;
    cy_stc_sd_host_data_config_t dat;
    cy_en_sd_host_status_t       result = CY_SD_HOST_ERROR_TIMEOUT;
    cyhal_sdio_transfer_mode_t      mode = (64U <= length) ? CYHAL_SDIO_BLOCK_MODE : CYHAL_SDIO_BYTE_MODE;

    /* Commented it out cose of IAR warnings. Currently WHD put 64b aligned 
    *  length so we should be fine
       static uint32_t temp_Buffer[length + 64 - 1];
    */

    /* Initialize data constants*/
    dat.autoCommand         = CY_SD_HOST_AUTO_CMD_NONE;
    dat.dataTimeout         = 0x0dUL;
    dat.enableIntAtBlockGap = false;
    dat.enReliableWrite     = false;
    dat.enableDma           = true;

    /*Clear out the response*/
    if ( response != NULL )
    {
        *response = 0UL;
    }

    while ((CY_SD_HOST_SUCCESS != result) && (retry-- > 0UL))
    {
        /*  Add SDIO Error Handling
         *  SDIO write timeout is expected when doing first write to register after KSO bit disable (as it goes to AOS core).
         *  This timeout, however, triggers an error state in the hardware.  So, check for the error and then recover from it
         *  as needed via reset issuance.  This is the only time known that a write timeout occurs.
         */

        /* First clear out the command complete and transfer complete statuses */
        Cy_SD_Host_ClearNormalInterruptStatus(obj->base, (CY_SD_HOST_XFER_COMPLETE | CY_SD_HOST_CMD_COMPLETE));

        /* Check if an error occurred on any previous transactions */
        if( Cy_SD_Host_GetNormalInterruptStatus(obj->base) & CY_SD_HOST_ERR_INTERRUPT )
        {
            /* Reset the block if there was an error.  Note a full reset usually
             * requires more time, but this short version is working quite well and
             * successfully clears out the error state.
             */
            Cy_SD_Host_ClearErrorInterruptStatus(obj->base, 0xffffU);
            Cy_SD_Host_SoftwareReset(obj->base,    CY_SD_HOST_RESET_CMD_LINE);
            Cy_SD_Host_SoftwareReset(obj->base,    CY_SD_HOST_RESET_DATALINE);
            Cy_SysLib_DelayUs(1);
            obj->base->CORE.SW_RST_R = 0x00;
        }

        /* Prepare the data transfer register */
        cmd.commandIndex                    = 53u;
        cmd.commandArgument                 = argument;
        cmd.enableCrcCheck                  = true;
        cmd.enableAutoResponseErrorCheck    = false;
        cmd.respType                        = CY_SD_HOST_RESPONSE_LEN_48;
        cmd.enableIdxCheck                  = true;
        cmd.dataPresent                     = true;
        cmd.cmdType                         = CY_SD_HOST_CMD_NORMAL;
        dat.data                            = (uint32_t*)data;

        dat.read = ( direction == CYHAL_WRITE ) ? false : true;

        /* TODO - fix as is in udb sdio or try to have two transitions */
        if( mode == CYHAL_SDIO_BLOCK_MODE )
        {
            dat.blockSize         = block_size;
            dat.numberOfBlock     = ( length + block_size - 1 ) / block_size;
              
            /* Commented it out cose of IAR warnings. Currently WHD put 64b aligned 
            *  length so we should be fine
            *
              
              if (dat.read)
              {
                  dat.data = (uint32_t*) temp_Buffer;
              }
              */
        }
        else
        {
              dat.blockSize       = length;
              dat.numberOfBlock   = 1UL;
        }

       /*Disable XFER Done interrupt*/
       obj->base->CORE.NORMAL_INT_SIGNAL_EN_R  &= (uint16_t)~CY_SD_HOST_XFER_COMPLETE;

       (void)Cy_SD_Host_InitDataTransfer(obj->base, &dat);

       result = Cy_SD_Host_SendCommand(obj->base, &cmd);
       if ( CY_SD_HOST_SUCCESS == result )
       {
           result = Cy_SD_Host_PollCmdComplete(obj->base);

           if ( CY_SD_HOST_SUCCESS == result )
           {
               result = Cy_SD_Host_PollTransferComplete(obj->base);
           }

       }
    }

    if (response != NULL )
    {
        (void)Cy_SD_Host_GetResponse(obj->base, response, false);
    }

    if (CY_SD_HOST_SUCCESS != result)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    /* Commented it out cose of IAR warnings. Currently WHD put 64b aligned 
    *  length so we should be fine
    *
    if(result == CY_RSLT_SUCCESS && mode == CYHAL_SDIO_BLOCK_MODE && dat.read)
    {
        memcpy((uint32_t *)data, temp_Buffer, (size_t)length);
    }
    */
    
    /* This interrupt is disabled in interrupt handler so need to enable it here */
    Cy_SD_Host_SetNormalInterruptEnable(obj->base, (Cy_SD_Host_GetNormalInterruptEnable(obj->base) | CY_SD_HOST_CARD_INTERRUPT));

    return ret;
}

cy_rslt_t cyhal_sdio_transfer_async(cyhal_sdio_t *obj, cyhal_transfer_t direction, uint32_t argument, const uint32_t* data, uint16_t length)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    /* TODO: implement */
    return ret;
}

cy_rslt_t cyhal_sdio_is_busy(const cyhal_sdio_t *obj, bool *busy)
{
    *busy = (CY_SD_HOST_XFER_COMPLETE != (CY_SD_HOST_XFER_COMPLETE & Cy_SD_Host_GetNormalInterruptStatus(obj->base)));

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_sdio_abort_async(const cyhal_sdio_t *obj)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    /* TODO: implement */
    return ret;
}


/* Callback array for SDHC SDIO interrupts */
static cyhal_sdio_irq_handler callbacks[CY_IP_MXSDHC_INSTANCES];
static void *callback_args[CY_IP_MXSDHC_INSTANCES];
static cyhal_sdio_t *cy_sdio_config_structs[CY_IP_MXSDHC_INSTANCES];


/*******************************************************************************
*       Dispatcher Interrupt Service Routine
*******************************************************************************/

static void isr_sdio_handler(uint8_t index, SDHC_Type *blockAddr)
{
    if (callbacks[index] != NULL)
    {
        /* Call registered callbacks here */
        (void)(callbacks[index])(callback_args[index], (cyhal_sdio_irq_event_t) cy_sdio_config_structs[index]->irq_cause);
    }

    Cy_SD_Host_ClearNormalInterruptStatus(blockAddr, cy_sdio_config_structs[index]->irq_cause);

    /* To clear SD Card interrupt need to disable SD Card interrupt Enable mask.
     * The SD Card interrupt is enabled in the bulk transfer function later on
     * the next data transfer.
     */
    if (0 != (Cy_SD_Host_GetNormalInterruptStatus(blockAddr) & CY_SD_HOST_CARD_INTERRUPT))
    {
        uint32_t interruptMask = Cy_SD_Host_GetNormalInterruptEnable(blockAddr);
        interruptMask &= (uint32_t) ~CY_SD_HOST_CARD_INTERRUPT;

        /* Disable SD Card interrupt */
        Cy_SD_Host_SetNormalInterruptEnable(blockAddr, interruptMask);
    }
}


/*******************************************************************************
*       (Internal) Interrupt Service Routines
*******************************************************************************/
static void isr_sdio_0_handler(void) __attribute__((unused));
static void isr_sdio_1_handler(void) __attribute__((unused));

typedef void (*interrupt_dispatcher)(void);

static const interrupt_dispatcher interrupts_dispatcher_table[CY_IP_MXSDHC_INSTANCES] =
{
#if (CY_IP_MXSDHC_INSTANCES > 0)
    isr_sdio_0_handler,
#endif
#if (CY_IP_MXSDHC_INSTANCES > 1)
    isr_sdio_1_handler,
#endif
};

static void isr_sdio_0_handler (void)
{
    isr_sdio_handler(0, SDHC0);
}
static void isr_sdio_1_handler (void)
{
    isr_sdio_handler(1, SDHC1);
}

cy_rslt_t cy_sdio_register_irq(cyhal_sdio_t *obj, cyhal_sdio_irq_handler handler, void *handler_arg)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    uint8_t index = obj->resource.block_num;

    IRQn_Type irqn = CY_SDHC_IRQ_N[index];
    cy_sdio_config_structs[index] = obj;
    callbacks[index] = handler;
    callback_args[index] = handler_arg;

    /* Only enable if it's not already enabled */
    //if ((NVIC_GetEnableIRQ(irqn) == 0) && (ret != CY_RSLT_SUCCESS))
    {
        cy_stc_sysint_t irqCfg = {irqn, SDIO_IRQ_PRIORITY};

        Cy_SysInt_Init(&irqCfg, interrupts_dispatcher_table[index]);
        NVIC_EnableIRQ(irqn);
    }
   // else
    {
       // NVIC_SetPriority(irqn, SDIO_IRQ_PRIORITY);
    }

    return ret;
}

cy_rslt_t cyhal_sdio_irq_enable(cyhal_sdio_t *obj, cyhal_sdio_irq_event_t event, bool enable)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    uint32_t interruptMask;
    uint32_t interruptEnable;

    interruptEnable = Cy_SD_Host_GetNormalInterruptEnable(obj->base);
    interruptMask = Cy_SD_Host_GetNormalInterruptMask(obj->base);

    if(enable)
    {
        interruptEnable |= event;
        interruptMask   |= event;

        obj->irq_cause |= event;
    }
    /* Disable interrupt */
    else
    {
        interruptEnable &= ~(event);
        interruptMask &= ~(event);

        obj->irq_cause &= ~event;
    }

    /* TODO: Need to double check is this flow is safe */
    //Cy_SD_Host_SetErrorInterruptEnable(obj->base, interruptEnable);
    //Cy_SD_Host_SetErrorInterruptMask(obj->base, interruptMask);

    Cy_SD_Host_SetNormalInterruptMask(obj->base, interruptMask);
    Cy_SD_Host_SetNormalInterruptEnable(obj->base, interruptEnable);

    return ret;
}

/* TODO: Remove these functions. Temporary added to make it workable */
cy_rslt_t sdio_bus_interrupt_enable(const cyhal_sdio_t *obj)
{
    uint32_t interrupt = Cy_SD_Host_GetNormalInterruptEnable(obj->base);

    interrupt |= CY_SD_HOST_CARD_INTERRUPT;

    Cy_SD_Host_SetNormalInterruptEnable(obj->base, interrupt);

    return CY_RSLT_SUCCESS;
}

cy_rslt_t sdio_bus_interrupt_disable(const cyhal_sdio_t *obj)
{
    uint32_t interrupt = Cy_SD_Host_GetNormalInterruptEnable(obj->base);

    interrupt &=~CY_SD_HOST_CARD_INTERRUPT;

    Cy_SD_Host_SetNormalInterruptEnable(obj->base, interrupt);

    return CY_RSLT_SUCCESS;
}

cy_rslt_t sdio_interrupt_status(const cyhal_sdio_t *obj, uint16_t* status)
{
    *status = (uint16_t)Cy_SD_Host_GetNormalInterruptStatus(obj->base);

    return CY_RSLT_SUCCESS;
}

cy_rslt_t sdio_clear_pending_isr(const cyhal_sdio_t *obj)
{
    NVIC_ClearPendingIRQ(sdhc_0_interrupt_general_IRQn);

    return CY_RSLT_SUCCESS;
}

#endif /* CY_IP_MXSDHC */
