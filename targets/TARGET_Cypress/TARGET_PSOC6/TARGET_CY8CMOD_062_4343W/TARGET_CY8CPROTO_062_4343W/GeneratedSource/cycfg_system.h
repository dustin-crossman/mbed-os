/*******************************************************************************
* File Name: cycfg_system.h
*
* Description:
* Peripheral Hardware Block configuration
* This file was automatically generated and should not be modified.
* 
********************************************************************************
* Copyright 2017-2019 Cypress Semiconductor Corporation
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
********************************************************************************/

#if !defined(CYCFG_SYSTEM_H)
#define CYCFG_SYSTEM_H

#include "cycfg_notices.h"
#include "cy_sysclk.h"
#include "cy_gpio.h"
#include "cy_syspm.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define CY_CFG_SYSCLK_CLKLF_FREQ_HZ 32768
#define CY_CFG_PWR_MODE_LP 0x01UL
#define CY_CFG_PWR_MODE_ULP 0x02UL
#define CY_CFG_PWR_MODE_ACTIVE 0x04UL
#define CY_CFG_PWR_MODE_SLEEP 0x08UL
#define CY_CFG_PWR_MODE_DEEPSLEEP 0x10UL
#define CY_CFG_PWR_SYS_IDLE_MODE CY_CFG_PWR_MODE_DEEPSLEEP
#define CY_CFG_PWR_SYS_ACTIVE_MODE CY_CFG_PWR_MODE_LP
#define CY_CFG_PWR_DEEPSLEEP_LATENCY 0UL
#define CY_CFG_PWR_USING_LDO 1
#define CY_CFG_PWR_VDDA_MV 3300
#define CY_CFG_PWR_VDDD_MV 3300
#define CY_CFG_PWR_VBACKUP_MV 3300
#define CY_CFG_PWR_VDD_NS_MV 3300
#define CY_CFG_PWR_VDDIO0_MV 3300
#define CY_CFG_PWR_VDDIO1_MV 3300

void init_cycfg_system(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_SYSTEM_H */
