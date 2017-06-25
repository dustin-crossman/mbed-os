#ifndef _SDK_CONFIG_H_
#define _SDK_CONFIG_H_

#include "nrf_drv_config.h"

#define UART_DEFAULT_CONFIG_BAUDRATE UART0_CONFIG_BAUDRATE
#define UART_DEFAULT_CONFIG_PARITY   UART0_CONFIG_PARITY
#define UART_DEFAULT_CONFIG_HWFC     UART0_CONFIG_HWFC

/* Use config at PinNames.h file in TARGET_XXXX directory */
//#define CTS_PIN_NUMBER               UART0_CONFIG_PSEL_CTS
//#define RTS_PIN_NUMBER               UART0_CONFIG_PSEL_RTS

#define SPIS_DEFAULT_ORC                 NRF_DRV_SPIS_DEFAULT_ORC
#define SPIS_DEFAULT_CONFIG_IRQ_PRIORITY SPIS1_CONFIG_IRQ_PRIORITY;
#define SPIS_DEFAULT_ORC                 NRF_DRV_SPIS_DEFAULT_ORC
#define SPIS_DEFAULT_DEF                 NRF_DRV_SPIS_DEFAULT_DEF

#define SPI_DEFAULT_CONFIG_IRQ_PRIORITY  SPI1_CONFIG_IRQ_PRIORITY

#endif