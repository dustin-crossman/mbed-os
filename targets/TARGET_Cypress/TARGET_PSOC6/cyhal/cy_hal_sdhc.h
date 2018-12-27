/*******************************************************************************
* File Name: cy_hal_sdhc.h
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

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cy_sd_host.h"
#include "cy_gpio.h"
#include "PinNames.h"

#ifdef CY_IP_MXSDHC

typedef struct {
    SDHC_Type *base;
    bool emmc;                          /**< Set to true of eMMC otherwise false. */
    cy_en_sd_host_dma_type_t dmaType;   /**< Selects the DMA type to be used. */
    bool enableLedControl;              /**< If true the SD clock controls one IO 
                                          *   used to indicate when the card 
                                          *    is being accessed. 
                                          */
    cy_stc_sd_host_context_t context;
} sdhc_t;

/** SD/eMMC card configuration structure. */
typedef struct
{
    bool                          lowVoltageSignaling; /**< If true, the host supports the 1.8V signaling. */ 
    cy_en_sd_host_bus_width_t     busWidth;            /**< The desired bus width. */
}cardConfig_t;

typedef cy_stc_sd_host_cmd_config_t cmdConfig_t;

/** Initialize the SDHC peripheral
 *
 * @param[out] obj   The SDHC object
 * @param[out] clk   The pin connected to the clk signal
 * @param[in]  cmd   The pin connected to the command signal
 * @param[in]  data0 The pin connected to the data0 signal
 * @param[in]  data1 The pin connected to the data1 signal
 * @param[in]  data2 The pin connected to the data2 signal
 * @param[in]  data3 The pin connected to the data3 signal
 * @param[in]  data4 The pin connected to the data4 signal
 * @param[in]  data5 The pin connected to the data5 signal
 * @param[in]  data6 The pin connected to the data6 signal
 * @param[in]  data7 The pin connected to the data7 signal
 * @param[in]  cardDetect The pin connected to the cardDetect signal
 * @param[out] ioVoltSel The pin connected to the ioVoltSel signal
 * @param[out] cardIfPwrEn The pin connected to the cardIfPwrEn signal
 * @param[in]  cardMechWriteProt The pin connected to the cardMechWriteProt signal
 * @param[in]  ledCtrl The pin connected to the ledCtrl signal
 * @param[in]  cardEmmcReset The pin connected to the cardEmmcReset signal
 * @return The status of the is_rx_active request
 */
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
                   PinName cardEmmcReset);

/** Sets the frequency of the SD Clock
 *
 * @param[in,out] obj The SDHC object
 * @param[in] Frequency of SD Clock in Hz
 * @return The status of the command request
 */
cy_rslt_t sdhc_set_sd_clk_frequency(sdhc_t *obj, uint32_t frequency);

/** Starts sending a command on the SD bus
 *
 * @param[in,out] obj The SDHC object
 * @param[in] config The configuration structure for the command
 * @return The status of the command request
 */
cy_rslt_t sdhc_send_command(sdhc_t *obj, const cmdConfig_t *config);

/** Reads the response register from the last completed command
 *
 * @param[in,out] obj The SDHC object
 * @param[out] response The pointer to response data
 * @return The status of the response request
 */
cy_rslt_t sdhc_get_response(sdhc_t *obj, uint32_t *response);                 
                   
/** Release the SDHC peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param[in,out] obj The SDHC object
 * @return The status of the free request
 */
cy_rslt_t sdhc_free(sdhc_t *obj);

/** Configure the SDHC block.
 *
 * @param[in,out] obj The SDHC object
 * @param[in,out] config The card configuration object
 * @return The status of the configure request
 */
cy_rslt_t sdhc_configure(sdhc_t *obj, cardConfig_t *config);

/** Attempts to read data over the SDHC peripheral.
 *
 * @param[in]     obj    The SDHC object
 * @param[in,out] address The address to Write/Read data on the card or eMMC
 * @param[out]    data   Pointer to the byte-array of data to read from the device
 * @param[in,out] length Number of blocks to read
 * @return The status of the read request
 */
cy_rslt_t sdhc_read(const sdhc_t *obj, uint32_t address, uint8_t *data, size_t length);

/** Attempts to write data over SDHC peripheral
 *
 * @param[in]     obj    The SDHC object
 * @param[in,out] address The address to Write/Read data on the card or eMMC
 * @param[out]    data   Pointer to the byte-array of data to read from the device
 * @param[in,out] length Number of blocks to write
 * @return The status of the write request
 */
cy_rslt_t sdhc_write(const sdhc_t *obj, uint32_t address, const uint8_t *data, size_t length);

/** Attempts to erase a block of data over the SDHC peripheral
 *
 * @param[in] obj       The SDHC object
 * @param[in] startAddr Is the address of the first byte to erase
 * @param[in] length    The number of bytes (starting at startAddr) to erase
 * @return The status of the erase request
 */
cy_rslt_t sdhc_erase(const sdhc_t *obj, uint32_t startAddr, size_t length);

/** Begin the SDHC read
 *
 * @param[in]     obj     The SDHC object that holds the transfer information
 * @param[out]    data    The receive buffer
 * @param[in,out] length  Number of bytes to read, updated with the number actually read
 * @param[in]     event   The logical OR of events to be registered
 * @param[in]     handler SDHC interrupt handler
 * @return The status of the read_asynch request
 */
cy_rslt_t sdhc_read_asynch(const sdhc_t *obj, uint32_t address, uint8_t *data, size_t length, uint32_t handler, uint32_t event);

/** Begin the SDHC write
 *
 * @param[in]     obj     The SDHC object that holds the transfer information
 * @param[in]     data    The transmit buffer
 * @param[in,out] length  Number of bytes to read, updated with the number actually read
 * @param[in]     event   The logical OR of events to be registered
 * @param[in]     handler SDHC interrupt handler
 * @return The status of the write_asynch request
 */
cy_rslt_t sdhc_write_asynch(const sdhc_t *obj, uint32_t address, const uint8_t *data, size_t length, uint32_t handler, uint32_t event);

/** Checks if the specified SDHC peripheral is in use
 *
 * @param[in] obj  The SDHC peripheral to check
 * @param[in] busy Indication of whether the SDHC is still transmitting
 * @return The status of the is_busy request
 */
cy_rslt_t sdhc_is_busy(const sdhc_t *obj, bool *busy);

/** Abort an SDHC transfer
 *
 * @param[in] obj The SDHC peripheral to stop
 * @return The status of the abort_asynch request
 */
cy_rslt_t sdhc_abort_asynch(const sdhc_t *obj);

#endif /* CY_IP_MXSDHC */

#if defined(__cplusplus)
}
#endif
