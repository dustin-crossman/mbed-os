/*******************************************************************************
* File Name: cyhal_usb_dev.c
*
* \brief
* Provides a high level interface for interacting with the Cypress USB Device.
* This interface abstracts out the chip specific details. 
* If any chip specific functionality is necessary, or performance is critical 
* the low level functions can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2019 Cypress Semiconductor Corporation
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

#include "cyhal_usb_dev.h"
#include "cyhal_interconnect.h"

#if defined(CY_IP_MXUSBFS)

/* Interrupt configuration and access */
#define CYHAL_USB_DEV_DEFAULT_IRQ_PRIORITY    (3U)

/* Assign all interrupt sources to Low interrupt */
#define CYHAL_USB_DEV_IRQ_LVL                   (CY_USBFS_DEV_DRV_LVL_LOW)
#define CYHAL_USB_DEV_IRQ_LVL_DEFAULT           (CY_USBFS_DEV_DRV_SET_SOF_LVL(CYHAL_USB_DEV_IRQ_LVL)      | \
                                                CY_USBFS_DEV_DRV_SET_BUS_RESET_LVL(CYHAL_USB_DEV_IRQ_LVL) | \
                                                CY_USBFS_DEV_DRV_SET_EP0_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_LPM_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_ARB_EP_LVL(CYHAL_USB_DEV_IRQ_LVL)    | \
                                                CY_USBFS_DEV_DRV_SET_EP1_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_EP2_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_EP3_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_EP4_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_EP5_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_EP6_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_EP7_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_EP8_LVL(CYHAL_USB_DEV_IRQ_LVL))

#define CYHAL_USB_DEV_IS_EVENT_VALID(event)     ( ((event) == CYHAL_USB_DEV_EVENT_BUS_RESET) || \
                                                  ((event) == CYHAL_USB_DEV_EVENT_EP0_SETUP) || \
                                                  ((event) == CYHAL_USB_DEV_EVENT_EP0_IN)    || \
                                                  ((event) == CYHAL_USB_DEV_EVENT_EP0_OUT) )

#define CYHAL_USB_DEV_IS_EP_NUM_VALID(endpoint)     CY_USBFS_DEV_DRV_IS_EP_VALID(endpoint)
#define CYHAL_USB_DEV_IS_EP0_SIZE_VALID(ep0_size)   (ep0_size >= CY_USBFS_DEV_DRV_EP0_BUFFER_SIZE)

#define CYHAL_USB_DEV_EVENT_NUM          (4)
#define CYHAL_USB_DEV_EP_EVENT_NUM       (CY_USBFS_DEV_DRV_NUM_EPS_MAX)

/* Clock configuration constants */
#define CYHAL_USB_DEV_BUS_RESET_CLOCK_HZ        (100000U)
#define CYHAL_USB_DEV_PLL_PATH                  (CY_SYSCLK_CLKHF_IN_CLKPATH1)
#define CYHAL_USB_DEV_USB_CLK_HF                (3U)
#define CYHAL_USB_DEV_PLL_ENABLE_TIMEOUT        (1000U)

typedef enum
{
    CYHAL_USB_DEV_EP1_IDX = 0,     /* Callback number for Endpoint 1 completion interrupt */
    CYHAL_USB_DEV_EP2_IDX = 1,     /* Callback number for Endpoint 2 completion interrupt */
    CYHAL_USB_DEV_EP3_IDX = 2,     /* Callback number for Endpoint 3 completion interrupt */
    CYHAL_USB_DEV_EP4_IDX = 3,     /* Callback number for Endpoint 4 completion interrupt */
    CYHAL_USB_DEV_EP5_IDX = 4,     /* Callback number for Endpoint 5 completion interrupt */
    CYHAL_USB_DEV_EP6_IDX = 5,     /* Callback number for Endpoint 6 completion interrupt */
    CYHAL_USB_DEV_EP7_IDX = 6,     /* Callback number for Endpoint 7 completion interrupt */
    CYHAL_USB_DEV_EP8_IDX = 7,     /* Callback number for Endpoint 8 completion interrupt */
} cyhal_usb_dev_ep_cb_num_t;

typedef struct
{
    cyhal_usb_dev_endpoint_handler_t  ep_handler;
    cyhal_usb_dev_ep_t                ep_address;
} cyhal_usb_dev_ep_handler_attr_t;


static USBFS_Type* const CYHAL_USB_DEV_BASE_ADDRESSES[CY_IP_MXUSBFS_INSTANCES] = 
{
#ifdef USBFS0
    USBFS0,
#endif
};

IRQn_Type CYHAL_USBDEV_IRQ_N[CY_IP_MXUSBFS_INSTANCES] =
{
#ifdef USBFS0
    usb_interrupt_lo_IRQn,
#endif
};

static void cyhal_usb_0_dev_bus_reset_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext);
static void cyhal_usb_0_dev_ep0_setup_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext);
static void cyhal_usb_0_dev_ep0_in_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext);
static void cyhal_usb_0_dev_ep0_out_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext);
static cyhal_usb_dev_event_handler_t cyhal_usb_dev_event_callback_table[CY_IP_MXUSBFS_INSTANCES][CYHAL_USB_DEV_EVENT_NUM]; 
static cy_cb_usbfs_dev_drv_callback_t cyhal_usb_dev_drv_event_cb_table[CY_IP_MXUSBFS_INSTANCES][CYHAL_USB_DEV_EVENT_NUM] = 
{
    /* USBFS0 */
    {
        &cyhal_usb_0_dev_bus_reset_callback,    /* CY_USB_DEV_BUS_RESET = 0U */ 
        &cyhal_usb_0_dev_ep0_setup_callback,    /* CY_USB_DEV_EP0_SETUP = 1U */ 
        &cyhal_usb_0_dev_ep0_in_callback,       /* CY_USB_DEV_EP0_IN    = 2U */ 
        &cyhal_usb_0_dev_ep0_out_callback       /* CY_USB_DEV_EP0_OUT   = 3U */  
    },
};

static void cyhal_usb_0_dev_sof_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext);
static cyhal_usb_dev_sof_handler_t cyhal_usb_dev_sof_user_callback[CY_IP_MXUSBFS_INSTANCES];
static cy_cb_usbfs_dev_drv_callback_t cyhal_usb_dev_drv_sof_cb_table[CY_IP_MXUSBFS_INSTANCES]=
{
    /* USBFS0 */
    &cyhal_usb_0_dev_sof_callback,
};

static void cyhal_usb_dev_ep_callback (uint32_t idx, uint32_t endpoint);
static void cyhal_usb_0_dev_ep1_callback(USBFS_Type *base, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext);
static void cyhal_usb_0_dev_ep2_callback(USBFS_Type *base, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext);
static void cyhal_usb_0_dev_ep3_callback(USBFS_Type *base, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext);
static void cyhal_usb_0_dev_ep4_callback(USBFS_Type *base, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext);
static void cyhal_usb_0_dev_ep5_callback(USBFS_Type *base, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext);
static void cyhal_usb_0_dev_ep6_callback(USBFS_Type *base, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext);
static void cyhal_usb_0_dev_ep7_callback(USBFS_Type *base, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext);
static void cyhal_usb_0_dev_ep8_callback(USBFS_Type *base, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext);
static cyhal_usb_dev_ep_handler_attr_t cyhal_usb_dev_ep_handler_table[CY_IP_MXUSBFS_INSTANCES][CYHAL_USB_DEV_EP_EVENT_NUM];

static cy_cb_usbfs_dev_drv_ep_callback_t cyhal_usb_dev_drv_ep_cb_table[CY_IP_MXUSBFS_INSTANCES][CYHAL_USB_DEV_EP_EVENT_NUM] =
{
    /* USBFS0 */
    {
        &cyhal_usb_0_dev_ep1_callback,
        &cyhal_usb_0_dev_ep2_callback,
        &cyhal_usb_0_dev_ep3_callback,
        &cyhal_usb_0_dev_ep4_callback,
        &cyhal_usb_0_dev_ep5_callback,
        &cyhal_usb_0_dev_ep6_callback,
        &cyhal_usb_0_dev_ep7_callback,
        &cyhal_usb_0_dev_ep8_callback,
    },
};

static cy_rslt_t cyhal_usb_dev_clock_setup(cyhal_clock_divider_t *clk_div, cyhal_clock_divider_t *user_clk_div);
static cy_rslt_t cyhal_usb_dev_pin_setup(cyhal_usb_dev_t *obj, cyhal_gpio_t dp, 
                                         cyhal_gpio_t dm);
static void cyhal_usb_dev_free_pin(cyhal_gpio_t pin);
static void cyhal_usb_dev_free_resources(cyhal_usb_dev_t *obj);

static void cyhal_usb_0_dev_bus_reset_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext)
{
    (void) base; (void) drvContext;

    cyhal_usb_dev_event_callback_table[0][CYHAL_USB_DEV_EVENT_BUS_RESET]();
}

static void cyhal_usb_0_dev_ep0_setup_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext)
{
    (void) base; (void) drvContext;

    cyhal_usb_dev_event_callback_table[0][CYHAL_USB_DEV_EVENT_EP0_SETUP]();
}

static void cyhal_usb_0_dev_ep0_in_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext)
{
    (void) base; (void) drvContext;

    cyhal_usb_dev_event_callback_table[0][CYHAL_USB_DEV_EVENT_EP0_IN]();
}

static void cyhal_usb_0_dev_ep0_out_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext)
{
    (void) base; (void) drvContext;

    cyhal_usb_dev_event_callback_table[0][CYHAL_USB_DEV_EVENT_EP0_OUT]();
}

void cyhal_usb_dev_register_event_callback(cyhal_usb_dev_t *obj, cyhal_usb_dev_event_t event, cyhal_usb_dev_event_handler_t handler)
{
    CY_ASSERT_L1(CYHAL_USB_DEV_IS_EVENT_VALID(event));
    uint32_t idx = (uint32_t)obj->resource.block_num;

    cyhal_usb_dev_event_callback_table[idx][event] = handler;
}


static void cyhal_usb_dev_ep_callback(uint32_t idx, uint32_t endpoint_idx)
{
    cyhal_usb_dev_endpoint_handler_t ep_handler = cyhal_usb_dev_ep_handler_table[idx][endpoint_idx].ep_handler;

    if (NULL != ep_handler)
    {
        ep_handler(cyhal_usb_dev_ep_handler_table[idx][endpoint_idx].ep_address);
    }
}

static void cyhal_usb_0_dev_ep1_callback(USBFS_Type *base, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext)
{   
    (void) base; (void) errorType; (void) drvContext; 
    cyhal_usb_dev_ep_callback(0U, CYHAL_USB_DEV_EP1_IDX);
}

static void cyhal_usb_0_dev_ep2_callback(USBFS_Type *base, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext)
{
    (void) base; (void) errorType; (void) drvContext; 
    cyhal_usb_dev_ep_callback(0U, CYHAL_USB_DEV_EP2_IDX);
}

static void cyhal_usb_0_dev_ep3_callback(USBFS_Type *base, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext)
{
    (void) base; (void) errorType; (void) drvContext; 
    cyhal_usb_dev_ep_callback(0U,CYHAL_USB_DEV_EP3_IDX);
}

static void cyhal_usb_0_dev_ep4_callback(USBFS_Type *base, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext)
{
    (void) base; (void) errorType; (void) drvContext; 
    cyhal_usb_dev_ep_callback(0U, CYHAL_USB_DEV_EP4_IDX);
}

static void cyhal_usb_0_dev_ep5_callback(USBFS_Type *base, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext)
{
    (void) base; (void) errorType; (void) drvContext; 
    cyhal_usb_dev_ep_callback(0U, CYHAL_USB_DEV_EP5_IDX);
}

static void cyhal_usb_0_dev_ep6_callback(USBFS_Type *base, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext)
{
    (void) base; (void) errorType; (void) drvContext; 
    cyhal_usb_dev_ep_callback(0U, CYHAL_USB_DEV_EP6_IDX);
}

static void cyhal_usb_0_dev_ep7_callback(USBFS_Type *base, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext)
{
    (void) base; (void) errorType; (void) drvContext; 
    cyhal_usb_dev_ep_callback(0U, CYHAL_USB_DEV_EP7_IDX);
}

static void cyhal_usb_0_dev_ep8_callback(USBFS_Type *base, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext)
{
    (void) base; (void) errorType; (void) drvContext; 
    cyhal_usb_dev_ep_callback(0U, CYHAL_USB_DEV_EP8_IDX);
}

void cyhal_usb_dev_register_endpoint_callback(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint, cyhal_usb_dev_endpoint_handler_t handler)
{
    uint32_t endpoint_num = CYHAL_USB_DEV_GET_EP_NUM(endpoint);
    uint32_t idx = (uint32_t)obj->resource.block_num;

    CY_ASSERT_L1(CYHAL_USB_DEV_IS_EP_NUM_VALID(endpoint_num));

    cyhal_usb_dev_ep_handler_table[idx][CYHAL_USB_DEV_GET_EP_IDX(endpoint_num)].ep_address = endpoint;
    cyhal_usb_dev_ep_handler_table[idx][CYHAL_USB_DEV_GET_EP_IDX(endpoint_num)].ep_handler = handler;
}

static cy_rslt_t cyhal_usb_dev_clock_setup(cyhal_clock_divider_t *clk_div, cyhal_clock_divider_t *user_clk_div)
{
    cy_rslt_t result;
    cy_en_sysclk_status_t status = CY_SYSCLK_BAD_PARAM;
    
    if (NULL == user_clk_div)
    {
        /* USB bus reset clock must be 100KHz. Usual peri clock frequency is > 26 MHz, which requires 16-bit divider */ 
        result = cyhal_hwmgr_allocate_clock(clk_div, CY_SYSCLK_DIV_16_BIT, false);
    }
    else
    {
        *clk_div = *user_clk_div;
        result = CY_RSLT_SUCCESS;
    }
    

    if (CY_RSLT_SUCCESS == result)
    {
        status = Cy_SysClk_PeriphAssignDivider(PCLK_USB_CLOCK_DEV_BRS, clk_div->div_type, clk_div->div_num);
    }

    if (CY_SYSCLK_SUCCESS == status)
    {
        status = Cy_SysClk_PeriphDisableDivider(clk_div->div_type, clk_div->div_num);
    }

    if (CY_SYSCLK_SUCCESS == status)
    {
        /* Get divider to provide 100kHz clock or less */
        uint32_t div_value = (cy_PeriClkFreqHz / CYHAL_USB_DEV_BUS_RESET_CLOCK_HZ) - 1U;    
	    status = Cy_SysClk_PeriphSetDivider(clk_div->div_type, clk_div->div_num, div_value);
    }

    if (CY_SYSCLK_SUCCESS == status)
    {
	    status = Cy_SysClk_PeriphEnableDivider(clk_div->div_type, clk_div->div_num); 
    }

    if (CY_SYSCLK_SUCCESS == status)
    {
        /* Disable PLL before configuring */
        status = Cy_SysClk_PllDisable(CYHAL_USB_DEV_PLL_PATH);
    }
    if (CY_SYSCLK_SUCCESS == status)
    {
        /* Connect IMO to PLL input */
        status = Cy_SysClk_ClkPathSetSource(CYHAL_USB_DEV_PLL_PATH, 
                                            CY_SYSCLK_CLKPATH_IN_IMO);
    }
    /* Configure and enable PLL: inline void Cy_SysClk_Pll0Init(void) */
    if (CY_SYSCLK_SUCCESS == status)
    {
        /* PLL configuration for 48MHz 0.25% */
        static const cy_stc_pll_manual_config_t pll_config_48MHz = {
            .feedbackDiv = 30U,    
            .referenceDiv = 1U,    
            .outputDiv = 5U,       
            .lfMode = false,      
            .outputMode = CY_SYSCLK_FLLPLL_OUTPUT_AUTO, 
        };
        status = Cy_SysClk_PllManualConfigure(CYHAL_USB_DEV_PLL_PATH, 
                                              &pll_config_48MHz);
    }

    if (CY_SYSCLK_SUCCESS == status)
    {
        status = Cy_SysClk_PllEnable(CYHAL_USB_DEV_PLL_PATH, 
                                     CYHAL_USB_DEV_PLL_ENABLE_TIMEOUT);
    }

    /* Connect PLL (CLK_PATH1) to USB HF clock (HF3) */
    if (CY_SYSCLK_SUCCESS == status)
    {
        status = Cy_SysClk_ClkHfSetSource(CYHAL_USB_DEV_USB_CLK_HF, CYHAL_USB_DEV_PLL_PATH);
    }
    if (CY_SYSCLK_SUCCESS == status)
    {
        status = Cy_SysClk_ClkHfSetDivider(CYHAL_USB_DEV_USB_CLK_HF, CY_SYSCLK_CLKHF_NO_DIVIDE);
    }
    if (CY_SYSCLK_SUCCESS == status)
    {
        status = Cy_SysClk_ClkHfEnable(CYHAL_USB_DEV_USB_CLK_HF);
    }

    if (CY_SYSCLK_SUCCESS != status)
    {
        result = CYHAL_USB_DEV_RSLT_ERR;
    }
    return result;
} 

static cy_rslt_t cyhal_usb_dev_pin_setup(cyhal_usb_dev_t *obj, cyhal_gpio_t dp, 
                                         cyhal_gpio_t dm)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;
    
    const cyhal_resource_pin_mapping_t *dp_map = CY_UTILS_GET_RESOURCE(dp, cyhal_pin_map_usb_usb_dp_pad);
    const cyhal_resource_pin_mapping_t *dm_map = CY_UTILS_GET_RESOURCE(dm, cyhal_pin_map_usb_usb_dm_pad);

    if((NULL != dp_map) && (NULL != dm_map) && 
       (dp_map->inst->block_num == dm_map->inst->block_num))
    {
        cyhal_resource_inst_t pin_rsc;

        obj->resource = *dp_map->inst;
        
        /* reserve DM and DP pins */
        pin_rsc = cyhal_utils_get_gpio_resource(dp);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (CY_RSLT_SUCCESS == result)
        {
            obj->pin_dp = dp;
            pin_rsc = cyhal_utils_get_gpio_resource(dm);
            result = cyhal_hwmgr_reserve(&pin_rsc);
        }

        if (CY_RSLT_SUCCESS == result)
        {
            obj->pin_dm = dm;
            result = cyhal_connect_pin(dp_map);
        }
        
        if (CY_RSLT_SUCCESS == result)
        {
            result = cyhal_connect_pin(dm_map);
        }
    }

    return result;
}

static void cyhal_usb_dev_free_pin(cyhal_gpio_t pin)
{
    if (CY_RSLT_SUCCESS == cyhal_disconnect_pin(pin))
    {
        cyhal_resource_inst_t rsc;
        rsc = cyhal_utils_get_gpio_resource(pin);
        (void) cyhal_hwmgr_free(&rsc);
    }
}

static void cyhal_usb_dev_free_resources(cyhal_usb_dev_t *obj)
{
    Cy_USBFS_Dev_Drv_DeInit(obj->base, &(obj->context));
    (void) cyhal_hwmgr_free(&(obj->resource));
    cyhal_usb_dev_free_pin(obj->pin_dp);
    cyhal_usb_dev_free_pin(obj->pin_dm);
    cyhal_hwmgr_free_clock(&(obj->clock));
}

cy_rslt_t cyhal_usb_dev_init(cyhal_usb_dev_t *obj, cyhal_gpio_t dp, cyhal_gpio_t dm,
                             const cyhal_clock_divider_t *clkPll, cyhal_clock_divider_t *clkDiv,
                             const cyhal_usb_dev_cfg_t *cfg)
{
    cy_rslt_t result = cyhal_usb_dev_pin_setup(obj, dp, dm);
    uint32_t idx = 0; 

    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_usb_dev_clock_setup(&(obj->clock), clkDiv);
        idx = (uint32_t)obj->resource.block_num;
    }

    if (CY_RSLT_SUCCESS == result)
    {
        obj->base = CYHAL_USB_DEV_BASE_ADDRESSES[idx];
    }

    bool configured = cyhal_hwmgr_is_configured(obj->resource.type, 
                                           obj->resource.block_num, 
                                           obj->resource.channel_num);

    if ((CY_RSLT_SUCCESS == result) && !configured)
    {
        static cy_stc_usbfs_dev_drv_config_t default_cfg =
        {
            .mode         = CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU,
            .dmaConfig[0] = NULL,
            .dmaConfig[1] = NULL, 
            .dmaConfig[2] = NULL, 
            .dmaConfig[3] = NULL, 
            .dmaConfig[4] = NULL, 
            .dmaConfig[5] = NULL, 
            .dmaConfig[6] = NULL,
            .dmaConfig[7] = NULL, 
            .epBuffer     = NULL,
            .epBufferSize = 0U,
            .intrLevelSel = CYHAL_USB_DEV_IRQ_LVL_DEFAULT,
            .enableLpm    = false,
            .epAccess     = CY_USBFS_DEV_DRV_USE_16_BITS_DR
        };

        if (NULL == cfg)
        {         
            if (CY_USBFS_DEV_DRV_SUCCESS != Cy_USBFS_Dev_Drv_Init(obj->base, 
                                                                  &default_cfg, 
                                                                  &(obj->context)))
            {
                result = CYHAL_USB_DEV_RSLT_ERR;
            }
        }
        else
        {
            default_cfg.epBuffer     = cfg->ep_buffer;
            default_cfg.epBufferSize = cfg->ep_buffer_size;

            if(CYHAL_USB_DEV_MODE1_16_BIT != cfg->mode)
            {
                /* Mode is not supported */
                result = CYHAL_USB_DEV_RSLT_ERR;
            }

            if (CY_USBFS_DEV_DRV_SUCCESS != Cy_USBFS_Dev_Drv_Init(obj->base, 
                                                                  &default_cfg, 
                                                                  &(obj->context)))
            {
                result = CYHAL_USB_DEV_RSLT_ERR;
            }
        }
    }
    
    if (CY_RSLT_SUCCESS == result)
    {
        uint32_t cb_num;
        
        /* Register service callbacks */
        Cy_USBFS_Dev_Drv_RegisterServiceCallback(obj->base, CY_USB_DEV_BUS_RESET, 
                                                cyhal_usb_dev_drv_event_cb_table[idx][CY_USB_DEV_BUS_RESET], 
                                                &(obj->context));
        Cy_USBFS_Dev_Drv_RegisterServiceCallback(obj->base, CY_USB_DEV_EP0_SETUP, 
                                                cyhal_usb_dev_drv_event_cb_table[idx][CY_USB_DEV_EP0_SETUP], 
                                                &(obj->context));
        Cy_USBFS_Dev_Drv_RegisterServiceCallback(obj->base, CY_USB_DEV_EP0_IN, 
                                                cyhal_usb_dev_drv_event_cb_table[idx][CY_USB_DEV_EP0_IN], 
                                                &(obj->context));
        Cy_USBFS_Dev_Drv_RegisterServiceCallback(obj->base, CY_USB_DEV_EP0_OUT,
                                                cyhal_usb_dev_drv_event_cb_table[idx][CY_USB_DEV_EP0_OUT],
                                                &(obj->context));

        /* Register sof callback (it enables sof interrupt, so disable it after registration)*/
        Cy_USBFS_Dev_Drv_RegisterSofCallback(obj->base, cyhal_usb_dev_drv_sof_cb_table[idx], 
                                             &(obj->context));
        cyhal_usb_dev_sof_enable(obj, false);

        /* Register data endpoint handlers*/
        for (cb_num = 0; cb_num < CYHAL_USB_DEV_EP_EVENT_NUM; cb_num++)
        {
            Cy_USBFS_Dev_Drv_RegisterEndpointCallback(obj->base, 
                                                      (cb_num + 1U), 
                                                      cyhal_usb_dev_drv_ep_cb_table[idx][cb_num], 
                                                      &(obj->context));
        }
    }
    else
    {
        cyhal_usb_dev_free_resources(obj);
    }
    
    return result;
}


void cyhal_usb_dev_free(cyhal_usb_dev_t *obj)
{
    cyhal_usb_dev_irq_enable(obj, false);
    cyhal_usb_dev_disconnect(obj);
    cyhal_usb_dev_free_resources(obj);
}


void cyhal_usb_dev_connect(cyhal_usb_dev_t *obj)
{
    Cy_USBFS_Dev_Drv_Enable(obj->base, &(obj->context));
}


void cyhal_usb_dev_disconnect(cyhal_usb_dev_t *obj)
{
    Cy_USBFS_Dev_Drv_Disable(obj->base, &(obj->context));
}


void cyhal_usb_dev_configure(cyhal_usb_dev_t *obj)
{
    Cy_USBFS_Dev_Drv_ConfigDevice(obj->base, &(obj->context));
}


void cyhal_usb_dev_unconfigure(cyhal_usb_dev_t *obj)
{
    Cy_USBFS_Dev_Drv_UnConfigureDevice(obj->base, &(obj->context));
}


void cyhal_usb_dev_sof_enable(cyhal_usb_dev_t *obj, bool enable)
{
    uint32_t mask = Cy_USBFS_Dev_Drv_GetSieInterruptMask(obj->base);
    
    if (enable)
    {
        mask |= CY_USBFS_DEV_DRV_INTR_SIE_SOF;
        Cy_USBFS_Dev_Drv_ClearSieInterrupt(obj->base, CY_USBFS_DEV_DRV_INTR_SIE_SOF);
    }
    else
    {
        mask &= ~CY_USBFS_DEV_DRV_INTR_SIE_SOF;
    }
    
    Cy_USBFS_Dev_Drv_SetSieInterruptMask(obj->base, mask);
}


void cyhal_usb_dev_set_address(cyhal_usb_dev_t *obj, uint8_t address)
{
    Cy_USBFS_Dev_Drv_SetAddress(obj->base, address, &(obj->context));
}


uint32_t cyhal_usb_dev_ep0_get_max_packet(cyhal_usb_dev_t *obj)
{
    return Cy_USBFS_Dev_Drv_GetEp0MaxPacket(obj->base);
}


void cyhal_usb_dev_ep0_setup_read_result(cyhal_usb_dev_t *obj, uint8_t *buffer, uint32_t size)
{
    CY_ASSERT_L1(CYHAL_USB_DEV_IS_EP0_SIZE_VALID(size));

    Cy_USBFS_Dev_Drv_Ep0GetSetup(obj->base, buffer, &(obj->context));
}


void cyhal_usb_dev_ep0_read(cyhal_usb_dev_t *obj, uint8_t *buffer, uint32_t size)
{
    Cy_USBFS_Dev_Drv_Ep0Read(obj->base, buffer, size, &(obj->context));
}


uint32_t cyhal_usb_dev_ep0_read_result(cyhal_usb_dev_t *obj)
{
    return Cy_USBFS_Dev_Drv_Ep0ReadResult(obj->base, &(obj->context));
}


uint32_t cyhal_usb_dev_ep0_write(cyhal_usb_dev_t *obj, uint8_t *buffer, uint32_t size)
{   
    return Cy_USBFS_Dev_Drv_Ep0Write(obj->base, buffer, size, &(obj->context));
}


void cyhal_usb_dev_ep0_stall(cyhal_usb_dev_t *obj)
{
    Cy_USBFS_Dev_Drv_Ep0Stall(obj->base);
}


cy_rslt_t cyhal_usb_dev_endpoint_add(cyhal_usb_dev_t *obj, bool alloc, bool enable,
                                     cyhal_usb_dev_ep_t endpoint, uint32_t max_packet, cyhal_usb_dev_ep_type_t type)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;
    cy_stc_usb_dev_ep_config_t ep_config;

    /* Set parameters to allocate endpoint buffer */
    ep_config.allocBuffer    = alloc;
    ep_config.bufferSize     = (uint16_t) max_packet;

    /* Set parameters to enable endpoint operation */
    ep_config.enableEndpoint = enable;
    ep_config.endpointAddr   = (uint8_t) endpoint;
    ep_config.attributes     = (uint8_t) type;
    ep_config.maxPacketSize  = (uint16_t) max_packet;
    
    if (CY_USBFS_DEV_DRV_SUCCESS ==  Cy_USBFS_Dev_Drv_AddEndpoint(obj->base, 
                                                                  &ep_config, 
                                                                  &(obj->context)))
    {
        result = CY_RSLT_SUCCESS;
    }

    return result;
}


cy_rslt_t cyhal_usb_dev_endpoint_remove(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;

    if (CY_USBFS_DEV_DRV_SUCCESS == Cy_USBFS_Dev_Drv_RemoveEndpoint(obj->base, 
                                                                    endpoint, 
                                                                    &(obj->context)))
    {
        result = CY_RSLT_SUCCESS;
    }
    
    return result;
}


cy_rslt_t cyhal_usb_dev_endpoint_stall(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;

    if (CY_USBFS_DEV_DRV_SUCCESS == Cy_USBFS_Dev_Drv_StallEndpoint(obj->base, 
                                                                   CYHAL_USB_DEV_GET_EP_NUM(endpoint), 
                                                                   &(obj->context)))
    {
        result = CY_RSLT_SUCCESS;
    }
    
    return result;
}


cy_rslt_t cyhal_usb_dev_endpoint_unstall(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;

    if (CY_USBFS_DEV_DRV_SUCCESS == Cy_USBFS_Dev_Drv_UnStallEndpoint(obj->base, 
                                                                     CYHAL_USB_DEV_GET_EP_NUM(endpoint), 
                                                                     &(obj->context)))
    {
        result = CY_RSLT_SUCCESS;
    }
    
    return result;
}


bool cyhal_usb_dev_endpoint_is_stalled(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint)
{
    bool is_stalled = (CY_USB_DEV_EP_STALLED == Cy_USBFS_Dev_Drv_GetEndpointState(obj->base, 
                                                                                  CYHAL_USB_DEV_GET_EP_NUM(endpoint), 
                                                                                  &(obj->context)));

    return is_stalled;
}


cy_rslt_t cyhal_usb_dev_endpoint_read(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint, uint8_t *data, uint32_t size)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;
    uint32_t  ep_num = CYHAL_USB_DEV_GET_EP_NUM(endpoint);
    
    /* Check if endpoint is read for a read operation */
    cy_en_usb_dev_ep_state_t ep_state;
    ep_state = Cy_USBFS_Dev_Drv_GetEndpointState(obj->base, ep_num, &(obj->context));
    if ((CY_USB_DEV_EP_IDLE == ep_state) || (CY_USB_DEV_EP_COMPLETED == ep_state))
    {
        /* Save pointer and size to use in cyhal_usb_dev_endpoint_read_result */
        obj->rd_data[CYHAL_USB_DEV_GET_EP_IDX(endpoint)] = data;
        obj->rd_size[CYHAL_USB_DEV_GET_EP_IDX(endpoint)] = size;

        Cy_USBFS_Dev_Drv_EnableOutEndpoint(obj->base, ep_num, &obj->context);

        result = CY_RSLT_SUCCESS;
    }

    return result;
}


cy_rslt_t cyhal_usb_dev_endpoint_read_result(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint, uint32_t *actSize)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;
    uint32_t  ep_num = CYHAL_USB_DEV_GET_EP_NUM(endpoint);
    
    /* Check if endpoint is read for a read result operation */
    if (CY_USB_DEV_EP_COMPLETED == Cy_USBFS_Dev_Drv_GetEndpointState(obj->base, ep_num, &(obj->context)))
    {
        cy_en_usbfs_dev_drv_status_t drvStatus;
        drvStatus = Cy_USBFS_Dev_Drv_ReadOutEndpoint(obj->base, 
                                                     ep_num, 
                                                     obj->rd_data[CYHAL_USB_DEV_GET_EP_IDX(endpoint)], 
                                                     obj->rd_size[CYHAL_USB_DEV_GET_EP_IDX(endpoint)], 
                                                     actSize, 
                                                     &(obj->context));
        
        if (drvStatus == CY_USBFS_DEV_DRV_SUCCESS)
        {
            result = CY_RSLT_SUCCESS;
        }
    }

    return result;
}


cy_rslt_t cyhal_usb_dev_endpoint_write(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint, uint8_t const *data, uint32_t size)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;
    uint32_t  ep_num = CYHAL_USB_DEV_GET_EP_NUM(endpoint);
    
    /* Check if endpoint is read for a write operation */
    cy_en_usb_dev_ep_state_t ep_state;
    ep_state = Cy_USBFS_Dev_Drv_GetEndpointState(obj->base, ep_num, &(obj->context));
    if ((CY_USB_DEV_EP_IDLE == ep_state) || (CY_USB_DEV_EP_COMPLETED == ep_state))
    {
        cy_en_usbfs_dev_drv_status_t drvStatus;
        drvStatus = Cy_USBFS_Dev_Drv_LoadInEndpoint(obj->base, 
                                                    ep_num, 
                                                    data, 
                                                    size, 
                                                    &(obj->context));

        if (drvStatus == CY_USBFS_DEV_DRV_SUCCESS)
        {
            result = CY_RSLT_SUCCESS;
        }
    }
    
    return result;
}


cy_rslt_t cyhal_usb_dev_endpoint_abort(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint)
{
    /* This function is not completely working now for MBED. Tracked in MIDDLEWARE-990. */
    uint32_t  ep_num = CYHAL_USB_DEV_GET_EP_NUM(endpoint);

    /* Request abort operation. IMPORTANT: this updates MODE register to NACK. */
    cy_en_usb_dev_ep_state_t ep_state;
    ep_state = Cy_USBFS_Dev_Drv_Abort(obj->base, ep_num, &(obj->context));

    if (CY_USB_DEV_EP_PENDING ==ep_state)
    {
        (void) Cy_USBFS_Dev_Drv_AbortComplete(obj->base, ep_num, &(obj->context));
    }

    return CY_RSLT_SUCCESS;
}


cy_rslt_t cyhal_usb_dev_register_irq(cyhal_usb_dev_t *obj, cyhal_usb_dev_irq_handler_t handler)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;
    cy_stc_sysint_t config;
    uint32_t idx = (uint32_t)obj->resource.block_num;
    
    /* Configure NVIC parameters */
    config.intrPriority = CYHAL_USB_DEV_DEFAULT_IRQ_PRIORITY;
    config.intrSrc = CYHAL_USBDEV_IRQ_N[idx];

    /* Setup interrupt in NVIC to trigger the handler */
    if (CY_SYSINT_SUCCESS == Cy_SysInt_Init(&config, (cy_israddress)handler))
    {
        result = CY_RSLT_SUCCESS;
    }

    return result;
}


void cyhal_usb_dev_irq_enable(cyhal_usb_dev_t *obj, bool enable)
{
    uint32_t idx = (uint32_t)obj->resource.block_num;
    if (enable)
    {
        NVIC_ClearPendingIRQ(CYHAL_USBDEV_IRQ_N[idx]);
        NVIC_EnableIRQ(CYHAL_USBDEV_IRQ_N[idx]);
    }
    else
    {
        NVIC_DisableIRQ(CYHAL_USBDEV_IRQ_N[idx]);
    }
}


void cyhal_usb_dev_process_irq(cyhal_usb_dev_t *obj)
{
    Cy_USBFS_Dev_Drv_Interrupt(obj->base, 
                               Cy_USBFS_Dev_Drv_GetInterruptCauseLo(obj->base), 
                               &(obj->context));
}

static void cyhal_usb_0_dev_sof_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext)
{
    (void) drvContext;
    
    if (NULL != cyhal_usb_dev_sof_user_callback[0])
    {
        uint32_t frame_number = Cy_USBFS_Dev_Drv_GetSofNubmer(base);
        cyhal_usb_dev_sof_user_callback[0](frame_number);
    }
}


void cyhal_usb_dev_register_sof_callback( cyhal_usb_dev_t *obj, cyhal_usb_dev_sof_handler_t handler)
{
    uint32_t idx = (uint32_t)obj->resource.block_num;
    cyhal_usb_dev_sof_user_callback[idx] = handler;
}

#endif /* CY_IP_MXUSBFS) */
