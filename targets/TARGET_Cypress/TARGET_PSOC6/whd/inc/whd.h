/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *  Abstract type of whd_driver structure
 */

#ifndef INCLUDED_WHD_H
#define INCLUDED_WHD_H

#include "whd_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct whd_driver *whd_driver_t;
typedef struct whd_interface *whd_interface_t;
typedef struct whd_buffer_funcs whd_buffer_funcs_t;
typedef struct whd_netif_funcs whd_netif_funcs_t;
typedef struct whd_resource_source whd_resource_source_t;

typedef struct whd_spi_config
{
    /* Bus config */
    whd_bool_t is_spi_normal_mode; /* Default is false */
    whd_bool_t is_spi_irq_falling_edge; /* Default is false */
} whd_spi_config_t;

typedef struct whd_init_config
{
    void *thread_stack_start; /* Pointer to the WHD thread stack  */
    uint32_t thread_stack_size; /* size of the    WHD thread stack  */
    uint32_t thread_priority; /* Priority to be set to WHD Thread */
    whd_country_code_t country;
    uint8_t oob_gpio_pin;
} whd_init_config_t;

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* INCLUDED_WHD_H */
