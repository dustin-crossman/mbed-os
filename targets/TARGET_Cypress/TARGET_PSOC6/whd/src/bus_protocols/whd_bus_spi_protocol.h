/*
 * $ Copyright Cypress Semiconductor Apache2 $
 */

#ifndef INCLUDED_SPI_WHD_BUS_PROTOCOL_H
#define INCLUDED_SPI_WHD_BUS_PROTOCOL_H

#include "whd.h"
#include "whd_bus_protocol_interface.h"
#include "cy_result.h"
#include "cy_hal_spi.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
*             Constants
******************************************************/

/******************************************************
*             Structures
******************************************************/

typedef uint32_t whd_bus_gspi_header_t;

#pragma pack(1)

typedef struct
{
    whd_bus_gspi_header_t gspi_header;
} whd_bus_header_t;

#pragma pack()

#define WHD_BUS_SPI_HEADER_SIZE                     (sizeof(whd_bus_header_t) )

#define WHD_BUS_SPI_USE_STATUS_REPORT_SCHEME        (1 == 1)

#define WHD_BUS_SPI_MAX_BACKPLANE_TRANSFER_SIZE     (64)  /* Max packet size on F1 */
#define WHD_BUS_SPI_BACKPLANE_READ_PADD_SIZE        (4)

/******************************************************
*             Function declarations
******************************************************/
extern whd_result_t whd_bus_spi_send_buffer(whd_driver_t whd_driver, whd_buffer_t buffer);
extern whd_result_t whd_bus_spi_poke_wlan(whd_driver_t whd_driver);
extern whd_result_t whd_bus_spi_ack_interrupt(whd_driver_t whd_driver, uint32_t intstatus);
extern uint32_t whd_bus_spi_packet_available_to_read(whd_driver_t whd_driver);
extern whd_result_t whd_bus_spi_read_frame(whd_driver_t whd_driver, whd_buffer_t *buffer);
extern whd_result_t whd_bus_spi_init(whd_driver_t whd_driver);
extern whd_result_t whd_bus_spi_deinit(whd_driver_t whd_driver);
extern whd_bool_t whd_bus_spi_wake_interrupt_present(whd_driver_t whd_driver);
extern whd_result_t whd_bus_spi_wait_for_wlan_event(whd_driver_t whd_driver,
                                                    whd_semaphore_type_t *transceive_semaphore);
extern whd_result_t whd_bus_spi_write_register_value(whd_driver_t whd_driver, whd_bus_function_t function,
                                                     uint32_t address, uint8_t value_length, uint32_t value);
extern whd_result_t whd_bus_spi_read_register_value(whd_driver_t whd_driver, whd_bus_function_t function,
                                                    uint32_t address, uint8_t value_length, /*@out@*/ uint8_t *value);
extern whd_result_t whd_bus_spi_write_backplane_value(whd_driver_t whd_driver, uint32_t address,
                                                      uint8_t register_length, uint32_t value);
extern whd_result_t whd_bus_spi_read_backplane_value(whd_driver_t whd_driver, uint32_t address, uint8_t register_length,
                                                     /*@out@*/ uint8_t *value);
extern whd_result_t whd_bus_spi_transfer_bytes(whd_driver_t whd_driver, whd_bus_transfer_direction_t direction,
                                               whd_bus_function_t function, uint32_t address, uint16_t size,
                                               /*@in@*/ /*@out@*/ whd_transfer_bytes_packet_t *packet);
extern whd_result_t whd_bus_spi_wakeup(whd_driver_t whd_driver);
extern whd_result_t whd_bus_spi_sleep(whd_driver_t whd_driver);
extern void whd_bus_spi_init_stats(whd_driver_t whd_driver);
extern whd_result_t whd_bus_spi_print_stats(whd_driver_t whd_driver, whd_bool_t reset_after_print);
extern whd_result_t whd_bus_spi_reinit_stats(whd_driver_t whd_driver, whd_bool_t wake_from_firmware);
extern uint8_t whd_bus_spi_backplane_read_padd_size(whd_driver_t whd_driver);
extern whd_bool_t whd_bus_spi_use_status_report_scheme(whd_driver_t whd_driver);
extern uint32_t whd_bus_spi_get_max_transfer_size(whd_driver_t whd_driver);
extern whd_result_t whd_bus_spi_irq_register(whd_driver_t whd_driver);
extern whd_result_t whd_bus_spi_irq_enable(whd_driver_t whd_driver, whd_bool_t enable);
extern void whd_bus_spi_irq_handler(void *handler_arg, cy_spi_irq_event_t event);

/******************************************************
*             Global variables
******************************************************/

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif /* ifndef INCLUDED_SPI_WHD_BUS_PROTOCOL_H */

