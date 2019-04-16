/*
 * $ Copyright Cypress Semiconductor $
 */

#include "whd_ap.h"
#include "whd_chip_constants.h"
#include "whd_debug.h"
#include "whd_events_int.h"
#include "whd_sdpcm.h"
#include "whd_thread_internal.h"
#include "whd_utils.h"
#include "whd_wifi_api.h"
#include "whd_wifi_p2p.h"


/******************************************************
* @cond       Constants
******************************************************/

whd_bool_t whd_wifi_p2p_is_go_up(whd_driver_t whd_driver)
{
    return whd_driver->internal_info.whd_wifi_p2p_go_is_up;
}

void whd_wifi_p2p_set_go_is_up(whd_driver_t whd_driver, whd_bool_t is_up)
{
    if (whd_driver->internal_info.whd_wifi_p2p_go_is_up != is_up)
    {
        whd_driver->internal_info.whd_wifi_p2p_go_is_up = is_up;
    }
}

