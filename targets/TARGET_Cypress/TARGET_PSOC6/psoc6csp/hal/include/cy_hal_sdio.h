/***************************************************************************//**
* \file cy_hal_sdio.h
*
* \brief
* Provides a high level interface for interacting with the Cypress SDIO interface. 
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


#define SDIO_RET_NO_ERRORS           (0x00)     /**< No error*/
#define SDIO_RET_NO_SP_ERRORS        (0x01)     /**< Non-specific error code*/
#define SDIO_RET_CMD_CRC_ERROR       (0x02)     /**< There was a CRC error on the Command/Response*/
#define SDIO_RET_CMD_IDX_ERROR       (0x04)     /**< The index for the command didn't match*/
#define SDIO_RET_CMD_EB_ERROR        (0x08)     /**< There was an end bit error on the command*/
#define SDIO_RET_DAT_CRC_ERROR       (0x10)     /**< There was a data CRC Error*/
#define SDIO_RET_CMD_TIMEOUT         (0x20)     /**< The command didn't finish before the timeout period was over*/
#define SDIO_RET_DAT_TIMEOUT         (0x40)     /**< The data didn't finish before the timeout period was over*/
#define SDIO_RET_RESP_FLAG_ERROR     (0x80)      /**< There was an error in the resposne flag for command 53*/ 

#define CY_SDIO_CLOCK_ERROR          (0x100)   /**< Failed to initial clock for SDIO */
#define CY_SDIO_BAD_ARGUMENT         (0x200)   /**< Bad argument passed for SDIO */


/* HAL return value defines */

/** Incorrect parameter value define */
#define CY_RSLT_SDIO_BAD_PARAM_ARGUMENT        CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, \
                                                             CY_RSLT_MODULE_SDIO, \
                                                             CY_SDIO_BAD_ARGUMENT)

/** Clock initialization error define */
#define CY_RSLT_SDIO_CLOCK_ERROR              CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, \
                                                             CY_RSLT_MODULE_SDIO, \
                                                             CY_SDIO_CLOCK_ERROR)
                                                             
                                                             
                                                             
/** Error define based on SDIO lower function return value */
#define CY_RSLT_SDIO_FUNC_RET_ERROR(retVal)   CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, \
                                                             CY_RSLT_MODULE_SDIO, (retVal))


/** Commands that can be issued */
typedef enum
{
    SDIO_CMD_GO_IDLE_STATE  =  0, //!> Go to idle state
    SDIO_CMD_SEND_RELATIVE_ADDR  =  3, //!> Send a relative address
    SDIO_CMD_IO_SEND_OP_COND  =  5, //!> Send an OP IO
    SDIO_CMD_SELECT_CARD  =  7, //!> Send a card select
    SDIO_CMD_GO_INACTIVE_STATE = 15, //!> Go to inactive state
    SDIO_CMD_IO_RW_DIRECT = 52, //!> Perform a direct read/write
    SDIO_CMD_IO_RW_EXTENDED = 53, //!> Perform an extended read/write
} cy_sdio_command_t;

/** Types of transfer that can be performed */
typedef enum
{
    CY_READ, //!> Read from the card
    CY_WRITE //!> Write to the card
} cy_transfer_t;

/** SDIO controller initial configuration */
typedef struct
{
    uint32_t frequency_hz; //!< Clock frequency, in hertz
    uint16_t block_size; //!< Block size
} cy_sdio_cfg_t;

/** Events that can cause an SDIO interrupt */
typedef enum {
    CY_SDIO_CMD_COMPLETE, //!> Command Complete
    CY_SDIO_XFER_COMPLETE, //!> Host read/write transfer is complete
    CY_SDIO_BGAP_EVENT, //!> This bit is set when both read/write transaction is stopped
    CY_SDIO_DMA_INTERRUPT, //!> Host controller detects an SDMA Buffer Boundary during transfer
    CY_SDIO_BUF_WR_READY, //!> This bit is set if the Buffer Write Enable changes from 0 to 1
    CY_SDIO_BUF_RD_READY, //!> This bit is set if the Buffer Read Enable changes from 0 to 1
    CY_SDIO_CARD_INSERTION, //!> This bit is set if the Card Inserted in the Present State
    CY_SDIO_CARD_REMOVAL, //!> This bit is set if the Card Inserted in the Present State
    CY_SDIO_CARD_INTERRUPT, //!> The synchronized value of the DAT[1] interrupt input for SD mode
    CY_SDIO_INT_A, 
    CY_SDIO_INT_B,
    CY_SDIO_INT_C,
    CY_SDIO_RE_TUNE_EVENT, //!> This bit is set if the Re-Tuning Request changes from 0 to 1
    CY_SDIO_FX_EVENT, //!> This status is set when R[14] of response register is set to 1
    CY_SDIO_CQE_EVENT, //!> This status is set if Command Queuing/Crypto event has occurred
    CY_SDIO_ERR_INTERRUPT, //!> If any of the bits in the Error Interrupt Status register are set
    CY_SDIO_ALL_INTERRUPTS, //!> Is used to enable/disable all interrupts
} cy_sdio_irq_event_t;

/** Handler for SDIO interrupts */
typedef void (*cy_sdio_irq_handler)(void *handler_arg, cy_sdio_irq_event_t event);

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
cy_rslt_t cy_sdio_init(cy_sdio_t *obj, cy_gpio_t cmd, cy_gpio_t clk, cy_gpio_t data0, cy_gpio_t data1, cy_gpio_t data2, cy_gpio_t data3);

/** Release the SDIO peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param[in,out] obj The SDIO object
 * @return The status of the free request
 */
cy_rslt_t cy_sdio_free(cy_sdio_t *obj);

/** Configure the SDIO block.
 *
 * @param[in,out] obj    The SDIO object
 * @param[in]     config The sdio configuration to apply
 * @return The status of the configure request
 */
cy_rslt_t cy_sdio_configure(cy_sdio_t *obj, const cy_sdio_cfg_t *config);

/** Sends a command to the SDIO block.
 *
 * @param[in,out] obj       The SDIO object
 * @param[in]     direction The direction of transfer (read/write)
 * @param[in]     command   The SDIO command to send
 * @param[in]     argument  The argument to the command
 * @param[out]    response  The response from the SDIO device
 * @return The status of the configure request
 */
cy_rslt_t cy_sdio_send_cmd(const cy_sdio_t *obj, cy_transfer_t direction, cy_sdio_command_t command, uint32_t argument, uint32_t* response);

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
cy_rslt_t cy_sdio_bulk_transfer(const cy_sdio_t *obj, cy_transfer_t direction, uint32_t argument, const uint32_t* data, uint16_t length, uint32_t* response);

/** Performs a bulk asynchronus data transfer (CMD=53) to the SDIO block.
 *
 * @param[in,out] obj       The SDIO object
 * @param[in]     direction The direction of transfer (read/write)
 * @param[in]     argument  The argument to the command
 * @param[in]     data      The data to send to the SDIO device
 * @param[in]     length    The number of bytes to send
 * @return The status of the configure request
 */
cy_rslt_t cy_sdio_transfer_async(const cy_sdio_t *obj, cy_transfer_t direction, uint32_t argument, const uint32_t* data, uint16_t length);

/** Checks if the specified SDIO is in use
 *
 * @param[in]  obj  The SDIO peripheral to check
 * @param[out] busy Indication of whether the SDIO is still transmitting
 * @return The status of the is_busy request
 */
cy_rslt_t cy_sdio_is_busy(const cy_sdio_t *obj, bool *busy);

/** Abort an SDIO transfer
 *
 * @param[in] obj The SDIO peripheral to stop
 * @return The status of the abort_async request
 */
cy_rslt_t cy_sdio_abort_async(const cy_sdio_t *obj);

/** The sdio interrupt handler registration
 *
 * @param[in] obj         The SDIO object
 * @param[in] handler     The callback handler which will be invoked when the interrupt fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 * @return The status of the register_irq request
 */
cy_rslt_t cy_sdio_register_irq(cy_sdio_t *obj, cy_sdio_irq_handler handler, void *handler_arg);

/** Configure sdio interrupt.
 *
 * @param[in] obj      The SDIO object
 * @param[in] event    The sdio IRQ type
 * @param[in] enable   Set to non-zero to enable events, or zero to disable them
 * @return The status of the irq_enable request
 */
cy_rslt_t cy_sdio_irq_enable(cy_sdio_t *obj, cy_sdio_irq_event_t event, bool enable);

#if defined(__cplusplus)
}
#endif
