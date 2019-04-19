/*******************************************************************************
* File Name: cy_hal_udb_sdio.c
*
* Description:
* Provides a high level interface for interacting with the Cypress UDB SDIO.
* This is a wrapper around the lower level UDB SDIO API.
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

#if defined(CY_IP_MXUDB)

#include <stdlib.h>
#include "cy_hal_sdio.h"
#include "cy_hal_gpio.h"
#include "cy_hal_interconnect.h"
#include "SDIO_HOST_cfg.h"

/* Not connected pin define */
#define SDIO_PINS_NC    ((cy_gpio_t) CY_NC_PIN_VALUE)

/* The 64b block transition mode define */
#define CY_HAL_SDIO_64B      (64u)

/* The 1 byte transition mode define */
#define CY_HAL_SDIO_1B        (1u)

/*******************************************************************************
*       Configuration structures for SDIO pins
*******************************************************************************/
const cy_stc_gpio_pin_config_t pin_cmd_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = P2_4_DSI_DSI, /* DSI controls 'out' and 'output enable' */
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
};

const cy_stc_gpio_pin_config_t pin_data_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = P2_0_DSI_DSI, /* DSI controls 'out' and 'output enable' */
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
};

const cy_stc_gpio_pin_config_t pin_clk_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = P2_5_DSI_GPIO, /* DSI controls 'out', GPIO controls 'output enable' */
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
};

/* Callback pointers */
static cy_sdio_irq_handler cy_hal_sdio_callback = NULL;
static cy_sdio_t *cy_hal_sdio_config_struct = NULL;
static void *cy_hal_sdio_callback_args = NULL;


/*******************************************************************************
*       Dispatcher Interrupt Callbacks Service Routine
*******************************************************************************/
static void cy_sdio_interrupts_dispatcher_IRQHandler(void)
{
    if (cy_hal_sdio_callback != NULL)
    {
        (void)(cy_hal_sdio_callback)(cy_hal_sdio_callback_args, (cy_sdio_irq_event_t) cy_hal_sdio_config_struct->irqCause);
    }
}


/*******************************************************************************
*       Internal functions
*******************************************************************************/
static cy_rslt_t cy_free_pins(cy_sdio_t *obj);

static cy_rslt_t cy_free_pins(cy_sdio_t *obj)
{
    cy_rslt_t retVal = CY_RSC_INVALID;

    retVal = cy_gpio_free(obj->pin_clk);
    
    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = cy_gpio_free(obj->pin_cmd);
    }

    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = cy_gpio_free(obj->pin_data0);
    }
    
    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = cy_gpio_free(obj->pin_data1);
    }
    
    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = cy_gpio_free(obj->pin_data2);
    }
    
    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = cy_gpio_free(obj->pin_data3);
    }

    return retVal;
}

static cy_rslt_t cy_free_clocks(cy_sdio_t *obj)
{
    cy_rslt_t retVal;

    /* Free clock divider resource*/
    cy_resource_inst_t udbClkRsc;
    
    udbClkRsc.type = CY_RSC_CLOCK;
    udbClkRsc.block_num = obj->clock.div_num;
    udbClkRsc.channel_num = 0;

    retVal = cy_hwmgr_set_unconfigured(udbClkRsc.type, udbClkRsc.block_num, udbClkRsc.channel_num);
    
    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = cy_hwmgr_free(&udbClkRsc);
    }

    return retVal;
}


static cy_rslt_t cy_free_dmas(cy_sdio_t *obj)
{
    cy_rslt_t retVal;

    /* Free clock divider resource*/
    cy_resource_inst_t dmaRsc;
    
    dmaRsc.type = CY_RSC_DMA;
    
    /* Release DMA0 CH0 */
    dmaRsc.block_num = 0;
    dmaRsc.channel_num = 0;

    retVal = cy_hwmgr_set_unconfigured(dmaRsc.type, dmaRsc.block_num, dmaRsc.channel_num);
    
    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = cy_hwmgr_free(&dmaRsc);
    }
    
    /* Release DMA0 CH1 */
    dmaRsc.block_num = 0;
    dmaRsc.channel_num = 1;

    retVal = cy_hwmgr_set_unconfigured(dmaRsc.type, dmaRsc.block_num, dmaRsc.channel_num);
    
    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = cy_hwmgr_free(&dmaRsc);
    }
    
    /* Release DMA1 CH1 */
    dmaRsc.block_num = 1;
    dmaRsc.channel_num = 1;

    retVal = cy_hwmgr_set_unconfigured(dmaRsc.type, dmaRsc.block_num, dmaRsc.channel_num);
    
    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = cy_hwmgr_free(&dmaRsc);
    }

    /* Release DMA1 CH1 */
    dmaRsc.block_num = 1;
    dmaRsc.channel_num = 3;

    retVal = cy_hwmgr_set_unconfigured(dmaRsc.type, dmaRsc.block_num, dmaRsc.channel_num);
    
    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = cy_hwmgr_free(&dmaRsc);
    }
    
    return retVal;
}


/** Initialize the SDIO peripheral
 *
 * @param[out] obj               The SDIO object
 * @param[out] clk               The pin connected to the clk signal
 * @param[in]  cmd               The pin connected to the command signal
 * @param[in]  data0             The pin connected to the data0 signal
 * @param[in]  data1             The pin connected to the data1 signal
 * @param[in]  data2             The pin connected to the data2 signal
 * @param[in]  data3             The pin connected to the data3 signal
 * @return The status of the init request
 */
cy_rslt_t cy_sdio_init(cy_sdio_t *obj, cy_gpio_t cmd, cy_gpio_t clk, cy_gpio_t data0, cy_gpio_t data1, cy_gpio_t data2, cy_gpio_t data3)
{
    bool isConfigured = false;
    cy_rslt_t retVal = CY_RSLT_SUCCESS;

    if(NULL == obj)
    {
        return CY_RSLT_SDIO_BAD_PARAM_ARGUMENT;
    }
    
    memset(obj, 0, sizeof(cy_sdio_t));
    
    /* If something go wrong, any resource not marked as invalid will be freed.
    *  Explicitly marked not allocated resources as invalid to prevent freeing 
    *  them.
    * 
    *  Before we reserve UDB at all we need try to reserve clock divider, then 
    *  DMAs and pins. If all these resources are reserved only then reserve UDB 
    *  SDIO.
    */
    obj->resource.type = CY_RSC_INVALID;
    obj->pin_cmd = SDIO_PINS_NC;
    obj->pin_clk = SDIO_PINS_NC;
    obj->pin_data0 = SDIO_PINS_NC;
    obj->pin_data1 = SDIO_PINS_NC;
    obj->pin_data2 = SDIO_PINS_NC;
    obj->pin_data3 = SDIO_PINS_NC;

    /* Reserve clock */
    cy_resource_inst_t udbClkRsc = { CY_RSC_CLOCK, 0, 0 };
    retVal = cy_hwmgr_reserve(&udbClkRsc);
    if (retVal == CY_RSLT_SUCCESS)
    {
        /* Check is the clock divider 0 configured */
        retVal = 
        cy_hwmgr_is_configured(udbClkRsc.type, udbClkRsc.block_num, udbClkRsc.channel_num, &isConfigured);

        /* UDB SDIO uses hard-coded clock divider 0 */
        if (retVal == CY_RSLT_SUCCESS && !isConfigured)
        {
            /* Assign clock divider */
            cy_en_sysclk_status_t clockAssignResult;

            obj->clock.div_type = CY_SYSCLK_DIV_8_BIT;
            obj->clock.div_num = 0;
           
            clockAssignResult = Cy_SysClk_PeriphSetDivider(obj->clock.div_type, obj->clock.div_num, 0U);
            
            if (CY_SYSCLK_SUCCESS == clockAssignResult)
            {
                clockAssignResult = Cy_SysClk_PeriphEnableDivider(obj->clock.div_type, obj->clock.div_num); 
            }
            
            if (CY_SYSCLK_SUCCESS == clockAssignResult)
            {
                clockAssignResult = Cy_SysClk_PeriphAssignDivider(PCLK_UDB_CLOCKS0, obj->clock.div_type, obj->clock.div_num);
            }

            retVal = (clockAssignResult == CY_SYSCLK_SUCCESS) 
                      ? CY_RSLT_SUCCESS
                      : CY_RSLT_SDIO_CLOCK_ERROR;

            /* Update object with reserved clock info */

            cy_hwmgr_set_configured(udbClkRsc.type, udbClkRsc.block_num, udbClkRsc.channel_num);
        }
    }

    /* The DMAs are initialized in SDIO_Init() function, so reserve DMA only */
    if (retVal == CY_RSLT_SUCCESS)
    {
        /* Reserve DMA0 CH0 */
        cy_resource_inst_t dmaRsc = { CY_RSC_DMA, 0, 0 };
        retVal = cy_hwmgr_reserve(&dmaRsc);

        if (retVal == CY_RSLT_SUCCESS)
        {
            retVal = cy_hwmgr_is_configured(dmaRsc.type, dmaRsc.block_num, dmaRsc.channel_num, &isConfigured);

            if (retVal == CY_RSLT_SUCCESS && !isConfigured)
            {
                //cy_dma_t udbDma0Ch0 = {TODO fill when is ready};

                /* Update obj info related to DMA */
                //obj->dma0Ch0.param1 = udbDma0Ch0.param1;
                //obj->dma0ChX.paramX = udbDma0Ch0.paramX;
                cy_hwmgr_set_configured(dmaRsc.type, dmaRsc.block_num, dmaRsc.channel_num);
            }
        }
    }

    if (retVal == CY_RSLT_SUCCESS)
    {
        /* Reserve DMA0 CH1 */
        cy_resource_inst_t dmaRsc = { CY_RSC_DMA, 0, 1 };

        retVal = cy_hwmgr_reserve(&dmaRsc);
        if (retVal == CY_RSLT_SUCCESS)
        {
            retVal = cy_hwmgr_is_configured(dmaRsc.type, dmaRsc.block_num, dmaRsc.channel_num, &isConfigured);

            if (retVal == CY_RSLT_SUCCESS && !isConfigured)
            {
                //cy_dma_t udbDma0Ch1 = {TODO fill when is ready};

                /* Update obj info related to DMA */
                //obj->dma0Ch1.param1 = udbDma0Ch1.param1;
                //obj->dma0ChX.paramX = udbDma0Ch1paramX;
                cy_hwmgr_set_configured(dmaRsc.type, dmaRsc.block_num, dmaRsc.channel_num);
            }
        }
    }
    
    if (retVal == CY_RSLT_SUCCESS)
    {
        /* Reserve DMA1 CH1 */
        cy_resource_inst_t dmaRsc = { CY_RSC_DMA, 1, 1 };
        retVal = cy_hwmgr_reserve(&dmaRsc);

        if (retVal == CY_RSLT_SUCCESS)
        {
            retVal = cy_hwmgr_is_configured(dmaRsc.type, dmaRsc.block_num, dmaRsc.channel_num, &isConfigured);

            if (retVal == CY_RSLT_SUCCESS && !isConfigured)
            {
                //cy_dma_t udbDma1Ch1 = {TODO fill when is ready};

                /* Update obj info related to DMA */
                //obj->dma0Ch1.param1 = udbDma1Ch1.param1;
                //obj->dma0ChX.paramX = udbDma1Ch1paramX;
                cy_hwmgr_set_configured(dmaRsc.type, dmaRsc.block_num, dmaRsc.channel_num);
            }
        }
    }
    
    if (retVal == CY_RSLT_SUCCESS)
    {
        /* Reserve DMA1 CH3 */
        cy_resource_inst_t dmaRsc = { CY_RSC_DMA, 1, 3 };
        retVal = cy_hwmgr_reserve(&dmaRsc);

        if (retVal == CY_RSLT_SUCCESS)
        {
            retVal = cy_hwmgr_is_configured(dmaRsc.type, dmaRsc.block_num, dmaRsc.channel_num, &isConfigured);
            
            if (retVal == CY_RSLT_SUCCESS && !isConfigured)
            {
                cy_hwmgr_set_configured(dmaRsc.type, dmaRsc.block_num, dmaRsc.channel_num);
            }
        }
    }

    /* Reserve the clk pin */
    if (retVal == CY_RSLT_SUCCESS)
    {
        cy_resource_inst_t pinRsc = cy_utils_get_gpio_resource(clk);
        retVal = cy_hwmgr_reserve(&pinRsc);
        if (retVal == CY_RSLT_SUCCESS)
        {
            retVal = cy_hwmgr_is_configured(pinRsc.type, pinRsc.block_num, pinRsc.channel_num, &isConfigured);
            if (retVal == CY_RSLT_SUCCESS && !isConfigured)
            {
                Cy_GPIO_Pin_Init(CY_GET_PORTADDR(clk), CY_GET_PIN(clk), &pin_clk_config);

                obj->pin_clk = clk;

                cy_hwmgr_set_configured(pinRsc.type, pinRsc.block_num, pinRsc.channel_num);
            }
        }
    }

    /* Reserve the cmd pin */
    if (retVal == CY_RSLT_SUCCESS)
    {
        cy_resource_inst_t pinRsc = cy_utils_get_gpio_resource(cmd);
        retVal = cy_hwmgr_reserve(&pinRsc);
        if (retVal == CY_RSLT_SUCCESS)
        {
            retVal = cy_hwmgr_is_configured(pinRsc.type, pinRsc.block_num, pinRsc.channel_num, &isConfigured);
            if (retVal == CY_RSLT_SUCCESS && !isConfigured)
            {
                Cy_GPIO_Pin_Init(CY_GET_PORTADDR(cmd), CY_GET_PIN(cmd), &pin_cmd_config);

                obj->pin_cmd = cmd;

                cy_hwmgr_set_configured(pinRsc.type, pinRsc.block_num, pinRsc.channel_num);
            }
        }
    }

    /* Reserve the data0 pin */
    if (retVal == CY_RSLT_SUCCESS)
    {
        cy_resource_inst_t pinRsc = cy_utils_get_gpio_resource(data0);
        retVal = cy_hwmgr_reserve(&pinRsc);
        if (retVal == CY_RSLT_SUCCESS)
        {
            retVal = cy_hwmgr_is_configured(pinRsc.type, pinRsc.block_num, pinRsc.channel_num, &isConfigured);
            if (retVal == CY_RSLT_SUCCESS && !isConfigured)
            {
                Cy_GPIO_Pin_Init(CY_GET_PORTADDR(data0), CY_GET_PIN(data0), &pin_data_config);

                obj->pin_data0 = data0;

                cy_hwmgr_set_configured(pinRsc.type, pinRsc.block_num, pinRsc.channel_num);
            }
        }
    }
    
    /* Reserve the data1 pin */
    if (retVal == CY_RSLT_SUCCESS)
    {
        cy_resource_inst_t pinRsc = cy_utils_get_gpio_resource(data1);
        retVal = cy_hwmgr_reserve(&pinRsc);
        if (retVal == CY_RSLT_SUCCESS)
        {
            retVal = cy_hwmgr_is_configured(pinRsc.type, pinRsc.block_num, pinRsc.channel_num, &isConfigured);
            if (retVal == CY_RSLT_SUCCESS && !isConfigured)
            {
                Cy_GPIO_Pin_Init(CY_GET_PORTADDR(data1), CY_GET_PIN(data1), &pin_data_config);

                obj->pin_data1 = data1;
                cy_hwmgr_set_configured(pinRsc.type, pinRsc.block_num, pinRsc.channel_num);
            }
        }
    }
    
    /* Reserve the data2 pin */
    if (retVal == CY_RSLT_SUCCESS)
    {
        cy_resource_inst_t pinRsc = cy_utils_get_gpio_resource(data2);
        retVal = cy_hwmgr_reserve(&pinRsc);
        if (retVal == CY_RSLT_SUCCESS)
        {
            retVal = cy_hwmgr_is_configured(pinRsc.type, pinRsc.block_num, pinRsc.channel_num, &isConfigured);
            if (retVal == CY_RSLT_SUCCESS && !isConfigured)
            {
                Cy_GPIO_Pin_Init(CY_GET_PORTADDR(data2), CY_GET_PIN(data2), &pin_data_config);

                obj->pin_data2 = data2;
                cy_hwmgr_set_configured(pinRsc.type, pinRsc.block_num, pinRsc.channel_num);
            }
        }
    }
    
    /* Reserve the data3 pin */
    if (retVal == CY_RSLT_SUCCESS)
    {
        cy_resource_inst_t pinRsc = cy_utils_get_gpio_resource(data3);
        retVal = cy_hwmgr_reserve(&pinRsc);
        if (retVal == CY_RSLT_SUCCESS)
        {
            retVal = cy_hwmgr_is_configured(pinRsc.type, pinRsc.block_num, pinRsc.channel_num, &isConfigured);
            if (retVal == CY_RSLT_SUCCESS && !isConfigured)
            {
                Cy_GPIO_Pin_Init(CY_GET_PORTADDR(data3), CY_GET_PIN(data3), &pin_data_config);

                obj->pin_data3 = data3;
                cy_hwmgr_set_configured(pinRsc.type, pinRsc.block_num, pinRsc.channel_num);
            }
        }
    }
    
    /* Reserve UDB */
    if (retVal == CY_RSLT_SUCCESS)
    {
        /* UDB SDIO resource */
        cy_resource_inst_t udbRsc = { CY_RSC_UDB, 0, 0 };
    
        retVal = cy_hwmgr_reserve(&udbRsc);
        if (retVal == CY_RSLT_SUCCESS)
        {
            /* Check is the UDB configured */
            retVal = cy_hwmgr_is_configured(udbRsc.type, udbRsc.block_num, udbRsc.channel_num, &isConfigured);
            if (retVal == CY_RSLT_SUCCESS && !isConfigured)
            {
                /* Update SDIO object */
                obj->resource.type = udbRsc.type;
                obj->resource.block_num = udbRsc.block_num;
                obj->resource.channel_num = udbRsc.channel_num;

                SDIO_Init((stc_sdio_irq_cb_t *) &cy_sdio_interrupts_dispatcher_IRQHandler);
                
                /* Register two DMA interrupt handler functions 
                *  TODO: 
                *  1. Need to do this in the DMA hal (once it is implemented)
                *  2. Figure out how to aware rtoses about these handler executions.
                *
                *   __asm__( "PUSH {lr}" ); 
                *   __asm__( "bl _tx_thread_context_save" );
                *   handler(); \
                *   __asm__( "b  _tx_thread_context_restore" ); 
                *  ...
                */
                
                    /* Set default interrupt priority of 7 (lowest possible priority) */
                cy_stc_sysint_t irqCfg = {udb_interrupts_0_IRQn, 7};
                Cy_SysInt_Init(&irqCfg, &SDIO_IRQ);
                NVIC_EnableIRQ(udb_interrupts_0_IRQn);
                
                /* Setup write DMA interrupt handler */
                cy_stc_sysint_t irqDma1_1 = {cpuss_interrupts_dw1_1_IRQn, 7};
                Cy_SysInt_Init(&irqDma1_1, &SDIO_WRITE_DMA_IRQ);
                NVIC_EnableIRQ(cpuss_interrupts_dw1_1_IRQn);
                
                /* Setup read DMA interrupt handler */
                cy_stc_sysint_t irqDma1_3 = {cpuss_interrupts_dw1_3_IRQn, 7};
                Cy_SysInt_Init(&irqDma1_3, &SDIO_READ_DMA_IRQ);
                NVIC_EnableIRQ(cpuss_interrupts_dw1_3_IRQn);

                obj->frequency_hz = 400000U;        /* SDIO_Init() configures the SDIO to 40 kHz */
                obj->block_size   = CY_HAL_SDIO_64B;
                
                /* Connect UDB to DMA */
                Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP14_OUTPUT1, TRIG0_OUT_CPUSS_DW0_TR_IN1, false, TRIGGER_TYPE_LEVEL);
                Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP14_OUTPUT4, TRIG0_OUT_CPUSS_DW0_TR_IN0, false, TRIGGER_TYPE_LEVEL);
                Cy_TrigMux_Connect(TRIG1_IN_TR_GROUP14_OUTPUT0, TRIG1_OUT_CPUSS_DW1_TR_IN1, false, TRIGGER_TYPE_LEVEL);
                Cy_TrigMux_Connect(TRIG1_IN_TR_GROUP14_OUTPUT5, TRIG1_OUT_CPUSS_DW1_TR_IN3, false, TRIGGER_TYPE_LEVEL);
                
                Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB0, TRIG14_OUT_TR_GROUP1_INPUT43, false, TRIGGER_TYPE_LEVEL);
                Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB1, TRIG14_OUT_TR_GROUP0_INPUT44, false, TRIGGER_TYPE_LEVEL);
                Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB3, TRIG14_OUT_TR_GROUP0_INPUT47, false, TRIGGER_TYPE_LEVEL);
                Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB7, TRIG14_OUT_TR_GROUP1_INPUT48, false, TRIGGER_TYPE_LEVEL);
                
                retVal = cy_hwmgr_set_configured(udbRsc.type, udbRsc.block_num, udbRsc.channel_num);
            }
        }
    }

    if (retVal != CY_RSLT_SUCCESS)
    {
        cy_sdio_free(obj);
    }

    return retVal;
}


/** Release the SDIO peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param[in,out] obj The SDIO object
 * @return The status of the free request
 */
cy_rslt_t cy_sdio_free(cy_sdio_t *obj)
{
    cy_rslt_t retVal;
    
    if (NULL == obj)
    {
        return CY_RSLT_SDIO_BAD_PARAM_ARGUMENT;
    }
    
    retVal = cy_free_pins(obj);

    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = cy_free_clocks(obj);
    }
    
    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = cy_free_dmas(obj);
    }
    
    if (retVal == CY_RSLT_SUCCESS)
    {
        /* All other resources were released so need to release UDB resource */
        retVal = cy_hwmgr_set_unconfigured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
    }

    return retVal;
}

/** Configure the SDIO block.
 *
 * @param[in,out] obj    The SDIO object
 * @param[in]     config The SDIO configuration to apply
 * @return The status of the configure request
 */
cy_rslt_t cy_sdio_configure(cy_sdio_t *obj, const cy_sdio_cfg_t *config)
{
    if (NULL == obj)
    {
        return CY_RSLT_SDIO_BAD_PARAM_ARGUMENT;
    }

    SDIO_SetSdClkFrequency(config->frequency_hz);
    SDIO_SetBlockSize(config->block_size);

    obj->frequency_hz = config->frequency_hz;
    obj->block_size   = config->block_size;
    
    return CY_RSLT_SUCCESS;
}

/** Sends a command to the SDIO block.
 *
 * @param[in,out] obj       The SDIO object
 * @param[in]     direction The direction of transfer (read/write)
 * @param[in]     command   The SDIO command to send
 * @param[in]     argument  The argument to the command
 * @param[out]    response  The response from the SDIO device
 * @return The status of the configure request
 */
cy_rslt_t cy_sdio_send_cmd(const cy_sdio_t *obj, cy_transfer_t direction, cy_sdio_command_t command, uint32_t argument, uint32_t* response)
{
    if ((NULL == obj) && (command == SDIO_CMD_IO_RW_EXTENDED))
    {
        return CY_RSLT_SDIO_BAD_PARAM_ARGUMENT;
    }

    if (response != NULL)
    {
        *response = 0;
    }
    
    stc_sdio_cmd_t cmd;
    en_sdio_result_t status;
    cy_rslt_t retVal = CY_RSLT_SUCCESS;

    cmd.u32CmdIdx = (uint32_t) command;
    cmd.u32Arg = argument;
    cmd.pu32Response = response;
    cmd.pu8Data = NULL;               /* Not used */
    cmd.bRead = (direction != CY_READ) ? false : true;
    cmd.u16BlockCnt = 0U;            /* Not used */
    cmd.u16BlockSize = 0U;          /* Not used */

    status = SDIO_SendCommandAndWait(&cmd);

    if (Ok != status)
    {
        /* Return values are mapped on defines located in the hal sdio header */
        retVal = CY_RSLT_SDIO_FUNC_RET_ERROR(status);
    }

    return retVal;
}


/** Performs a bulk data transfer (CMD=53) to the SDIO block.
 *
 * @param[in,out] obj       The SDIO object
 * @param[in]     direction The direction of transfer (read/write)
 * @param[in]     argument  The argument to the command
 * @param[in]     data      The data to send to the SDIO device
 * @param[in]     length    The number of bytes to send
 * @param[out]    response  The response from the SDIO device
 * @return The status of the configure request
 */
cy_rslt_t cy_sdio_bulk_transfer(cy_sdio_t *obj, cy_transfer_t direction, uint32_t argument, const uint32_t* data, uint16_t length, uint32_t* response)
{
    if (NULL == obj)
    {
        return CY_RSLT_SDIO_BAD_PARAM_ARGUMENT;
    }

    stc_sdio_cmd_t cmd;
    en_sdio_result_t status;
    cy_rslt_t retVal;

    cmd.u32CmdIdx = (uint32_t) SDIO_CMD_IO_RW_EXTENDED;
    cmd.u32Arg = argument;
    cmd.pu32Response = response;
    cmd.pu8Data = (uint8_t *) data;
    cmd.bRead = (direction != CY_READ) ? false : true;

    if (length >= CY_HAL_SDIO_64B)
    {
        cmd.u16BlockCnt = (uint16_t) ((length + CY_HAL_SDIO_64B - 1)/CY_HAL_SDIO_64B);
        cmd.u16BlockSize = CY_HAL_SDIO_64B; 

        /* Update object info */
        obj->block_size = CY_HAL_SDIO_64B;
    }
    else
    {
        /* Data will be sent as one packet */
        cmd.u16BlockCnt = CY_HAL_SDIO_1B;
        cmd.u16BlockSize = length;
        
        /* Update object info */
        obj->block_size = length;
    }

    status = SDIO_SendCommandAndWait(&cmd);

    if (Ok != status)
    {
        /* return values are mapped on defines located in the hal sdio header */
        retVal = CY_RSLT_SDIO_FUNC_RET_ERROR(status);
    }
    else
    {
        retVal = CY_RSLT_SUCCESS;
    }

    return retVal;
}

/** Performs a bulk asynchronus data transfer (CMD=53) to the SDIO block.
 *
 * @param[in,out] obj       The SDIO object
 * @param[in]     direction The direction of transfer (read/write)
 * @param[in]     argument  The argument to the command
 * @param[in]     data      The data to send to the SDIO device
 * @param[in]     length    The number of bytes to send
 * @return The status of the configure request
 */
cy_rslt_t cy_sdio_transfer_async(cy_sdio_t *obj, cy_transfer_t direction, uint32_t argument, const uint32_t* data, uint16_t length)
{
    if (NULL == obj)
    {
        return CY_RSLT_SDIO_BAD_PARAM_ARGUMENT;
    }

    stc_sdio_cmd_t cmd;
    en_sdio_result_t status;
    cy_rslt_t retVal;

    cmd.u32CmdIdx = (uint32_t) SDIO_CMD_IO_RW_EXTENDED;
    cmd.u32Arg = argument;
    cmd.pu32Response = NULL;
    cmd.pu8Data = (uint8_t *) data;
    cmd.bRead = (direction != CY_READ) ? false : true;

    if (length >= CY_HAL_SDIO_64B)
    {
        cmd.u16BlockCnt = (uint16_t) ((length + CY_HAL_SDIO_64B - 1)/CY_HAL_SDIO_64B);
        cmd.u16BlockSize = CY_HAL_SDIO_64B; 

        /* Update object info */
        obj->block_size = CY_HAL_SDIO_64B;
    }
    else
    {
        /* Data will be sent as one packet */
        cmd.u16BlockCnt = CY_HAL_SDIO_1B;
        cmd.u16BlockSize = length;
        
        /* Update object info */
        obj->block_size = length;
    }

    status = SDIO_SendCommandAndWait(&cmd);

    if (Ok != status)
    {
        /* return values are mapped on defines located in the hal sdio header */
        retVal = CY_RSLT_SDIO_FUNC_RET_ERROR(status);
    }
    else
    {
        retVal = CY_RSLT_SUCCESS;
    }

    return retVal;
}


/** Checks if the specified SDIO is in use
 *
 * @param[in]  obj  The SDIO peripheral to check
 * @param[out] busy Indication of whether the SDIO is still transmitting
 * @return The status of the is_busy request
 */
cy_rslt_t cy_sdio_is_busy(const cy_sdio_t *obj, bool *busy)
{
    if (obj == NULL)
    {
        return CY_RSLT_SDIO_BAD_PARAM_ARGUMENT;
    }
    
    en_sdio_result_t retVal = SDIO_GetSemaphoreStatus(busy);

    return ((Ok != retVal) ? CY_RSLT_SDIO_SEMA_NOT_INITED : CY_RSLT_SUCCESS);
}

/** Abort an SDIO transfer
 *
 * @param[in] obj The SDIO peripheral to stop. Resets UDB block
 * @return The status of the abort_async request
 */
cy_rslt_t cy_sdio_abort_async(const cy_sdio_t *obj)
{
    /* Reset UDB SDIO */
    SDIO_Reset();
    return CY_RSLT_SUCCESS;
}

/** The sdio interrupt handler registration
 *
 * @param[in] obj         The SDIO object
 * @param[in] handler     The callback handler which will be invoked when the interrupt fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 * @return The status of the register_irq request
 */
cy_rslt_t cy_sdio_register_irq(cy_sdio_t *obj, cy_sdio_irq_handler handler, void *handler_arg)
{
    cy_hal_sdio_config_struct = obj;
    cy_hal_sdio_callback = handler;
    cy_hal_sdio_callback_args = handler_arg;

    return CY_RSLT_SUCCESS;
}

/** Configure SDIO interrupt.
 *
 * @param[in] obj      The SDIO object
 * @param[in] event    The SDIO IRQ type
 * @param[in] enable   Set to non-zero to enable events, or zero to disable them
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_sdio_irq_enable(cy_sdio_t *obj, cy_sdio_irq_event_t event, bool enable)
{
    if (enable)
    {
        obj->irqCause |= event;
        
        if (cy_hal_sdio_config_struct != NULL)
        {
            cy_hal_sdio_config_struct->irqCause |= event;
        }
        
        SDIO_EnableChipInt();
    }
    else
    {
        obj->irqCause &= ~event;

        if (cy_hal_sdio_config_struct != NULL)
        {
            cy_hal_sdio_config_struct->irqCause &= ~event;
        }
        
        SDIO_DisableChipInt();
    }
    return CY_RSLT_SUCCESS;
}


#endif /* defined(CY_IP_MXUDB) */
