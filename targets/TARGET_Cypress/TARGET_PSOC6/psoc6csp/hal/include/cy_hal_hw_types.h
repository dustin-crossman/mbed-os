/***************************************************************************//**
* \file cy_hw_types.h
*
* \brief
* Provides a struct definitions for configuration resources in the PDL.
* 
********************************************************************************
* Copyright (c) 2018-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#pragma once

#include "cy_pdl.h"
#include "cy_hal_hw_resources.h"
#include "cy_hal_pin_package.h"
#include <stdbool.h>

#if defined(CY_IP_MXUDB)
    #include "SDIO_HOST.h"
#endif /* defined(CY_IP_MXUDB) */

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t cy_source_t; //!< Routable signal source
typedef uint32_t cy_dest_t; //!< Routable signal destination

/** Available clock divider types */
typedef cy_en_divider_types_t cy_clock_divider_types_t;

typedef struct {
    cy_clock_divider_types_t    div_type;
    uint8_t                     div_num;
} cy_clock_divider_t;


typedef struct {
#ifdef CY_IP_MXS40PASS_SAR
    SAR_Type*                   base;
    cy_resource_inst_t          resource;
    cy_gpio_t                   pin;
    cy_clock_divider_t          clock;
#endif
} cy_adc_t;

typedef struct {
#if defined(CY_IP_MXLPCOMP_INSTANCES) || defined(PASS_NR_CTBS)
    /* TODO: define */
    void * TODO_define;
#else
    void *empty;
#endif
} cy_comp_t;

typedef struct {
#if defined(CY_IP_MXCRYPTO_INSTANCES) || defined(CPUSS_CRYPTO_PRESENT)
    CRYPTO_Type*                base;
    cy_resource_inst_t          resource;
    uint32_t                    crc_width;
#endif
} cy_crc_t;

typedef struct {
#ifdef CY_IP_MXS40PASS_CTDAC
    CTDAC_Type*                 base;
    cy_resource_inst_t          resource;
    cy_gpio_t                   pin;
    cy_clock_divider_t          clock;
#else
    void *empty;
#endif
} cy_dac_t;

typedef struct {
#if defined(CY_IP_M4CPUSS_DMAC_INSTANCES) || defined(CY_IP_M4CPUSS_DMA_INSTANCES)
    /* TODO: define */
    void * TODO_define;
#else
    void *empty;
#endif
} cy_dma_t;

typedef struct {
    /* TODO: define */
    void * TODO_define;
} cy_flash_t;

typedef struct {
#ifdef CY_IP_MXSCB
    CySCB_Type*                            base;
    cy_resource_inst_t                     resource;
    cy_gpio_t                              pin_sda;
    cy_gpio_t                              pin_scl;
    cy_clock_divider_t                     clock;
    cy_stc_scb_i2c_context_t               context;
    cy_stc_scb_i2c_master_xfer_config_t    rx_config;
    cy_stc_scb_i2c_master_xfer_config_t    tx_config;
    bool                                   is_slave;
    uint32_t                               address;
    uint32_t                               irq_cause;
    uint16_t                               pending;
    uint16_t                               events;
    uint32_t                               handler;
#else
    void *empty;
#endif
} cy_i2c_t;

typedef struct {
#ifdef CY_IP_MXAUDIOSS_INSTANCES
    /* TODO: define */
    void * TODO_define;
#else
    void *empty;
#endif
} cy_i2s_t;

typedef struct {
#ifdef PASS_NR_CTBS
    /* TODO: define */
    void * TODO_define;
#else
    void *empty;
#endif
} cy_opamp_t;

typedef struct {
#ifdef CY_IP_MXAUDIOSS_INSTANCES
    /* TODO: define */
    void * TODO_define;
#else
    void *empty;
#endif
} cy_pdm_pcm_t;

typedef struct {
#ifdef CY_IP_MXTCPWM
    TCPWM_Type*                 base;
    cy_resource_inst_t          resource;
    cy_gpio_t                   pin;
    cy_clock_divider_t          clock;
    uint32_t                    clock_hz;
#else
    void *empty;
#endif
} cy_pwm_t;

typedef struct {
#ifdef CY_IP_MXSMIF_INSTANCES
    /* TODO: define */
    void * TODO_define;
#else
    void *empty;
#endif
} cy_qspi_t;

typedef struct {
#if defined(CY_IP_MXCRYPTO_INSTANCES) || defined(CPUSS_CRYPTO_PRESENT)
    /* TODO: define */
    void * TODO_define;
#else
    void *empty;
#endif
} cy_rng_t;

typedef struct {
#ifdef CY_IP_MXS40SRSS_RTC_INSTANCES
    /* TODO: define */
    void * TODO_define;
#else
    void *empty;
#endif
} cy_rtc_t;

typedef struct {
#ifdef CY_IP_MXSDHC
    SDHC_Type*                  base;
    bool                        emmc;
    cy_en_sd_host_dma_type_t    dmaType;
    bool                        enableLedControl;
    cy_stc_sd_host_context_t    context;
#else
    void *empty;
#endif
} cy_sdhc_t;

typedef struct {
#if defined(CY_IP_MXSDHC)
    SDHC_Type*                  base;
    cy_resource_inst_t          resource;
    bool                        emmc;
    cy_en_sd_host_dma_type_t    dmaType;
    bool                        enableLedControl;
    cy_stc_sd_host_context_t    context;

#elif defined(CY_IP_MXUDB)
    cy_resource_inst_t    resource;
    cy_gpio_t             pin_clk;
    cy_gpio_t             pin_cmd;
    cy_gpio_t             pin_data0;
    cy_gpio_t             pin_data1;
    cy_gpio_t             pin_data2;
    cy_gpio_t             pin_data3;

    cy_clock_divider_t    clock;

    cy_dma_t              dma0Ch0;
    cy_dma_t              dma0Ch1;
    cy_dma_t              dma1Ch1;
    cy_dma_t              dma1Ch3;
    
    uint32_t              frequency_hz;
    uint16_t              block_size;
    
    stc_sdio_irq_cb_t*          pfuCb;
    uint32_t                    irqCause;
#endif /* defined(CY_IP_MXSDHC) */
} cy_sdio_t;

typedef struct {
#ifdef CY_IP_MXSCB
    CySCB_Type*                 base;
    cy_resource_inst_t          resource;
    cy_gpio_t                   pin_miso;
    cy_gpio_t                   pin_mosi;
    cy_gpio_t                   pin_sclk;
    cy_gpio_t                   pin_ssel;
    cy_clock_divider_t          clock;
    cy_en_scb_spi_mode_t        ms_mode;
    cy_en_scb_spi_sclk_mode_t   clk_mode;
    uint8_t                     data_bits;
    cy_stc_scb_spi_context_t    context;
#else
    void *empty;
#endif
} cy_spi_t;

typedef struct {
#ifdef CY_IP_MXTCPWM
    /* TODO: define */
    void * TODO_define;
#else
    void *empty;
#endif
} cy_timer_t;

typedef struct {
#ifdef CY_IP_MXSCB
    CySCB_Type*                 base;
    cy_resource_inst_t          resource;
    cy_gpio_t                   pin_rx;
    cy_gpio_t                   pin_tx;
    cy_gpio_t                   pin_cts;
    cy_gpio_t                   pin_rts;
    cy_clock_divider_t          clock;
    cy_stc_scb_uart_context_t   context;
    uint32_t                    irq_cause;
#else
    void *empty;
#endif
} cy_uart_t;

typedef struct {
#ifdef CY_IP_MXS40SRSS_MCWDT_INSTANCES
    /* TODO: define */
    void * TODO_define;
#else
    void *empty;
#endif
} cy_wdt_t;


#if defined(__cplusplus)
}
#endif /* __cplusplus */
