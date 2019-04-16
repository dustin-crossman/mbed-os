/*******************************************************************************
* File Name: cy_hal_uart.c
*
* Description:
* Provides a high level interface for interacting with the Cypress UART. This is
* a wrapper around the lower level PDL API.
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include <stdlib.h>
#include "cy_hal_uart.h"
#include "cy_hal_hwmgr.h"
#include "cy_hal_gpio.h"
#include "cy_hal_interconnect.h"
#include "cy_scb_uart.h"
#include "cy_hal_scb_common.h"

#define UART_OVERSAMPLE                 12

/* Default UART configuration */
static const cy_stc_scb_uart_config_t default_uart_config = {
    .uartMode                   = CY_SCB_UART_STANDARD,
    .enableMutliProcessorMode   = false,
    .smartCardRetryOnNack       = false,
    .irdaInvertRx               = false,
    .irdaEnableLowPowerReceiver = false,

    .oversample                 = UART_OVERSAMPLE,

    .enableMsbFirst             = false,
    .dataWidth                  = 8UL,
    .parity                     = CY_SCB_UART_PARITY_NONE,
    .stopBits                   = CY_SCB_UART_STOP_BITS_1,
    .enableInputFilter          = false,
    .breakWidth                 = 11UL,
    .dropOnFrameError           = false,
    .dropOnParityError          = false,

    .receiverAddress            = 0x0UL,
    .receiverAddressMask        = 0x0UL,
    .acceptAddrInFifo           = false,

    .enableCts                  = false,
    .ctsPolarity                = CY_SCB_UART_ACTIVE_LOW,
    .rtsRxFifoLevel             = 20UL,
    .rtsPolarity                = CY_SCB_UART_ACTIVE_LOW,

    .rxFifoTriggerLevel         = 0UL,  /* Level triggers when at least one element is in FIFO */
    .rxFifoIntEnableMask        = 0x0UL,

    .txFifoTriggerLevel         = 63UL, /* Level triggers when half-fifo is half empty */
    .txFifoIntEnableMask        = 0x0UL
};

static cy_uart_irq_event_t cy_convert_interrupt_cause(uint32_t pdl_cause);

static cy_uart_t *cy_uart_config_structs[CY_IP_MXSCB_INSTANCES];
static cy_uart_irq_handler_t cy_uart_user_callbacks[CY_IP_MXSCB_INSTANCES];
static void *cy_uart_callback_args[CY_IP_MXSCB_INSTANCES];

static void cy_uart_0_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_1_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_2_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_3_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_4_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_5_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_6_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_7_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_8_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_9_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_10_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_11_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_12_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_13_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_14_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_15_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_16_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_17_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_18_cb_wrapper(uint32_t event) __attribute__((unused));
static void cy_uart_19_cb_wrapper(uint32_t event) __attribute__((unused));

static void cy_uart_0_irq_handler(void) __attribute__((unused));
static void cy_uart_1_irq_handler(void) __attribute__((unused));
static void cy_uart_2_irq_handler(void) __attribute__((unused));
static void cy_uart_3_irq_handler(void) __attribute__((unused));
static void cy_uart_4_irq_handler(void) __attribute__((unused));
static void cy_uart_5_irq_handler(void) __attribute__((unused));
static void cy_uart_6_irq_handler(void) __attribute__((unused));
static void cy_uart_7_irq_handler(void) __attribute__((unused));
static void cy_uart_8_irq_handler(void) __attribute__((unused));
static void cy_uart_9_irq_handler(void) __attribute__((unused));
static void cy_uart_10_irq_handler(void) __attribute__((unused));
static void cy_uart_11_irq_handler(void) __attribute__((unused));
static void cy_uart_12_irq_handler(void) __attribute__((unused));
static void cy_uart_13_irq_handler(void) __attribute__((unused));
static void cy_uart_14_irq_handler(void) __attribute__((unused));
static void cy_uart_15_irq_handler(void) __attribute__((unused));
static void cy_uart_16_irq_handler(void) __attribute__((unused));
static void cy_uart_17_irq_handler(void) __attribute__((unused));
static void cy_uart_18_irq_handler(void) __attribute__((unused));
static void cy_uart_19_irq_handler(void) __attribute__((unused));

static __INLINE  void cy_uart_cb_wrapper_indexed(uint32_t event, uint8_t idx)
{
    if (NULL != cy_uart_user_callbacks[idx])
    {
        cy_uart_irq_event_t anded_events = (cy_uart_irq_event_t)(cy_uart_config_structs[idx]->irq_cause & (uint32_t)cy_convert_interrupt_cause(event));
        if (anded_events)
        {
            cy_uart_user_callbacks[idx](cy_uart_callback_args[idx], anded_events);
        }
    }
}
static void cy_uart_0_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 0);
}
static void cy_uart_1_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 1);
}
static void cy_uart_2_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 2);
}
static void cy_uart_3_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 3);
}
static void cy_uart_4_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 4);
}
static void cy_uart_5_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 5);
}
static void cy_uart_6_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 6);
}
static void cy_uart_7_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 7);
}
static void cy_uart_8_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 8);
}
static void cy_uart_9_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 9);
}
static void cy_uart_10_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 10);
}
static void cy_uart_11_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 11);
}
static void cy_uart_12_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 12);
}
static void cy_uart_13_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 13);
}
static void cy_uart_14_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 14);
}
static void cy_uart_15_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 15);
}
static void cy_uart_16_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 16);
}
static void cy_uart_17_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 17);
}
static void cy_uart_18_cb_wrapper(uint32_t event)
{
    cy_uart_cb_wrapper_indexed(event, 18);
}
static void cy_uart_19_cb_wrapper(uint32_t event) 
{
    cy_uart_cb_wrapper_indexed(event, 19);
}

static void (*cy_uart_cb_wrapper_table[CY_IP_MXSCB_INSTANCES])(uint32_t event) =
{
#if (CY_IP_MXSCB_INSTANCES > 0)
    cy_uart_0_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 1)
    cy_uart_1_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 2)
    cy_uart_2_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 3)
    cy_uart_3_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 4)
    cy_uart_4_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 5)
    cy_uart_5_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 6)
    cy_uart_6_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 7)
    cy_uart_7_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 8)
    cy_uart_8_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 9)
    cy_uart_9_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 10)
    cy_uart_10_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 11)
    cy_uart_11_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 12)
    cy_uart_12_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 13)
    cy_uart_13_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 14)
    cy_uart_14_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 15)
    cy_uart_15_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 16)
    cy_uart_16_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 17)
    cy_uart_17_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 18)
    cy_uart_18_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 19)
    cy_uart_19_cb_wrapper,
#endif
#if (CY_IP_MXSCB_INSTANCES > 20)
    #error "Unhandled scb count"
#endif
};

static void cy_uart_interrupts_dispatcher_IRQHandler(uint32_t uart_num)
{
    Cy_SCB_UART_Interrupt(cy_uart_config_structs[uart_num]->base, &(cy_uart_config_structs[uart_num]->context));
}
static void cy_uart_0_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(0);
}
static void cy_uart_1_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(1);
}
static void cy_uart_2_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(2);
}
static void cy_uart_3_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(3);
}
static void cy_uart_4_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(4);
}
static void cy_uart_5_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(5);
}
static void cy_uart_6_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(6);
}
static void cy_uart_7_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(7);
}
static void cy_uart_8_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(8);
}
static void cy_uart_9_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(9);
}
static void cy_uart_10_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(10);
}
static void cy_uart_11_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(11);
}
static void cy_uart_12_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(12);
}
static void cy_uart_13_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(13);
}
static void cy_uart_14_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(14);
}
static void cy_uart_15_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(15);
}
static void cy_uart_16_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(16);
}
static void cy_uart_17_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(17);
}
static void cy_uart_18_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(18);
}
static void cy_uart_19_irq_handler(void)
{
    cy_uart_interrupts_dispatcher_IRQHandler(19);
}
static void (*cy_uart_interrupts_dispatcher_table[CY_IP_MXSCB_INSTANCES])(void) = 
{
#if (CY_IP_MXSCB_INSTANCES > 0)
    cy_uart_0_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 1)
    cy_uart_1_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 2)
    cy_uart_2_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 3)
    cy_uart_3_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 4)
    cy_uart_4_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 5)
    cy_uart_5_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 6)
    cy_uart_6_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 7)
    cy_uart_7_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 8)
    cy_uart_8_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 9)
    cy_uart_9_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 10)
    cy_uart_10_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 11)
    cy_uart_11_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 12)
    cy_uart_12_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 13)
    cy_uart_13_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 14)
    cy_uart_14_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 15)
    cy_uart_15_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 16)
    cy_uart_16_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 17)
    cy_uart_17_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 18)
    cy_uart_18_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 19)
    cy_uart_19_irq_handler,
#endif
#if (CY_IP_MXSCB_INSTANCES > 20)
    #error "Unhandled scb count"
#endif
};

static cy_rslt_t free_resources(cy_uart_t *obj)
{
    cy_rslt_t error_accum = CY_RSLT_SUCCESS;
    cy_rslt_t error;
    if (obj->resource.type != CY_RSC_INVALID)
    {
        error = cy_hwmgr_free(&(obj->resource));
        if (error != CY_RSLT_SUCCESS && error_accum == CY_RSLT_SUCCESS)
        {
            error_accum = error;
        }
    }
    if (obj->pin_rx != NC)
    {
        error = cy_disconnect_pin(obj->pin_rx);
        if (error == CY_RSLT_SUCCESS)
        {
			cy_resource_inst_t rsc = cy_utils_get_gpio_resource(obj->pin_rx);
            error = cy_hwmgr_free(&rsc);
        }
        if (error != CY_RSLT_SUCCESS && error_accum == CY_RSLT_SUCCESS)
        {
            error_accum = error;
        }
    }
    if (obj->pin_tx != NC)
    {
        error = cy_disconnect_pin(obj->pin_tx);
        if (error == CY_RSLT_SUCCESS)
        {
			cy_resource_inst_t rsc = cy_utils_get_gpio_resource(obj->pin_tx);
            error = cy_hwmgr_free(&rsc);
        }
        if (error != CY_RSLT_SUCCESS && error_accum == CY_RSLT_SUCCESS)
        {
            error_accum = error;
        }
    }
    if (obj->pin_rts != NC)
    {
        error = cy_disconnect_pin(obj->pin_rts);
        if (error == CY_RSLT_SUCCESS)
        {
			cy_resource_inst_t rsc = cy_utils_get_gpio_resource(obj->pin_rts);
            error = cy_hwmgr_free(&rsc);
        }
        if (error != CY_RSLT_SUCCESS && error_accum == CY_RSLT_SUCCESS)
        {
            error_accum = error;
        }
    }
    if (obj->pin_cts != NC)
    {
        error = cy_disconnect_pin(obj->pin_cts);
        if (error == CY_RSLT_SUCCESS)
        {
			cy_resource_inst_t rsc = cy_utils_get_gpio_resource(obj->pin_cts);
            error = cy_hwmgr_free(&rsc);
        }
        if (error != CY_RSLT_SUCCESS && error_accum == CY_RSLT_SUCCESS)
        {
            error_accum = error;
        }
    }
    return error_accum;
}

static cy_en_scb_uart_parity_t convert_parity(cy_uart_parity_t parity)
{
    switch (parity)
    {
        case CY_UART_PARITY_NONE:
            return CY_SCB_UART_PARITY_NONE;
        case CY_UART_PARITY_EVEN:
            return CY_SCB_UART_PARITY_EVEN;
        case CY_UART_PARITY_ODD:
            return CY_SCB_UART_PARITY_ODD;
        default:
            return CY_SCB_UART_PARITY_NONE;
    }
}

static cy_en_scb_uart_stop_bits_t convert_stopbits(uint8_t stopbits)
{
    switch (stopbits)
    {
        case 1:
            return CY_SCB_UART_STOP_BITS_1;
        case 2:
            return CY_SCB_UART_STOP_BITS_2;
        case 3:
            return CY_SCB_UART_STOP_BITS_3;
        case 4:
            return CY_SCB_UART_STOP_BITS_4;
        default:
            CY_ASSERT(false);
            return CY_SCB_UART_STOP_BITS_1;
    }
}

cy_rslt_t cy_uart_init(cy_uart_t *obj, cy_gpio_t tx, cy_gpio_t rx, const cy_clock_divider_t *clk, const cy_hal_uart_cfg_t *cfg)
{
    // If something go wrong, any resource not marked as invalid will be freed.
    // Explicitly marked not allocated resources as invalid to prevent freeing them.
    memset(obj, 0, sizeof(cy_uart_t));
	cy_resource_inst_t pin_rsc;
    
    // Reserve the UART
    const cy_resource_pin_mapping_t *tx_map = CY_UTILS_GET_RESOURCE(tx, cy_pin_map_scb_uart_tx);
    const cy_resource_pin_mapping_t *rx_map = CY_UTILS_GET_RESOURCE(rx, cy_pin_map_scb_uart_rx);
    if (NULL == tx_map || NULL == rx_map || tx_map->inst->block_num != rx_map->inst->block_num)
    {
        return CY_RSLT_ERR_CSP_UART_INVALID_PIN;
    }

    obj->resource = *rx_map->inst;
    
    // reserve the TX pin
	pin_rsc = cy_utils_get_gpio_resource(tx);
	cy_rslt_t result = cy_hwmgr_reserve(&pin_rsc);
    if (result == CY_RSLT_SUCCESS)
    {
        obj->pin_tx = tx;
    }
    
    //reseve the RX pin
    if (result == CY_RSLT_SUCCESS)
    {
		pin_rsc = cy_utils_get_gpio_resource(rx);
		cy_rslt_t result = cy_hwmgr_reserve(&pin_rsc);
		if (result == CY_RSLT_SUCCESS)
		{
			obj->pin_rx = rx;
		}
	}
    
    obj->base = CY_SCB_BASE_ADDRESSES[obj->resource.block_num];
    
    if (result == CY_RSLT_SUCCESS)
    {
        if (clk == NULL)
        {
            result = cy_hwmgr_allocate_clock(&(obj->clock), CY_SYSCLK_DIV_8_BIT, true);
        }
        else
        {
            obj->clock = *clk;
        }
    }
	
    if (result == CY_RSLT_SUCCESS)
    {
        cy_en_sysclk_status_t clock_assign_result = Cy_SysClk_PeriphAssignDivider(
                (en_clk_dst_t)((uint8_t)PCLK_SCB0_CLOCK + obj->resource.block_num), obj->clock.div_type, obj->clock.div_num);
        result = clock_assign_result == CY_SYSCLK_SUCCESS
            ? CY_RSLT_SUCCESS
            : CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_UART, CY_UART_CLOCK_ERROR);
    }    
    if (result == CY_RSLT_SUCCESS)
    {
        result = cy_connect_pin(rx_map);
    }
    if (result == CY_RSLT_SUCCESS)
    {
        result = cy_connect_pin(tx_map);
    }

    bool configured = false;
    if (result == CY_RSLT_SUCCESS)
    {
        result = cy_hwmgr_is_configured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num, &configured);
    }
    
    if (result == CY_RSLT_SUCCESS && !configured)
    {
        if (cfg == NULL)
        {
            Cy_SCB_UART_Init(obj->base, &default_uart_config, &(obj->context));
        }
        else 
        {
            cy_stc_scb_uart_config_t config_structure = default_uart_config;
            config_structure.dataWidth = cfg->data_bits;
            config_structure.stopBits = convert_stopbits(cfg->stop_bits);
            config_structure.parity = convert_parity(cfg->parity);
            Cy_SCB_UART_Init(obj->base, &config_structure, &(obj->context));
            if (cfg->rx_buffer != NULL)
            {
                Cy_SCB_UART_StartRingBuffer(obj->base, cfg->rx_buffer, cfg->rx_buffer_size, &(obj->context));
            }
        }        
        cy_hwmgr_set_configured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
    }
    if (result != CY_RSLT_SUCCESS)
    {
        free_resources(obj);
    }
    return result;
}

cy_rslt_t cy_uart_free(cy_uart_t *obj)
{
    cy_rslt_t error_accum = CY_RSLT_SUCCESS;
    cy_rslt_t error = free_resources(obj);
    if (error != CY_RSLT_SUCCESS && error_accum == CY_RSLT_SUCCESS)
    {
        error_accum = error;
    }
    error = cy_hwmgr_set_unconfigured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
    if (error != CY_RSLT_SUCCESS && error_accum == CY_RSLT_SUCCESS)
    {
        error_accum = error;
    }
    return error_accum;
}

#define FRACT_DIV_INT(divider)      (((divider) >> 5U) - 1U)
#define FRACT_DIV_FARCT(divider)    ((divider) & 0x1FU)

static uint32_t cy_divider_value(uint32_t frequency, uint32_t frac_bits)
{
    /* UARTs use peripheral clock */
    return ((cy_PeriClkFreqHz * (1 << frac_bits)) + (frequency / 2)) / frequency;
}

cy_rslt_t cy_uart_baud(cy_uart_t *obj, uint32_t baudrate)
{
    Cy_SCB_UART_Disable(obj->base, NULL);
    cy_en_sysclk_status_t status;

    Cy_SysClk_PeriphDisableDivider(obj->clock.div_type, obj->clock.div_num);

    /* Set baud rate */
    if ((obj->clock.div_type == CY_SYSCLK_DIV_16_5_BIT) || (obj->clock.div_type == CY_SYSCLK_DIV_24_5_BIT)) {
        /* Get fractional divider */
        uint32_t divider = cy_divider_value(baudrate * UART_OVERSAMPLE, 5U);

        status = Cy_SysClk_PeriphSetFracDivider(obj->clock.div_type,
                                                obj->clock.div_num,
                                                FRACT_DIV_INT(divider),
                                                FRACT_DIV_FARCT(divider));
    } else {
        /* Get integer divider */
        status = Cy_SysClk_PeriphSetDivider(obj->clock.div_type,
                                            obj->clock.div_num,
                                            cy_divider_value(baudrate * UART_OVERSAMPLE, 0));
    }

    Cy_SysClk_PeriphEnableDivider(obj->clock.div_type, obj->clock.div_num);

    Cy_SCB_UART_Enable(obj->base);
    return status;
}

cy_rslt_t cy_uart_getc(cy_uart_t *obj, uint8_t *value)
{
    uint32_t read_value = CY_SCB_UART_RX_NO_DATA;
    while (read_value == CY_SCB_UART_RX_NO_DATA)
    {
        read_value = Cy_SCB_UART_Get(obj->base);
    }
    *value = (uint8_t)read_value;
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_uart_putc(cy_uart_t *obj, uint32_t value)
{
    uint32_t count = 0;
    while (count == 0)
    {
        count = Cy_SCB_UART_Put(obj->base, value);
    }
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_uart_is_readable(cy_uart_t *obj, bool *readable)
{
    uint32_t read_value = Cy_SCB_UART_GetNumInRxFifo(obj->base);
    *readable = read_value != CY_SCB_UART_RX_NO_DATA;
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_uart_is_writable(cy_uart_t *obj, bool *writable)
{
    uint32_t count = Cy_SCB_UART_GetNumInTxFifo(obj->base);
    *writable = count < CY_SCB_BUFFER_SIZE[obj->resource.block_num];
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_uart_clear(cy_uart_t *obj)
{
    Cy_SCB_UART_ClearRxFifo(obj->base);
    Cy_SCB_UART_ClearTxFifo(obj->base);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_uart_set_flow_control(cy_uart_t *obj, cy_gpio_t cts, cy_gpio_t rts)
{
    cy_rslt_t result;
	cy_resource_inst_t pin_rsc;
    
	if (cts != obj->pin_cts)
	{
		if (NC == cts)
		{
			result = cy_disconnect_pin(cts);
			
			if (CY_RSLT_SUCCESS == result)
			{
				Cy_SCB_UART_DisableCts(obj->base);
				
				pin_rsc = cy_utils_get_gpio_resource(cts);
				result = cy_hwmgr_free(&pin_rsc);
			}
		}
		else
		{
			const cy_resource_pin_mapping_t *cts_map = CY_UTILS_GET_RESOURCE(cts, cy_pin_map_scb_uart_cts);
			if (obj->resource.block_num != cts_map->inst->block_num ||
				obj->resource.channel_num != cts_map->inst->channel_num)
			{
				return CY_RSLT_ERR_CSP_UART_INVALID_PIN;
			}
			
			pin_rsc = cy_utils_get_gpio_resource(cts);
			result = cy_hwmgr_reserve(&pin_rsc);
			
			if (CY_RSLT_SUCCESS == result)
			{
				Cy_SCB_UART_EnableCts(obj->base);
				result = cy_connect_pin(cts_map);
			}
		}
		
		if (result != CY_RSLT_SUCCESS)
		{
			return result;
		}
		obj->pin_cts = cts;
	}
	if (rts != obj->pin_rts)
	{
		if (NC == rts)
		{
			result = cy_disconnect_pin(rts);
			
			if (CY_RSLT_SUCCESS == result)
			{
				pin_rsc = cy_utils_get_gpio_resource(rts);
				result = cy_hwmgr_free(&pin_rsc);
			}
		}
		else
		{
			const cy_resource_pin_mapping_t *rts_map = CY_UTILS_GET_RESOURCE(rts, cy_pin_map_scb_uart_rts);
			if (obj->resource.block_num != rts_map->inst->block_num ||
				obj->resource.channel_num != rts_map->inst->channel_num)
			{
				return CY_RSLT_ERR_CSP_UART_INVALID_PIN;
			}
			
			pin_rsc = cy_utils_get_gpio_resource(rts);
			result = cy_hwmgr_reserve(&pin_rsc);
			
			if (CY_RSLT_SUCCESS == result)
			{
				result = cy_connect_pin(rts_map);
			}
		}
		
		if (result != CY_RSLT_SUCCESS)
		{
			return result;
		}
		obj->pin_rts = rts;
	}
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_uart_tx(cy_uart_t *obj, void *tx, size_t *tx_length)
{
    *tx_length = Cy_SCB_UART_PutArray(obj->base, tx, *tx_length);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_uart_rx(cy_uart_t *obj, void *rx, size_t *rx_length)
{
    *rx_length = Cy_SCB_UART_GetArray(obj->base, rx, *rx_length);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_uart_tx_asynch(cy_uart_t *obj, void *tx, size_t length)
{
    cy_en_scb_uart_status_t uart_status = Cy_SCB_UART_Transmit(obj->base, tx, length, &(obj->context));
    return uart_status == CY_SCB_UART_SUCCESS
        ? CY_RSLT_SUCCESS
        : CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_UART, 0);
}

cy_rslt_t cy_uart_rx_asynch(cy_uart_t *obj, void *rx, size_t length)
{
    cy_en_scb_uart_status_t uart_status = Cy_SCB_UART_Receive(obj->base, rx, length, &(obj->context));
    return uart_status == CY_SCB_UART_SUCCESS
        ? CY_RSLT_SUCCESS
        : CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_UART, 0);
}

cy_rslt_t cy_uart_is_tx_active(cy_uart_t *obj, bool *active)
{
    *active = 0UL != (obj->context.txStatus & CY_SCB_UART_TRANSMIT_ACTIVE);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_uart_is_rx_active(cy_uart_t *obj, bool *active)
{
    *active = 0UL != (obj->context.rxStatus & CY_SCB_UART_RECEIVE_ACTIVE);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_uart_tx_abort(cy_uart_t *obj)
{
    Cy_SCB_UART_AbortTransmit(obj->base, &(obj->context));
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_uart_rx_abort(cy_uart_t *obj)
{
    Cy_SCB_UART_AbortReceive(obj->base, &(obj->context));
    return CY_RSLT_SUCCESS;
}

static cy_uart_irq_event_t cy_convert_interrupt_cause(uint32_t pdl_cause)
{    
    cy_uart_irq_event_t cause = CY_UART_IRQ_NONE;
    if (pdl_cause == CY_SCB_UART_TRANSMIT_IN_FIFO_EVENT)
    {
        cause = CY_UART_IRQ_TX_TRANSMIT_IN_FIFO;
    }
    else if (pdl_cause == CY_SCB_UART_TRANSMIT_DONE_EVENT)
    {
        cause = CY_UART_IRQ_TX_DONE;
    }
    else if (pdl_cause == CY_SCB_UART_RECEIVE_DONE_EVENT)
    {
        cause = CY_UART_IRQ_RX_DONE;
    }
    else if (pdl_cause == CY_SCB_UART_RB_FULL_EVENT)
    {
        cause = CY_UART_IRQ_RX_FULL;
    }
    else if (pdl_cause == CY_SCB_UART_RECEIVE_ERR_EVENT)
    {
        cause = CY_UART_IRQ_RX_ERROR;
    }
    else if (pdl_cause == CY_SCB_UART_TRANSMIT_ERR_EVENT)
    {
        cause = CY_UART_IRQ_TX_ERROR;
    }
    return cause;
}

cy_rslt_t cy_uart_register_irq(cy_uart_t *obj, cy_uart_irq_handler_t handler, void *handler_arg)
{
    uint8_t idx = obj->resource.block_num;
    cy_uart_config_structs[idx] = obj;
    cy_uart_user_callbacks[idx] = handler;
    cy_uart_callback_args[idx] = handler_arg;
    Cy_SCB_UART_RegisterCallback(obj->base, cy_uart_cb_wrapper_table[idx], &(obj->context));
    if (NVIC_GetEnableIRQ(CY_SCB_IRQ_N[idx]) == 0)
    {
        // default interrupt priority of 7 (lowest possible priority).
        cy_stc_sysint_t irqCfg = {CY_SCB_IRQ_N[idx], 7u};

        Cy_SysInt_Init(&irqCfg, cy_uart_interrupts_dispatcher_table[idx]);
        NVIC_EnableIRQ(CY_SCB_IRQ_N[idx]);
    }
    
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_uart_irq_enable(cy_uart_t *obj, cy_uart_irq_event_t event, bool enable)
{
    if (enable)
    {
        obj->irq_cause |= event;
    }
    else
    {
        obj->irq_cause &= ~event;
    }
    return CY_RSLT_SUCCESS;
}
