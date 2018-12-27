/*******************************************************************************
* File Name: cy_hal_sdhc.c
*
* Description:
* Provides a high level interface for interacting with the Cypress SDHC. This 
* is a wrapper around the lower level PDL API.
* 
********************************************************************************
* Copyright (c) 2017-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/
#include "cy_hal_sdhc.h"

#ifdef CY_IP_MXSDHC

#define CY_HAL_SDHC_RETRY_TIME               (1000UL)  /* The number loops to make the timeout in msec. */
#define CY_HAL_SDHC_FUJE_TIMEOUT_MS          (1000U)   /* The Fuje timeout for one block. */
#define CY_HAL_SDHC_READ_WRITE_TIMEOUT_MS    (500U)    /* The Read/Write timeout for one block. */
#define CY_HAL_SDHC_CMD_TIMEOUT_MS           (5U)      /* The Command timeout. */
#define CY_HAL_SDHC_TRANSFER_TIMEOUT         (0xCUL)   /* The transfer timeout. */
#define CY_HAL_SDHC_EMMC_TRIM_TIMEOUT        (100U)    /* The EMMC TRIM timeout. */

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

    while ((CY_SD_HOST_ERROR_TIMEOUT == ret) && (retry-- > 0u))
    {
        /* Transfer complete */
        if (CY_SD_HOST_XFER_COMPLETE == (CY_SD_HOST_XFER_COMPLETE & Cy_SD_Host_GetNormalInterruptStatus(base)))
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

cy_rslt_t sdhc_init(sdhc_t *obj,
                    PinName cmd,
                    PinName clk,
                    PinName data0,
                    PinName data1,
                    PinName data2,
                    PinName data3,
                    PinName data4,
                    PinName data5,
                    PinName data6,
                    PinName data7,
                    PinName cardDetect,
                    PinName ioVoltSel,
                    PinName cardIfPwrEn,
                    PinName cardMechWriteProt,
                    PinName ledCtrl,
                    PinName cardEmmcReset)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet = CY_SD_HOST_SUCCESS;
    GPIO_PRT_Type *port;
    const uint32_t outputVal = 0UL;
    cy_stc_sd_host_init_config_t   hostConfig;

    /* Enable the SDHC block. */
    Cy_SD_Host_Enable(obj->base);

    /* Configure GPIO pins. */
    port = Cy_GPIO_PortToAddr(CY_PORT(cmd));
    Cy_GPIO_Pin_FastInit(port, CY_PIN(cmd), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);

    port = Cy_GPIO_PortToAddr(CY_PORT(clk));
    Cy_GPIO_Pin_FastInit(port, CY_PIN(clk), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);

    port = Cy_GPIO_PortToAddr(CY_PORT(data0));
    Cy_GPIO_Pin_FastInit(port, CY_PIN(data0), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);

    if (NC != data1)
    {
        port = Cy_GPIO_PortToAddr(CY_PORT(data1));
        Cy_GPIO_Pin_FastInit(port, CY_PIN(data1), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);
    }

    if (NC != data2)
    {
        port = Cy_GPIO_PortToAddr(CY_PORT(data2));
        Cy_GPIO_Pin_FastInit(port, CY_PIN(data2), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);
    }

    if (NC != data3)
    {
        port = Cy_GPIO_PortToAddr(CY_PORT(data3));
        Cy_GPIO_Pin_FastInit(port, CY_PIN(data3), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);
    }

    if (NC != data4)
    {
        port = Cy_GPIO_PortToAddr(CY_PORT(data4));
        Cy_GPIO_Pin_FastInit(port, CY_PIN(data4), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);
    }

    if (NC != data5)
    {
        port = Cy_GPIO_PortToAddr(CY_PORT(data5));
        Cy_GPIO_Pin_FastInit(port, CY_PIN(data5), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);
    }

    if (NC != data6)
    {
        port = Cy_GPIO_PortToAddr(CY_PORT(data6));
        Cy_GPIO_Pin_FastInit(port, CY_PIN(data6), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);
    }

    if (NC != data7)
    {
        port = Cy_GPIO_PortToAddr(CY_PORT(data7));
        Cy_GPIO_Pin_FastInit(port, CY_PIN(data7), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);
    }

    if (NC != cardDetect)
    {
        port = Cy_GPIO_PortToAddr(CY_PORT(cardDetect));
        Cy_GPIO_Pin_FastInit(port, CY_PIN(cardDetect), CY_GPIO_DM_HIGHZ, outputVal, HSIOM_SEL_ACT_14);
    }

    if (NC != ioVoltSel)
    {
        port = Cy_GPIO_PortToAddr(CY_PORT(ioVoltSel));
        Cy_GPIO_Pin_FastInit(port, CY_PIN(ioVoltSel), CY_GPIO_DM_STRONG_IN_OFF, outputVal, HSIOM_SEL_ACT_14);
    }

    if (NC != cardIfPwrEn)
    {
        port = Cy_GPIO_PortToAddr(CY_PORT(cardIfPwrEn));
        Cy_GPIO_Pin_FastInit(port, CY_PIN(cardIfPwrEn), CY_GPIO_DM_STRONG_IN_OFF, outputVal, HSIOM_SEL_ACT_14);
    }

    if (NC != cardMechWriteProt)
    {
        port = Cy_GPIO_PortToAddr(CY_PORT(cardMechWriteProt));
        Cy_GPIO_Pin_FastInit(port, CY_PIN(cardMechWriteProt), CY_GPIO_DM_HIGHZ, outputVal, HSIOM_SEL_ACT_14);
    }

    if (NC != ledCtrl)
    {
        port = Cy_GPIO_PortToAddr(CY_PORT(ledCtrl));
        Cy_GPIO_Pin_FastInit(port, CY_PIN(ledCtrl), CY_GPIO_DM_STRONG_IN_OFF, outputVal, HSIOM_SEL_ACT_14);
    }

    if (NC != cardEmmcReset)
    {
        port = Cy_GPIO_PortToAddr(CY_PORT(cardEmmcReset));
        Cy_GPIO_Pin_FastInit(port, CY_PIN(cardEmmcReset), CY_GPIO_DM_STRONG, outputVal, HSIOM_SEL_ACT_14);
    }

    /* Configure SD Host to operate */
    hostConfig.dmaType = obj->dmaType;
    hostConfig.enableLedControl = obj->enableLedControl;
    hostConfig.emmc = obj->emmc;
    obj->context.cardType = CY_SD_HOST_NOT_EMMC;

    driverRet = Cy_SD_Host_Init(obj->base, &hostConfig, &obj->context);

    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    return ret;
}

cy_rslt_t sdhc_set_sd_clk_frequency(sdhc_t *obj, uint32_t frequency)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet;
    uint32_t               clkDiv;
    uint32_t               clockInput = 100000000UL;

    clkDiv = (clockInput / frequency) >> 1UL;
    Cy_SD_Host_DisableSdClk(obj->base);
    driverRet = Cy_SD_Host_SetSdClkDiv(obj->base, (uint16_t)clkDiv);
    Cy_SD_Host_EnableSdClk(obj->base);

    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    return ret;
}

cy_rslt_t sdhc_send_command(sdhc_t *obj, const cmdConfig_t *config)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet;

    driverRet = Cy_SD_Host_SendCommand(obj->base, config);

    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    return ret;
}

cy_rslt_t sdhc_get_response(sdhc_t *obj, uint32_t *response)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet;

    driverRet = Cy_SD_Host_GetResponse(obj->base, (uint32_t *)response, true);

    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    return ret;
}

cy_rslt_t sdhc_free(sdhc_t *obj)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    
    Cy_SD_Host_DeInit(obj->base);

    return ret;
}

cy_rslt_t sdhc_configure(sdhc_t *obj, cardConfig_t *config)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet = CY_SD_HOST_SUCCESS;
    cy_stc_sd_host_sd_card_config_t stcSdcardCfg;
    cy_en_sd_host_card_capacity_t cardCapacity;
    uint32_t rca = 0UL;
    cy_en_sd_host_card_type_t cardType = CY_SD_HOST_UNUSABLE;
    
    /** Initialize the SDIO/SD/eMMC card configuration structure */
    stcSdcardCfg.lowVoltageSignaling = config->lowVoltageSignaling;
    stcSdcardCfg.busWidth = config->busWidth;
    stcSdcardCfg.cardType = &cardType;
    stcSdcardCfg.rca = &rca;
    stcSdcardCfg.cardCapacity = &cardCapacity;
    
    /** Initialize the card */
    driverRet = Cy_SD_Host_InitCard(obj->base, &stcSdcardCfg, &obj->context);
    
    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    return ret;
}

cy_rslt_t sdhc_read(const sdhc_t *obj, uint32_t address, uint8_t *data, size_t length)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet;
    cy_stc_sd_host_write_read_config_t dataConfig;

    dataConfig.data = (uint32_t*)data;   /* The pointer to data. */
    dataConfig.address = address;        /* The address to write/read data on the card or eMMC. */
    dataConfig.numberOfBlocks = length;  /* The number of blocks to write/read. */
    if (1UL == length)
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

cy_rslt_t sdhc_write(const sdhc_t *obj, uint32_t address, const uint8_t *data, size_t length)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet;
    cy_stc_sd_host_write_read_config_t dataConfig;

    dataConfig.data = (uint32_t*)data;   /* The pointer to data. */
    dataConfig.address = address;        /* The address to write/read data on the card or eMMC. */
    dataConfig.numberOfBlocks = length;  /* The number of blocks to write/read. */
    if (1UL == length)
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

cy_rslt_t sdhc_erase(const sdhc_t *obj, uint32_t startAddr, size_t length)
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

cy_rslt_t sdhc_read_asynch(const sdhc_t *obj, uint32_t address, uint8_t *data, size_t length, uint32_t handler, uint32_t event)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    
    return ret;
}

cy_rslt_t sdhc_write_asynch(const sdhc_t *obj, uint32_t address, const uint8_t *data, size_t length, uint32_t handler, uint32_t event)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    
    return ret;
}

cy_rslt_t sdhc_is_busy(const sdhc_t *obj, bool *busy)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;

    if (CY_SD_HOST_XFER_COMPLETE == (CY_SD_HOST_XFER_COMPLETE & Cy_SD_Host_GetNormalInterruptStatus(obj->base)))
    {
        *busy = false;
    }
    else
    {
        *busy = true;
    }

    return ret;
}

cy_rslt_t sdhc_abort_asynch(const sdhc_t *obj)
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

#endif /* CY_IP_MXSDHC */
