
/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifdef DEVICE_WATCHDOG

#include "mbed_watchdog_mgr.h"

static bool is_watchdog_started = false; //boolean to control watchdog start and stop
#define MS_TO_US(x) ((x) * 1000) //macro to convert millisecond to microsecond

MBED_STATIC_ASSERT((HW_WATCHDOG_TIMEOUT >= 0),"Timeout must be greater than zero");


/** Create singleton instance of LowPowerTicker for watchdog periodic call back of kick.
 */
static LowPowerTicker *get_ticker()
{
    static LowPowerTicker ticker;
    return &ticker;
}

/** Refreshes the watchdog timer.
 *
 * This function should be called periodically before the watchdog times out.
 * Otherwise, the system is reset.
 *
 * If the watchdog timer is not currently running this function does nothing
 */
static void mbed_wdog_manager_kick()
{
    core_util_critical_section_enter();
    hal_watchdog_kick();
    core_util_critical_section_exit();
}

uint32_t mbed_wdog_manager_get_max_timeout()
{
    const watchdog_features_t features = hal_watchdog_get_platform_features();
    return features.max_timeout;
}

bool mbed_wdog_manager_start()
{

    watchdog_status_t sts;
    bool msts = true;
    MBED_ASSERT(HW_WATCHDOG_TIMEOUT < mbed_wdog_manager_get_max_timeout());
    core_util_critical_section_enter();
    if(is_watchdog_started) {
    	core_util_critical_section_exit();
        return false;
    }
    watchdog_config_t config;
    config.timeout_ms = HW_WATCHDOG_TIMEOUT;
    sts = hal_watchdog_init(&config);
    if(sts != WATCHDOG_STATUS_OK) {
        msts = false;
    } else {
        us_timestamp_t timeout = (MS_TO_US(HW_WATCHDOG_TIMEOUT)/2);
        get_ticker()->attach_us(callback(&mbed_wdog_manager_kick),timeout);
        is_watchdog_started = true;
    }
    core_util_critical_section_exit();
    return msts;
}

bool mbed_wdog_manager_stop()
{
    watchdog_status_t sts;
    bool msts = true;
    core_util_critical_section_enter();
    if(is_watchdog_started) {
        sts = hal_watchdog_stop();
        if(sts != WATCHDOG_STATUS_OK) {
            msts = false;
        } else {
            get_ticker()->detach();
            is_watchdog_started = false;
        }

    } else {
         msts = false;
    }
    core_util_critical_section_exit();
    return msts;
}


uint32_t mbed_wdog_manager_get_reload_value()
{
    return hal_watchdog_get_reload_value();
}


#endif // DEVICE_WATCHDOG
