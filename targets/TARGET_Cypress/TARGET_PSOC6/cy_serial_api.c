/***************************************************************************//**
* \file cy_serial_api.c
*
* \brief
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "hal/serial_api.h"
#include "cyhal_uart.h"
#include "mbed_error.h"
#include "PeripheralPins.h"
#include "PinNames.h"

#if DEVICE_SERIAL

#ifdef __cplusplus
extern "C" {
#endif

int stdio_uart_inited = 0;
serial_t stdio_uart;

static inline struct serial_s *cy_serial_get_struct(serial_t *obj)
{
#if DEVICE_SERIAL_ASYNCH
    return &(obj->serial);
#else
    return obj;
#endif
}

static void serial_handler_internal(void *handler_arg, cyhal_uart_irq_event_t event)
{
    serial_t *obj = (serial_t *)handler_arg;
    struct serial_s *ser = cy_serial_get_struct(obj);
#if DEVICE_SERIAL_ASYNCH
    ser->event_flags = event;
    void (*async_rx_handler)(void) = (void (*)(void))ser->async_rx_handler;
    if (NULL != async_rx_handler && (event & ser->rx_event_mask))
        (*async_rx_handler)();
    void (*async_tx_handler)(void) = (void (*)(void)ser->async_tx_handler;
    if (NULL != async_tx_handler && (event & ser->tx_event_mask))
        (*async_tx_handler)();
#endif
    void (*handler)(uint32_t, int) = (void (*)(uint32_t, int))ser->handler;
    if (NULL != handler)
    {
        if (CYHAL_UART_IRQ_NONE != (event & ser->rx_event_mask))
            (*handler)(ser->handler_arg, (int)RxIrq);
        if (CYHAL_UART_IRQ_NONE != (event & ser->tx_event_mask))
            (*handler)(ser->handler_arg, (int)TxIrq);
    }
}

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    if (tx == STDIO_UART_TX && stdio_uart_inited != 0)
    {
        memmove(obj, &stdio_uart, sizeof(serial_t));
        return;
    }
    memset(obj, 0, sizeof(serial_t));
    struct serial_s *ser = cy_serial_get_struct(obj);
    ser->pin_tx = tx;
    ser->pin_rx = rx;
    ser->pin_rts = NC;
    ser->pin_cts = NC;
    if (CY_RSLT_SUCCESS != cyhal_uart_init(&(ser->hal_obj), tx, rx, NULL, NULL))
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_uart_init");
    if (CY_RSLT_SUCCESS != cyhal_uart_register_irq(&(ser->hal_obj), &serial_handler_internal, obj))
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_uart_register_irq");
    static const cyhal_uart_irq_event_t ENABLE_EVENTS =
        CYHAL_UART_IRQ_TX_DONE | CYHAL_UART_IRQ_TX_ERROR |
        CYHAL_UART_IRQ_RX_DONE | CYHAL_UART_IRQ_RX_ERROR;
    if (CY_RSLT_SUCCESS != cyhal_uart_irq_enable(&(ser->hal_obj), ENABLE_EVENTS, true))
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_uart_irq_enable");
    if (tx == STDIO_UART_TX)
    {
        memmove(&stdio_uart, obj, sizeof(serial_t));
        stdio_uart_inited = 1;
    }
}

void serial_free(serial_t *obj)
{
    struct serial_s *ser = cy_serial_get_struct(obj);
    if (CY_RSLT_SUCCESS != cyhal_uart_free(&(ser->hal_obj)))
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_FAILED_OPERATION), "serial_free");
}

void serial_baud(serial_t *obj, int baudrate)
{
    struct serial_s *ser = cy_serial_get_struct(obj);
    if (CY_RSLT_SUCCESS != cyhal_uart_baud(&(ser->hal_obj), baudrate))
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_FAILED_OPERATION), "serial_baud");
    ser->baud = (uint32_t)baudrate;
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    struct serial_s *ser = cy_serial_get_struct(obj);
    struct serial_s orig = *ser;
    cyhal_uart_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.data_bits = data_bits;
    cfg.stop_bits = stop_bits;
    switch (parity)
    {
        case ParityNone: cfg.parity = CYHAL_UART_PARITY_NONE; break;
        case ParityOdd: cfg.parity = CYHAL_UART_PARITY_ODD; break;
        case ParityEven: cfg.parity = CYHAL_UART_PARITY_EVEN; break;
        default:
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_UNSUPPORTED), "Unsupported parity");
            break;
    }
    if (CY_RSLT_SUCCESS != cyhal_uart_free(&(ser->hal_obj)))
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_uart_free");
    if (CY_RSLT_SUCCESS != cyhal_uart_init(&(ser->hal_obj), orig.pin_tx, orig.pin_rx, NULL, &cfg))
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_uart_init");
    serial_set_flow_control(obj, FlowControlNone, orig.pin_rts, orig.pin_cts);
    serial_baud(obj, orig.baud);
    serial_irq_handler(obj, (uart_irq_handler)orig.handler, orig.handler_arg);
    ser->rx_event_mask = orig.rx_event_mask;
    ser->tx_event_mask = orig.tx_event_mask;
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    struct serial_s *ser = cy_serial_get_struct(obj);
    ser->handler_arg = id;
    // The struct uses a different type because objects.h cannot include serial_api.h without creating a cycle
    ser->handler = (void*)handler;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    struct serial_s *ser = cy_serial_get_struct(obj);
    if (irq == RxIrq)
    {
        ser->rx_event_mask = enable
            ? (ser->rx_event_mask | CYHAL_UART_IRQ_RX_DONE | CYHAL_UART_IRQ_RX_ERROR)
            : (ser->rx_event_mask & ~(CYHAL_UART_IRQ_RX_DONE | CYHAL_UART_IRQ_RX_ERROR));
    }
    else if (irq == TxIrq)
    {
        ser->tx_event_mask = enable
            ? (ser->tx_event_mask | CYHAL_UART_IRQ_TX_DONE | CYHAL_UART_IRQ_TX_ERROR)
            : (ser->tx_event_mask & ~(CYHAL_UART_IRQ_TX_DONE | CYHAL_UART_IRQ_TX_ERROR));
    }
}

int  serial_getc(serial_t *obj)
{
    struct serial_s *ser = cy_serial_get_struct(obj);
    uint8_t value;
    if (CY_RSLT_SUCCESS != cyhal_uart_getc(&(ser->hal_obj), &value, 0))
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_READ_FAILED), "serial_getc");
    return value;
}

void serial_putc(serial_t *obj, int c)
{
    struct serial_s *ser = cy_serial_get_struct(obj);
    if (CY_RSLT_SUCCESS != cyhal_uart_putc(&(ser->hal_obj), (uint32_t)c))
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_WRITE_FAILED), "serial_putc");
}

int  serial_readable(serial_t *obj)
{
    struct serial_s *ser = cy_serial_get_struct(obj);
    return cyhal_uart_readable(&(ser->hal_obj)) > 0 ? 1 : 0;
}

int  serial_writable(serial_t *obj)
{
    struct serial_s *ser = cy_serial_get_struct(obj);
    return cyhal_uart_writable(&(ser->hal_obj)) > 0 ? 1 : 0;
}

void serial_clear(serial_t *obj)
{
    struct serial_s *ser = cy_serial_get_struct(obj);
    if (CY_RSLT_SUCCESS != cyhal_uart_clear(&(ser->hal_obj)))
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_WRITE_FAILED), "serial_clear");
}

void serial_break_set(serial_t *obj)
{
    MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_UNSUPPORTED), "serial_break_set");
}

void serial_break_clear(serial_t *obj)
{
    MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_UNSUPPORTED), "serial_break_clear");
}

void serial_pinout_tx(PinName tx)
{
    MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_UNSUPPORTED), "serial_pinout_tx");
}

void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
    struct serial_s *ser = cy_serial_get_struct(obj);
    if (CY_RSLT_SUCCESS != cyhal_uart_set_flow_control(&(ser->hal_obj), rxflow, txflow))
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_FAILED_OPERATION), "serial_set_flow_control");
    ser->pin_rts = rxflow;
    ser->pin_cts = txflow;
}

const PinMap *serial_tx_pinmap(void)
{
    return PinMap_UART_TX;
}

const PinMap *serial_rx_pinmap(void)
{
    return PinMap_UART_RX;
}

const PinMap *serial_cts_pinmap(void)
{
    return PinMap_UART_CTS;
}

const PinMap *serial_rts_pinmap(void)
{
    return PinMap_UART_RTS;
}

#if DEVICE_SERIAL_ASYNCH

int serial_tx_asynch(serial_t *obj, const void *tx, size_t tx_length, uint8_t, uint32_t handler, uint32_t event, DMAUsage)
{
    struct serial_s *ser = cy_serial_get_struct(obj);
    // handler calls serial_irq_handler_asynch
    ser->async_tx_handler = (void*)handler;
    if (CY_RSLT_SUCCESS != cyhal_uart_tx_asynch(&(ser->hal_obj), tx, tx_length))
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_WRITE_FAILED), "serial_tx_asynch");
    return 0;
}

void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length, uint8_t, uint32_t handler, uint32_t event, uint8_t char_match, DMAUsage)
{
    struct serial_s *ser = cy_serial_get_struct(obj);
    // handler calls serial_irq_handler_asynch
    ser->async_rx_handler = (void*)handler;
    if (CY_RSLT_SUCCESS != cyhal_uart_rx_asynch(&(ser->hal_obj), rx, rx_length))
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_WRITE_FAILED), "serial_rx_asynch");
}

uint8_t serial_tx_active(serial_t *obj)
{
    bool active;
    return (CY_RSLT_SUCCESS == cyhal_uart_is_tx_active(&(ser->hal_obj), &active) && active) ? 1 : 0;
}

uint8_t serial_rx_active(serial_t *obj);
{
    bool active;
    return (CY_RSLT_SUCCESS == cyhal_uart_is_rx_active(&(ser->hal_obj), &active) && active) ? 1 : 0;
}

int serial_irq_handler_asynch(serial_t *obj)
{
    struct serial_s *ser = cy_serial_get_struct(obj);
    int flags = 0;
    if (ser->event_flags & CYHAL_UART_TX_DONE)
    {
        ser->async_tx_handler = NULL;
        flags |= SERIAL_EVENT_TX_COMPLETE;
    }
    if (ser->event_flags & CYHAL_UART_RX_DONE)
    {
        ser->async_rx_handler = NULL;
        flags |= SERIAL_EVENT_RX_COMPLETE;
    }
    // HAL event type does not specify which error occurred
    if (ser->event_flags & CYHAL_UART_RX_ERROR)
        flags |= SERIAL_EVENT_RX_FRAMING_ERROR;
    return flags;
}

void serial_tx_abort_asynch(serial_t *obj)
{
    struct serial_s *ser = cy_serial_get_struct(obj);
    if (CY_RSLT_SUCCESS != cyhal_uart_tx_abort(&(ser->hal_obj)))
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_FAILED_OPERATION), "serial_tx_abort_asynch");
}

void serial_rx_abort_asynch(serial_t *obj)
{
    struct serial_s *ser = cy_serial_get_struct(obj);
    if (CY_RSLT_SUCCESS != cyhal_uart_rx_abort(&(ser->hal_obj)))
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_FAILED_OPERATION), "serial_rx_abort_asynch");
}

#endif

#ifdef __cplusplus
}
#endif

#endif
