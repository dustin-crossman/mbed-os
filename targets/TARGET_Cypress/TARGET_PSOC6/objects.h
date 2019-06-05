/***************************************************************************//**
* \file objects.h
*
* \brief
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "cyhal_hw_types.h"
#include "cyhal_uart.h"
#include "cyhal_flash.h"
#include "cyhal_i2c.h"
#include "cyhal_spi.h"
#include "PeripheralPins.h"
#include "gpio_object.h"

#ifdef __cplusplus
extern "C" {
#endif

#if DEVICE_ANALOGIN
struct analogin_s {
    cyhal_adc_t hal_adc;
};
#endif

#if DEVICE_ANALOGOUT
struct dac_s {
    cyhal_dac_t hal_dac;
    uint16_t max;
};
#endif

#if DEVICE_SERIAL
struct serial_s {
    cyhal_uart_t hal_obj;
    PinName pin_rx;
    PinName pin_tx;
    PinName pin_rts;
    PinName pin_cts;
    uint32_t baud;
    void *handler;
    uint32_t handler_arg;
    cyhal_uart_irq_event_t rx_event_mask;
    cyhal_uart_irq_event_t tx_event_mask;
#if DEVICE_SERIAL_ASYNCH
    cyhal_uart_irq_event_t event_flags;
    void *async_rx_handler;
    void *async_tx_handler;
#endif
};
#endif

#if DEVICE_FLASH
struct flash_s {
    cyhal_flash_t flash;
    cyhal_flash_info_t info;
};
#endif

#if DEVICE_PWMOUT
struct pwmout_s {
    cyhal_pwm_t hal_pwm;
    uint32_t period_us;
    uint32_t width_us;
};
#endif

#if DEVICE_TRNG
struct trng_s {
    cyhal_trng_t trng;
};
#endif

#if DEVICE_I2C
struct i2c_s {
    cyhal_i2c_t hal_i2c;
    cyhal_i2c_cfg_t cfg;
#ifdef DEVICE_I2C_ASYNCH
    void (*async_handler)(void);
    uint32_t async_event;
    size_t async_rx_size;
#endif
    uint8_t slave_event;
};
#endif

#if DEVICE_SPI
struct spi_s {
    cyhal_spi_t hal_spi;
    cyhal_spi_cfg_t cfg;
    void (*async_handler)(void);
    int async_events;
    int async_event_mask;
    cyhal_gpio_t mosi;
    cyhal_gpio_t miso;
    cyhal_gpio_t sclk;
    cyhal_gpio_t ssel;
    int hz;
};
#endif

#if DEVICE_QSPI
struct qspi_s {
    cyhal_qspi_t hal_qspi;
};
#endif

#ifdef __cplusplus
}
#endif


#endif /* CY_HAL_OBJECTS_H */
