/*******************************************************************************
* File Name: cycfg_routing.c
*
* Description:
* Establishes all necessary connections between hardware elements.
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

#include "cycfg_routing.h"

#include "cy_device_headers.h"

void init_cycfg_routing(void)
{
	CTBM0->CTD_SW = CTBM_CTD_SW_CTDH_CIS_Msk;
	CTBM0->OA0_SW = CTBM_OA0_SW_OA0M_A81_Msk |
		CTBM_OA0_SW_OA0P_A20_Msk;
	CTBM0->OA1_SW = CTBM_OA1_SW_OA1M_A22_Msk |
		CTBM_OA1_SW_OA1P_A13_Msk;
	SAR->MUX_SWITCH0 = SAR_MUX_SWITCH0_MUX_FW_P1_VPLUS_Msk |
		SAR_MUX_SWITCH0_MUX_FW_P2_VPLUS_Msk;
}
