/***************************************************************************//**
* \file cybsp_cy8ckit_062s2_43012.c
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CKIT-062S2-43012 pioneer kit.
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


#include <stdlib.h>
#include "cycfg.h"
#include "cybsp_api_core.h"
#include "cyhal_implementation.h"

#if defined(__cplusplus)
extern "C" {
#endif

uint8_t cybsp_stdio_getchar(void);
void cybsp_stdio_putchar(char c);

cy_rslt_t cybsp_leds_on();
cy_rslt_t cybsp_leds_off();

/*******************************************************************************
*       Functions
*******************************************************************************/

cy_rslt_t cybsp_retarget_init()
{
	cyhal_gpio_t uart_tx_pin = UART_TX;
	cyhal_gpio_t uart_rx_pin = UART_RX;
	cyhal_gpio_t uart_cts_pin = NC;
	cyhal_gpio_t uart_rts_pin = NC;
	cyhal_clock_divider_t uart_clock_div;
	cyhal_uart_cfg_t uart_config;

	uart_clock_div.div_num = DEBUG_UART_CLK_DIV_NUM;
	uart_clock_div.div_type = DEBUG_UART_CLK_DIV_HW;

	uart_config.data_bits = DEBUG_UART_config.dataWidth;
	uart_config.parity = DEBUG_UART_config.parity;
	uart_config.rx_buffer = NULL;
	uart_config.stop_bits = DEBUG_UART_config.stopBits;
	/* Workaround: design.modus UART config is restricted to use oversampling = 12
	 * as UART HAL doesn't have a oversamplinmg parameter, and uses default = 12 (See JIRA BSP-654) */

	cy_rslt_t result = cyhal_uart_init(&cybsp_debug_uart, uart_tx_pin, uart_rx_pin, &uart_clock_div, &uart_config);

	/* Workaround as cyhal_uart_init doesn't enable the UART block*/
	Cy_SCB_UART_Enable(DEBUG_UART_HW);


	if (result != CY_RSLT_SUCCESS)
	{
		return result;
	}

	if ((uart_cts_pin != NC) && (uart_rts_pin != NC))
	{
		result = cyhal_uart_set_flow_control(&cybsp_debug_uart, uart_cts_pin, uart_cts_pin);
	}

	return result;
}


cy_rslt_t cybsp_init(void)
{
	cy_rslt_t result;

	init_cycfg_all();

    /* Initialize retargetting stdio to 'DEBUG_UART' peripheral */
	result = cybsp_retarget_init();

    /* enable interrupts */
	__enable_irq();

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cybsp_led_init(cybsp_led_t which)
{
    return cyhal_gpio_init((cyhal_gpio_t)which, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
}

void cybsp_led_set_state(cybsp_led_t which, bool on)
{
    cyhal_gpio_write((cyhal_gpio_t)which, on);
}

void cybsp_led_toggle(cybsp_led_t which)
{
    cyhal_gpio_toggle((cyhal_gpio_t)which);
}

cy_rslt_t cybsp_leds_on()
{
	cy_rslt_t result;
	result = cybsp_led_set_state(CYBSP_USER_LED1, CYBSP_LED_STATE_ON);
	result += cybsp_led_set_state(CYBSP_USER_LED2, CYBSP_LED_STATE_ON);
	result += cybsp_led_set_state(CYBSP_USER_LED3, CYBSP_LED_STATE_ON);
	result += cybsp_led_set_state(CYBSP_USER_LED4, CYBSP_LED_STATE_ON);
	result += cybsp_led_set_state(CYBSP_USER_LED5, CYBSP_LED_STATE_ON);
    return result;
}

cy_rslt_t cybsp_leds_off()
{
	cy_rslt_t result;
	result = cybsp_led_set_state(CYBSP_USER_LED1, CYBSP_LED_STATE_OFF);
	result += cybsp_led_set_state(CYBSP_USER_LED2, CYBSP_LED_STATE_OFF);
	result += cybsp_led_set_state(CYBSP_USER_LED3, CYBSP_LED_STATE_OFF);
	result += cybsp_led_set_state(CYBSP_USER_LED4, CYBSP_LED_STATE_OFF);
	result += cybsp_led_set_state(CYBSP_USER_LED5, CYBSP_LED_STATE_OFF);
    return result;
}

cy_rslt_t cybsp_btn_init(cybsp_btn_t which)
{
    return cyhal_gpio_init((cyhal_gpio_t)which, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_OFF);
}

bool cybsp_btn_get_state(cybsp_btn_t which)
{
    return cyhal_gpio_read((cyhal_gpio_t)which);
}

static void (*btn_interrupt_call_back) (void);
static void gpio_call_back_wrapper(void *handler_arg, cyhal_gpio_irq_event_t event)
{
    if (btn_interrupt_call_back != NULL)
    {
        btn_interrupt_call_back();
    }
}

void cybsp_btn_set_interrupt(cybsp_btn_t which, cyhal_gpio_irq_event_t type, void (*callback)(void))
{
    btn_interrupt_call_back = callback;
    cyhal_gpio_register_irq((cyhal_gpio_t)which, 7, &gpio_call_back_wrapper, NULL);
    cyhal_gpio_irq_enable((cyhal_gpio_t)which, type, 1);
}

#if defined(__cplusplus)
}
#endif
