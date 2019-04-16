/***************************************************************************//**
* \file cy_hal_sdhc.h
*
* \brief
* Provides a high level interface for interacting with the Cypress SDHC. 
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

/** Transfer mode for bulk transfer API */
typedef enum
{
    CY_SDIO_BLOCK_MODE = ( 0 << 2 ),
    CY_SDIO_BYTE_MODE  = ( 1 << 2 )
} cy_sdio_transfer_mode_t;

/** SDHC interrupt triggers */
typedef enum {
    CY_SDHC_CMD_COMPLETE, //!> Command Complete
    CY_SDHC_XFER_COMPLETE, //!> Host read/write transfer is complete
    CY_SDHC_BGAP_EVENT, //!> This bit is set when both read/write transaction is stopped
    CY_SDHC_DMA_INTERRUPT, //!> Host controller detects an SDMA Buffer Boundary during transfer
    CY_SDHC_BUF_WR_READY, //!> This bit is set if the Buffer Write Enable changes from 0 to 1
    CY_SDHC_BUF_RD_READY, //!> This bit is set if the Buffer Read Enable changes from 0 to 1
    CY_SDHC_CARD_INSERTION, //!> This bit is set if the Card Inserted in the Present State
    CY_SDHC_CARD_REMOVAL, //!> This bit is set if the Card Inserted in the Present State
    CY_SDHC_CARD_INTERRUPT, //!> The synchronized value of the DAT[1] interrupt input for SD mode
    CY_SDHC_INT_A,
    CY_SDHC_INT_B,
    CY_SDHC_INT_C,
    CY_SDHC_RE_TUNE_EVENT, //!> This bit is set if the Re-Tuning Request changes from 0 to 1
    CY_SDHC_FX_EVENT, //!> This status is set when R[14] of response register is set to 1
    CY_SDHC_CQE_EVENT, //!> This status is set if Command Queuing/Crypto event has occurred
    CY_SDHC_ERR_INTERRUPT, //!> If any of the bits in the Error Interrupt Status register are set
    CY_SDHC_ALL_INTERRUPTS, //!> Is used to enable/disable all interrupts
} cy_sdhc_irq_event_t;

/** Handler for SDHC interrupts */
typedef void (*cy_sdhc_irq_handler)(void *handler_arg, cy_sdhc_irq_event_t event);

/** Card types */
typedef enum 
{
    CY_SDHC_SD, //!< Secure Digital card
    CY_SDHC_SDIO, //!< CD Input Output card 
    CY_SDHC_EMMC, //!< Embedded Multimedia card
    CY_SDHC_COMBO, //!< Combo Card (SD + SDIO)
    CY_SDHC_UNUSABLE, //!< Unusable card or unsupported type
    CY_SDHC_NOT_EMMC, //!< Not an eMMC card
} cy_sdhc_card_type_t;

/** Defines configuration options for the SDHC block */
typedef struct
{
    bool                 enableLedControl; //!< Drive one IO to indicate when the card is being accessed
    bool                 lowVoltageSignaling; //!< Whether 1.8V signaling is supported
    uint8_t              busWidth; //!< The desired bus width
} cy_sdhc_config_t;

/** Initialize the SDHC peripheral
 *
 * @param[out] obj               The SDHC object
 * @param[out] clk               The pin connected to the clk signal
 * @param[in]  cmd               The pin connected to the command signal
 * @param[in]  data0             The pin connected to the data0 signal
 * @param[in]  data1             The pin connected to the data1 signal
 * @param[in]  data2             The pin connected to the data2 signal
 * @param[in]  data3             The pin connected to the data3 signal
 * @param[in]  data4             The pin connected to the data4 signal
 * @param[in]  data5             The pin connected to the data5 signal
 * @param[in]  data6             The pin connected to the data6 signal
 * @param[in]  data7             The pin connected to the data7 signal
 * @param[in]  cardDetect        The pin connected to the cardDetect signal
 * @param[out] ioVoltSel         The pin connected to the ioVoltSel signal
 * @param[out] cardIfPwrEn       The pin connected to the cardIfPwrEn signal
 * @param[in]  cardMechWriteProt The pin connected to the cardMechWriteProt signal
 * @param[in]  ledCtrl           The pin connected to the ledCtrl signal
 * @param[in]  cardEmmcReset     The pin connected to the cardEmmcReset signal
 * @return The status of the init request
 */
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
                   cy_gpio_t cardEmmcReset);

/** Release the SDHC peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param[in,out] obj The SDHC object
 * @return The status of the free request
 */
cy_rslt_t cy_sdhc_free(cy_sdhc_t *obj);

/** Configure the SDHC block.
 *
 * @param[in,out] obj    The SDHC object
 * @param[in]     config The card configuration object
 * @return The status of the configure request
 */
cy_rslt_t cy_sdhc_configure(cy_sdhc_t *obj, const cy_sdhc_config_t *config);

/** Attempts to read data over the SDHC peripheral.
 *
 * @param[in]     obj     The SDHC object
 * @param[in]     address The address to read data from
 * @param[out]    data    Pointer to the byte-array of data to read from the device
 * @param[in,out] length  Number of bytes to read, updated with the number actually read
 * @return The status of the read request
 */
cy_rslt_t cy_sdhc_read(const cy_sdhc_t *obj, uint32_t address, uint8_t *data, size_t *length);

/** Attempts to write data over SDHC peripheral
 *
 * @param[in]     obj     The SDHC object
 * @param[in]     address The address to write data to
 * @param[in]     data    Pointer to the byte-array of data to write to the device
 * @param[in,out] length  Number of bytes to read, updated with the number actually read
 * @return The status of the write request
 */
cy_rslt_t cy_sdhc_write(const cy_sdhc_t *obj, uint32_t address, const uint8_t *data, size_t *length);

/** Attempts to erase a block of data over the SDHC peripheral
 *
 * @param[in] obj       The SDHC object
 * @param[in] startAddr Is the address of the first byte to erase
 * @param[in] length    The number of bytes (starting at startAddr) to erase
 * @return The status of the erase request
 */
cy_rslt_t cy_sdhc_erase(const cy_sdhc_t *obj, uint32_t startAddr, size_t length);

/** Begin the SDHC read
 *
 * @param[in]     obj     The SDHC object that holds the transfer information
 * @param[in]     address The address to read data from
 * @param[out]    data    The receive buffer
 * @param[in,out] length  Number of bytes to read, updated with the number actually read
 * @return The status of the read_async request
 */
cy_rslt_t cy_sdhc_read_async(const cy_sdhc_t *obj, uint32_t address, uint8_t *data, size_t *length);

/** Begin the SDHC write
 *
 * @param[in]     obj     The SDHC object that holds the transfer information
 * @param[in]     address The address to write data to
 * @param[in]     data    The transmit buffer
 * @param[in,out] length  Number of bytes to read, updated with the number actually read
 * @return The status of the write_async request
 */
cy_rslt_t cy_sdhc_write_async(const cy_sdhc_t *obj, uint32_t address, const uint8_t *data, size_t *length);

/** Checks if the specified SDHC peripheral is in use
 *
 * @param[in]  obj  The SDHC peripheral to check
 * @param[out] busy Indication of whether the SDHC is still transmitting
 * @return The status of the is_busy request
 */
cy_rslt_t cy_sdhc_is_busy(const cy_sdhc_t *obj, bool *busy);

/** Abort an SDHC transfer
 *
 * @param[in] obj The SDHC peripheral to stop
 * @return The status of the abort_async request
 */
cy_rslt_t cy_sdhc_abort_async(const cy_sdhc_t *obj);

/** The SDHC interrupt handler registration
 *
 * @param[in] obj         The SDHC object
 * @param[in] handler     The callback handler which will be invoked when the interrupt fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 * @return The status of the register_irq request
 */
cy_rslt_t cy_sdhc_register_irq(cy_sdhc_t *obj, cy_sdhc_irq_handler handler, void *handler_arg);

/** Configure SDHC interrupt enablement.
 *
 * @param[in] obj      The SDHC object
 * @param[in] event    The SDHC IRQ type
 * @param[in] enable   True to turn on interrupts, False to turn off
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_sdhc_irq_enable(cy_sdhc_t *obj, cy_sdhc_irq_event_t event, bool enable);

#if defined(__cplusplus)
}
#endif
