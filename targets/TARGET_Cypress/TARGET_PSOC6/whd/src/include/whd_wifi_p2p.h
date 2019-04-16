/*
 * $ Copyright Cypress Semiconductor $
 */



#ifndef INCLUDED_WHD_WIFI_P2P_H
#define INCLUDED_WHD_WIFI_P2P_H

#include "whd_rtos.h" /* For semaphores */
#include "whd.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** @cond !ADDTHIS*/

/**
 * Set whether the p2p GO is up or not
 * @param is_up   : specify whether the p2p GO is up currently or not
 */
extern void whd_wifi_p2p_set_go_is_up(whd_driver_t whd_driver, whd_bool_t is_up);

/**
 * @return WHD_TRUE if the P2P GO is currently up
 */
extern whd_bool_t whd_wifi_p2p_is_go_up(whd_driver_t whd_driver);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_WHD_WIFI_P2P_H */

