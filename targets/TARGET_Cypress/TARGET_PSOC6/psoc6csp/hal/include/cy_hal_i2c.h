/***************************************************************************//**
* \file cy_hal_i2c.h
*
* \brief
* Provides a high level interface for interacting with the Cypress I2C. 
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cy_hal_implementation.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Failed to initial clock for i2c. */
#define CY_I2C_CLOCK_ERROR 1

/** The requested resource type is invalid */
#define CY_RSLT_ERR_CSP_I2C_INVALID_PIN (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_I2C, 0))

/** Enum to enable/disable/report interrupt cause flags. */
typedef enum
{
    CY_I2C_IRQ_NONE                = 0,      //!< Disable all interrupt call backs
    CY_I2C_IRQ_TX_TRANSMIT_IN_FIFO = 1 << 1, //!< All tx data from transmit has been moved to uart FIFO
    CY_I2C_IRQ_TX_DONE             = 1 << 2, //!< All tx data has been transmitted
    CY_I2C_IRQ_TX_ERROR            = 1 << 3, //!< An error occurred in tx
    CY_I2C_IRQ_RX_FULL             = 1 << 4, //!< The rx software buffer is full, additional data are store into fifo buffer.
    CY_I2C_IRQ_RX_DONE             = 1 << 5, //!< All rx data has been received
    CY_I2C_IRQ_RX_ERROR            = 1 << 6, //!< An error occurred in rx
} cy_i2c_irq_event_t;

/** I2C callback function type */
typedef void (*cy_i2c_irq_handler_t)(void *handler_arg, cy_i2c_irq_event_t event);

/** Handler for I2C interrupts */
typedef void (*cy_i2c_irq_handler)(void *handler_arg, cy_i2c_irq_event_t event);

/** Initial I2C configuration */
typedef struct
{
	bool is_slave; //!< I2C mode, is the device a master or slave
	uint16_t address; //!< Address of this device
	uint32_t frequency_hz; //!< Frequency that the I2C bus runs at
} cy_hal_i2c_cfg_t;

/** Initialize the I2C peripheral. It sets the default parameters for I2C
 *  peripheral, and configures its specifieds pins.
 *
 * @param[out] obj The I2C object
 * @param[in]  sda The sda pin
 * @param[in]  scl The scl pin
 * @param[in]  clk The clock to use can be shared, if not provided a new clock will be allocated
 * @return The status of the init request
 */
cy_rslt_t cy_i2c_init(cy_i2c_t *obj, cy_gpio_t sda, cy_gpio_t scl, const cy_clock_divider_t *clk);

/** Deinitialize the i2c object
 *
 * @param[in,out] obj The i2c object
 * @return The status of the free request
 */
cy_rslt_t cy_i2c_free(cy_i2c_t *obj);

/** Configure the I2C block
 *
 * @param[in] obj     The I2C object
 * @param[in] cfg     Configuration settings to apply
 * @return The status of the set_config request
 */
cy_rslt_t cy_i2c_set_config(cy_i2c_t *obj, const cy_hal_i2c_cfg_t *cfg);

/**
 * I2C master send
 *
 * @param[in]  obj       The I2C object
 * @param[in]  dev_addr  device address
 * @param[in]  data      i2c send data
 * @param[in]  size      i2c send data size
 * @param[in]  timeout   timeout in milisecond, set this value to 0 if you want to wait forever
 *
 * @return The status of the master_send request
 */
cy_rslt_t cy_i2c_master_send(cy_i2c_t *obj, uint16_t dev_addr, const uint8_t *data, uint16_t size, uint32_t timeout);

/**
 * I2C master recv
 *
 * @param[in]   obj       The I2C object
 * @param[in]   dev_addr  device address
 * @param[out]  data      i2c receive data
 * @param[in]   size      i2c receive data size
 * @param[in]   timeout   timeout in milisecond, set this value to 0 if you want to wait forever
 *
 * @return The status of the master_recv request
 */
cy_rslt_t cy_i2c_master_recv(cy_i2c_t *obj, uint16_t dev_addr, uint8_t *data, uint16_t size, uint32_t timeout);

/**
 * I2C slave send
 *
 * @param[in]  obj      The I2C object
 * @param[in]  data     i2c slave send data
 * @param[in]  size     i2c slave send data size
 * @param[in]  timeout  timeout in milisecond, set this value to 0 if you want to wait forever
 *
 * @return The status of the slave_send request
 */
cy_rslt_t cy_i2c_slave_send(cy_i2c_t *obj, const uint8_t *data, uint16_t size, uint32_t timeout);

/**
 * I2C slave receive
 *
 * @param[in]   obj      The I2C object
 * @param[out]  data     i2c slave receive data
 * @param[in]   size     i2c slave receive data size
 * @param[in]   timeout  timeout in milisecond, set this value to 0 if you want to wait forever
 *
 * @return The status of the slave_recv request
 */
cy_rslt_t cy_i2c_slave_recv(cy_i2c_t *obj, uint8_t *data, uint16_t size, uint32_t timeout);

/** Perform an i2c write using a block of data stored at the specified memory location
 *
 * @param[in]  obj            The I2C object
 * @param[in]  address        device address
 * @param[in]  mem_addr       mem address to store the written data
 * @param[in]  mem_addr_size  number of bytes in the mem address
 * @param[in]  data           i2c master send data
 * @param[in]  size           i2c master send data size
 * @param[in]  timeout        timeout in milisecond, set this value to 0 if you want to wait forever
 * @return The status of the write request
 */
cy_rslt_t cy_i2c_mem_write(cy_i2c_t *obj, uint16_t address, uint16_t mem_addr, uint16_t mem_addr_size, const uint8_t *data, uint16_t size, uint32_t timeout);

/** Perform an i2c read using a block of data stored at the specified memory location
 *
 * @param[in]  obj            The I2C object
 * @param[in]  address        device address
 * @param[in]  mem_addr       mem address to store the written data
 * @param[in]  mem_addr_size  number of bytes in the mem address
 * @param[out] data           i2c master send data
 * @param[in]  size           i2c master send data size
 * @param[in]  timeout        timeout in milisecond, set this value to 0 if you want to wait forever
 * @return The status of the read request
 */
cy_rslt_t cy_i2c_mem_read(cy_i2c_t *obj, uint16_t address, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *data, uint16_t size, uint32_t timeout);

/** Start I2C asynchronous transfer
 *
 * @param[in]  obj      The I2C object
 * @param[in]  tx       The transmit buffer
 * @param[in]  tx_size  The number of bytes to transmit
 * @param[out] rx       The receive buffer
 * @param[in]  rx_size  The number of bytes to receive
 * @param[in]  address  The address to be set - 7bit or 9bit
 * @return The status of the transfer_async request
 */
cy_rslt_t cy_i2c_transfer_async(cy_i2c_t *obj, const void *tx, size_t tx_size, void *rx, size_t rx_size, uint16_t address);

/** Abort asynchronous transfer
 *
 * This function does not perform any check - that should happen in upper layers.
 * @param[in] obj The I2C object
 * @return The status of the abort_async request
 */
cy_rslt_t cy_i2c_abort_async(cy_i2c_t *obj);

/** The I2C interrupt handler registration
 *
 * @param[in] obj         The I2C object
 * @param[in] handler     The callback handler which will be invoked when the interrupt fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 * @return The status of the register_irq request
 */
cy_rslt_t cy_i2c_register_irq(cy_i2c_t *obj, cy_i2c_irq_handler handler, void *handler_arg);

/** Configure I2C interrupt enablement.
 *
 * @param[in] obj      The I2C object
 * @param[in] event    The I2C IRQ type
 * @param[in] enable   True to turn on interrupts, False to turn off
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_i2c_irq_enable(cy_i2c_t *obj, cy_i2c_irq_event_t event, bool enable);

#if defined(__cplusplus)
}
#endif
