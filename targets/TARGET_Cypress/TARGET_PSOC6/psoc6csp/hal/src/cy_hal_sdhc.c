/*******************************************************************************
* File Name: cy_hal_sdhc.c
*
* Description:
* Provides a high level interface for interacting with the Cypress SDHC. This 
* is a wrapper around the lower level PDL API.
* 
********************************************************************************
* Copyright (c) 2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include <string.h> /* For memcpy */
#include "cy_hal_sdhc.h"
#include "cy_hal_sdio.h"
#include "cy_sysint.h"
#include "cy_sd_host.h"
#include "cy_gpio.h"
#include "cy_hal_pin_package.h"


#ifdef CY_IP_MXSDHC

#define CY_HAL_SDHC_RETRY_TIME               (1000UL)  /* The number loops to make the timeout in msec. */
#define CY_HAL_SDHC_FUJE_TIMEOUT_MS          (1000U)   /* The Fuje timeout for one block. */
#define CY_HAL_SDHC_READ_WRITE_TIMEOUT_MS    (500U)    /* The Read/Write timeout for one block. */
#define CY_HAL_SDHC_CMD_TIMEOUT_MS           (5U)      /* The Command timeout. */
#define CY_HAL_SDHC_TRANSFER_TIMEOUT         (0xCUL)   /* The transfer timeout. */
#define CY_HAL_SDHC_EMMC_TRIM_TIMEOUT        (100U)    /* The EMMC TRIM timeout. */

#define CY_SD_HOST_CLK_400K                 (400UL * 1000UL)    /* 400 kHz. */
#define SDIO_ENUMERATION_TIMEOUT_MS         (500U)
#define SDHC_SDIO_64B_BLOCK                 (64U)

#define SDHC_IRQ_PRIORITY                   (3UL)
#define SDIO_IRQ_PRIORITY                   (3UL)

#define ENABLE_ALL_INTERRUPT_MASK            (0x0000FFFFUL)
#define ENABLE_ALL_INTERRUPT_ENABLE          (0x0000FFFFUL)
#define DISABLE_ALL_INTERRUPT_MASK           (0x0UL)
#define DISABLE_ALL_INTERRUPT_ENABLE         (0x0UL)

#define BIT_MASK( x )                       (( 1 << x ) - 1 )


static inline unsigned CY_PIN(cy_gpio_t pin);
static inline unsigned CY_PIN(cy_gpio_t pin)
{
    return pin & 0x07;
}

static inline unsigned CY_PORT(cy_gpio_t pin);
static inline unsigned CY_PORT(cy_gpio_t pin)
{
    return (pin >> 8) & 0xFF;
}

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


cy_rslt_t cy_sdhc_init(cy_sdhc_t *obj,
                    cy_gpio_t cmd,
                    cy_gpio_t clk,
                    cy_gpio_t data0,
                    cy_gpio_t data1,
                    cy_gpio_t data2,
                    cy_gpio_t data3,
                    cy_gpio_t data4,
                    cy_gpio_t data5,
                    cy_gpio_t data6,
                    cy_gpio_t data7,
                    cy_gpio_t cardDetect,
                    cy_gpio_t ioVoltSel,
                    cy_gpio_t cardIfPwrEn,
                    cy_gpio_t cardMechWriteProt,
                    cy_gpio_t ledCtrl,
                    cy_gpio_t cardEmmcReset)
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


cy_rslt_t cy_sdhc_free(cy_sdhc_t *obj)
{
    Cy_SD_Host_DeInit(obj->base);

    return CY_RSLT_SUCCESS;
}


cy_rslt_t cy_sdhc_configure(cy_sdhc_t *obj, const cy_sdhc_config_t *config)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet = CY_SD_HOST_SUCCESS;
    cy_stc_sd_host_sd_card_config_t stcSdcardCfg;
    cy_en_sd_host_card_capacity_t cardCapacity;
    uint32_t rca = 0UL;
    cy_en_sd_host_card_type_t cardType = CY_SD_HOST_UNUSABLE;
    
    /** Initialize the SDIO/SD/eMMC card configuration structure */
    stcSdcardCfg.lowVoltageSignaling = config->lowVoltageSignaling;
    stcSdcardCfg.busWidth = convert_buswidth(config->busWidth);
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


cy_rslt_t cy_sdhc_read(const cy_sdhc_t *obj, uint32_t address, uint8_t *data, size_t *length)
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


cy_rslt_t cy_sdhc_write(const cy_sdhc_t *obj, uint32_t address, const uint8_t *data, size_t *length)
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


cy_rslt_t cy_sdhc_erase(const cy_sdhc_t *obj, uint32_t startAddr, size_t length)
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


cy_rslt_t cy_sdhc_read_async(const cy_sdhc_t *obj, uint32_t address, uint8_t *data, size_t *length)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    //TODO: implement
    return ret;
}


cy_rslt_t cy_sdhc_write_async(const cy_sdhc_t *obj, uint32_t address, const uint8_t *data, size_t *length)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    //TODO: implement
    return ret;
}


cy_rslt_t cy_sdhc_is_busy(const cy_sdhc_t *obj, bool *busy)
{
    *busy = (CY_SD_HOST_XFER_COMPLETE != (CY_SD_HOST_XFER_COMPLETE & Cy_SD_Host_GetNormalInterruptStatus(obj->base)));

    return CY_RSLT_SUCCESS;
}


cy_rslt_t cy_sdhc_abort_async(const cy_sdhc_t *obj)
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


cy_rslt_t cy_sdhc_register_irq(cy_sdhc_t *obj, cy_sdhc_irq_handler handler, void *handler_arg)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    IRQn_Type irqn = sdhc_0_interrupt_general_IRQn;

    if (obj->base == SDHC1)
    {
        irqn = sdhc_1_interrupt_general_IRQn;
    }
    else if (obj->base != SDHC0)
    {
        ret = CY_SYSINT_BAD_PARAM;
    }

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


cy_rslt_t cy_sdhc_irq_enable(cy_sdhc_t *obj, cy_sdhc_irq_event_t event, bool enable)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    uint32_t interruptMask;
    uint32_t interruptEnable;

    interruptEnable = Cy_SD_Host_GetNormalInterruptEnable(obj->base);
    interruptMask = Cy_SD_Host_GetNormalInterruptMask(obj->base);

    if(enable == true) 
    {
        /* Enable specific interrupt */
        if(event < CY_SDHC_ERR_INTERRUPT)
        {
            interruptEnable |= (1 << event);
            interruptMask |= (1 << event);
        }
        /* Enable error interrupts */
        else if (event == CY_SDHC_ERR_INTERRUPT)
        {
            Cy_SD_Host_SetErrorInterruptEnable(obj->base, ENABLE_ALL_INTERRUPT_ENABLE);
            Cy_SD_Host_SetErrorInterruptMask(obj->base, ENABLE_ALL_INTERRUPT_MASK);
        }
        /* Enable all interrupts */
        else if (event == CY_SDHC_ALL_INTERRUPTS)
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
        if(event < CY_SDHC_ERR_INTERRUPT)
        {
            interruptEnable &= ~(1 << event);
            interruptMask &= ~(1 << event);
        }
        /* Disable error interrupts */
        else if (event == CY_SDHC_ERR_INTERRUPT)
        {
            Cy_SD_Host_SetErrorInterruptEnable(obj->base, DISABLE_ALL_INTERRUPT_ENABLE);
            Cy_SD_Host_SetErrorInterruptMask(obj->base, DISABLE_ALL_INTERRUPT_MASK);
        }
        /* Disable all interrupts */
        else if (event == CY_SDHC_ALL_INTERRUPTS)
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




cy_rslt_t cy_sdio_init(cy_sdio_t *obj, cy_gpio_t cmd, cy_gpio_t clk, cy_gpio_t data0, cy_gpio_t data1, cy_gpio_t data2, cy_gpio_t data3)
{
    cy_stc_sd_host_init_config_t    hostConfig;
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    GPIO_PRT_Type *port;
    const uint32_t outputVal = 0UL;
    cy_stc_sd_host_context_t context;

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

    /*Enable SDHC interrupt*/
    if (SDHC0 == obj->base)
    {
        NVIC_EnableIRQ(sdhc_0_interrupt_general_IRQn);
    }
    else if (SDHC1 == obj->base)
    {
        NVIC_EnableIRQ(sdhc_1_interrupt_general_IRQn);
    }
    else
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    (void)Cy_SD_Host_SetHostBusWidth(SDHC0, CY_SD_HOST_BUS_WIDTH_4_BIT);

    /* Change the host SD clock to 400 kHz */
    (void)Cy_SD_Host_SdCardChangeClock(SDHC0, CY_SD_HOST_CLK_400K);

    /* Wait for WiFi chip to be ready */
    Cy_SysLib_Delay(1000);

    return ret;
}


cy_rslt_t cy_sdio_free(cy_sdio_t *obj)
{
    Cy_SD_Host_DeInit(obj->base);

    return CY_RSLT_SUCCESS;
}


cy_rslt_t cy_sdio_configure(cy_sdio_t *obj, const cy_sdio_cfg_t *config)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t result;
    uint32_t               loop_count = 0UL;
    uint32_t               rca = 0UL;
    cy_stc_sd_host_cmd_config_t cmd;

    do
    {
        /* Send CMD0 to set it to idle state */
        cmd.commandIndex                    = (uint32_t)SDIO_CMD_GO_IDLE_STATE;
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
        cmd.commandIndex                    = (uint32_t)SDIO_CMD_IO_SEND_OP_COND;
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
        cmd.commandIndex                    = (uint32_t)SDIO_CMD_SEND_RELATIVE_ADDR;
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
        cmd.commandIndex                    = (uint32_t)SDIO_CMD_SELECT_CARD;
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


cy_rslt_t cy_sdio_send_cmd(const cy_sdio_t *obj, cy_transfer_t direction, cy_sdio_command_t command, uint32_t argument, uint32_t* response)
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
            Cy_SD_Host_SoftwareReset(obj->base,    CY_SD_HOST_RESET_CMD_LINE);
            Cy_SD_Host_SoftwareReset(obj->base,    CY_SD_HOST_RESET_DATALINE);
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

    return ret;
}

#ifndef WICED_PLATFORM_DOESNT_USE_TEMP_DMA_BUFFER
    static uint8_t temp_dma_buffer[2048];
#endif


cy_rslt_t cy_sdio_bulk_transfer(const cy_sdio_t *obj, cy_transfer_t direction, uint32_t argument, const uint32_t* data, uint16_t length, uint32_t* response)
{
    cy_rslt_t                    ret = CY_RSLT_SUCCESS;
    uint32_t                     retry = 50UL;
    const uint32_t               block_size = SDHC_SDIO_64B_BLOCK;
    cy_stc_sd_host_cmd_config_t  cmd;
    cy_stc_sd_host_data_config_t dat;
    cy_en_sd_host_status_t       result = CY_SD_HOST_ERROR_TIMEOUT;
    cy_sdio_transfer_mode_t      mode = (64U <= length) ? CY_SDIO_BLOCK_MODE : CY_SDIO_BYTE_MODE;


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

        if( mode == CY_SDIO_BLOCK_MODE )
        {
              dat.blockSize         = block_size;
              dat.numberOfBlock     = ( length + block_size - 1 ) / block_size;
        }
        else
        {
              dat.blockSize       = length;
              dat.numberOfBlock   = 1UL;
        }

        dat.read = ( direction == CY_WRITE ) ? false : true;

#ifndef WICED_PLATFORM_DOESNT_USE_TEMP_DMA_BUFFER
        if (( direction == CY_READ ) && ( mode == CY_SDIO_BLOCK_MODE ))
        {
            /* In this mode, we may be reading more than the requested size to round
             * up to the nearest multiple of block size. So, providing temp buffer
             * instead of the original buffer to avoid memory corruption
             */
            dat.data       = (uint32_t*)temp_dma_buffer;
        }
#endif
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

#ifndef WICED_PLATFORM_DOESNT_USE_TEMP_DMA_BUFFER
        if (( direction == CY_READ ) && ( mode == CY_SDIO_BLOCK_MODE ))
        {
            memcpy((uint32_t*)data, (const uint32_t*)temp_dma_buffer, (size_t) length );
        }
#endif

    if (response != NULL )
    {
        (void)Cy_SD_Host_GetResponse(obj->base, response, false);
    }

    if (CY_SD_HOST_SUCCESS != result)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    return ret;
}


cy_rslt_t cy_sdio_transfer_async(const cy_sdio_t *obj, cy_transfer_t direction, uint32_t argument, const uint32_t* data, uint16_t length)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    /* TODO: implement */
    return ret;
}


cy_rslt_t cy_sdio_is_busy(const cy_sdio_t *obj, bool *busy)
{
    *busy = (CY_SD_HOST_XFER_COMPLETE != (CY_SD_HOST_XFER_COMPLETE & Cy_SD_Host_GetNormalInterruptStatus(obj->base)));

    return CY_RSLT_SUCCESS;
}


cy_rslt_t cy_sdio_abort_async(const cy_sdio_t *obj)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    /* TODO: implement */
    return ret;
}


cy_rslt_t cy_sdio_register_irq(cy_sdio_t *obj, cy_sdio_irq_handler handler, void *handler_arg)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    IRQn_Type irqn = sdhc_0_interrupt_general_IRQn;
    
    if (obj->base == SDHC1)
    {
        irqn = sdhc_1_interrupt_general_IRQn;
    }
    else if (obj->base != SDHC0)
    {
        ret = CY_SYSINT_BAD_PARAM;
    }

    /* Only enable if it's not already enabled */
    if (NVIC_GetEnableIRQ(irqn) == 0)
    {
        cy_stc_sysint_t irqCfg = {irqn, SDIO_IRQ_PRIORITY};

        Cy_SysInt_Init(&irqCfg, (cy_israddress)handler);
        NVIC_EnableIRQ(irqn);
    }
    else
    {
        NVIC_SetPriority(irqn, SDIO_IRQ_PRIORITY);
    }   

    return ret;
}


cy_rslt_t cy_sdio_irq_enable(cy_sdio_t *obj, cy_sdio_irq_event_t event, bool enable)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    uint32_t interruptMask;
    uint32_t interruptEnable;

    interruptEnable = Cy_SD_Host_GetNormalInterruptEnable(obj->base);
    interruptMask = Cy_SD_Host_GetNormalInterruptMask(obj->base);

    if(enable == true) 
    {
        /* Enable specific interrupt */
        if(event < CY_SDIO_ERR_INTERRUPT)
        {
            interruptEnable |= (1 << event);
            interruptMask |= (1 << event);
        }
        /* Enable error interrupts */
        else if (event == CY_SDIO_ERR_INTERRUPT)
        {
            Cy_SD_Host_SetErrorInterruptEnable(obj->base, ENABLE_ALL_INTERRUPT_ENABLE);
            Cy_SD_Host_SetErrorInterruptMask(obj->base, ENABLE_ALL_INTERRUPT_MASK);
        }
        /* Enable all interrupts */
        else if (event == CY_SDIO_ALL_INTERRUPTS)
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
        if(event < CY_SDIO_ERR_INTERRUPT)
        {
            interruptEnable &= ~(1 << event);
            interruptMask &= ~(1 << event);
        }
        /* Disable error interrupts */
        else if (event == CY_SDIO_ERR_INTERRUPT)
        {
            Cy_SD_Host_SetErrorInterruptEnable(obj->base, DISABLE_ALL_INTERRUPT_ENABLE);
            Cy_SD_Host_SetErrorInterruptMask(obj->base, DISABLE_ALL_INTERRUPT_MASK);
        }
        /* Disable all interrupts */
        else if (event == CY_SDIO_ALL_INTERRUPTS)
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

#endif /* CY_IP_MXSDHC */
