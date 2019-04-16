/*
 * $ Copyright Cypress Semiconductor $
 */
#ifndef INCLUDED__WHD_BUS_COMMON_H
#define INCLUDED_WHD_BUS_COMMON_H

#include "whd.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct whd_bus_common_info;

void whd_bus_common_info_init(whd_driver_t whd_driver);

/* Configure delayed sleep of bus.
 * A high number will mean a longer delay before sleep after the last operation (higher performance)
 * A shorter delay will mean quicker sleep after last operation (lower power use)
 */
extern void         whd_bus_sdio_wlan_set_delayed_bus_powersave_milliseconds(whd_driver_t whd_driver, uint32_t time_ms);
extern whd_result_t whd_bus_resume_after_deep_sleep(whd_driver_t whd_driver);

extern whd_result_t whd_bus_write_wifi_firmware_image(whd_driver_t whd_driver);
extern whd_result_t whd_bus_write_wifi_nvram_image(whd_driver_t whd_driver);
extern void         whd_bus_set_resource_download_halt(whd_driver_t whd_driver, whd_bool_t halt);

extern whd_result_t whd_bus_transfer_backplane_bytes(whd_driver_t whd_driver, whd_bus_transfer_direction_t direction,
                                                     uint32_t address, uint32_t size, /*@in@*/ /*@out@*/ uint8_t *data);
extern void whd_bus_init_backplane_window(whd_driver_t whd_driver);
whd_result_t whd_bus_set_backplane_window(whd_driver_t whd_driver, uint32_t addr);

whd_bool_t whd_bus_is_up(whd_driver_t whd_driver);
extern whd_result_t whd_bus_set_flow_control(whd_driver_t whd_driver, uint8_t value);
extern whd_bool_t whd_bus_is_flow_controlled(whd_driver_t whd_driver);

void whd_bus_set_state(whd_driver_t whd_driver, whd_bool_t state);

extern void whd_delayed_bus_release_schedule_update(whd_driver_t whd_driver, whd_bool_t is_scheduled);

/* handle delayed sleep of bus */
extern uint32_t     whd_bus_handle_delayed_release(whd_driver_t whd_driver);
whd_bool_t whd_bus_platform_mcu_power_save_deep_sleep_enabled(whd_driver_t whd_driver);

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif

