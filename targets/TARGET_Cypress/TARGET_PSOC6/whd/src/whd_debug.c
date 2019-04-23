/*
 * $ Copyright Cypress Semiconductor Apache2 $
 */

#include "whd_debug.h"
#include "whd_int.h"
#include "bus_protocols/whd_bus_protocol_interface.h"

/******************************************************
*             Constants
******************************************************/

/******************************************************
*             Structures
******************************************************/

/******************************************************
*             Variables
******************************************************/

/******************************************************
*             Function definitions
******************************************************/
void whd_init_stats(whd_driver_t whd_driver)
{
    memset(&whd_driver->whd_stats, 0, sizeof(whd_driver->whd_stats) );
}

uint32_t whd_print_stats(whd_driver_t whd_driver, whd_bool_t reset_after_print)
{
    WPRINT_MACRO( ("WHD Stats.. \n"
                   "tx_total:%ld, rx_total:%ld, tx_no_mem:%ld, rx_no_mem:%ld\n"
                   "tx_fail:%ld, no_credit:%ld, flow_control:%ld\n",
                   whd_driver->whd_stats.tx_total, whd_driver->whd_stats.rx_total,
                   whd_driver->whd_stats.tx_no_mem, whd_driver->whd_stats.rx_no_mem,
                   whd_driver->whd_stats.tx_fail, whd_driver->whd_stats.no_credit,
                   whd_driver->whd_stats.flow_control) );

    if (reset_after_print == WHD_TRUE)
    {
        memset(&whd_driver->whd_stats, 0, sizeof(whd_driver->whd_stats) );
    }

    CHECK_RETURN(whd_bus_print_stats(whd_driver, reset_after_print) );
    return WHD_SUCCESS;
}

