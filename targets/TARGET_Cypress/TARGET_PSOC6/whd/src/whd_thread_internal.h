/*
 * $ Copyright Cypress Semiconductor Apache2 $
 */

#ifndef INCLUDED_WHD_THREAD_INTERNAL_H
#define INCLUDED_WHD_THREAD_INTERNAL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "whd_debug.h"
#include "whd_int.h"

/******************************************************
*                      Macros
******************************************************/

#define WHD_WLAN_KEEP_AWAKE(whd_driver) \
    do { \
        whd_result_t verify_result; \
        whd_driver->internal_info.whd_wlan_status.keep_wlan_awake++; \
        verify_result = whd_ensure_wlan_bus_is_up(whd_driver); \
        whd_assert("Could not bring bus up", (verify_result == WHD_SUCCESS) ); \
    } while (0)
#define WHD_WLAN_LET_SLEEP(whd_driver) \
    do { \
        whd_driver->internal_info.whd_wlan_status.keep_wlan_awake--; \
        if (whd_driver->internal_info.whd_wlan_status.keep_wlan_awake == 0) \
            whd_thread_notify(whd_driver); \
    } while (0)
#define WHD_WLAN_MAY_SLEEP() \
    ( (whd_driver->internal_info.whd_wlan_status.keep_wlan_awake == 0) && \
      (whd_driver->internal_info.whd_wlan_status.state == WLAN_UP) )

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_WHD_THREAD_INTERNAL_H */

