/* Copyright (c) 2017-2018 ARM Limited
 *
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


/* -------------------------------------- Includes ----------------------------------- */

#include "cy_sysint.h"
#include "spm_internal.h"
#include "cy_device.h"

#ifdef PU_ENABLE
#include "cyprotection_config.h"
#endif // PU_ENABLE

#define CY_SRSS_TST_MODE_ADDR           (SRSS_BASE | 0x0100UL)
#define TST_MODE_TEST_MODE_MASK         (0x80000000UL)
#define TST_MODE_ENTERED_MAGIC          (0x12344321UL)
#define CY_SYS_CM4_PWR_CTL_KEY_OPEN     (0x05FAUL)
#define CY_BL_CM4_ROM_LOOP_ADDR         (0x16004000UL)

static void turn_on_cm4(void)
{
    uint32_t regValue;

    regValue = CPUSS->CM4_PWR_CTL & ~(CPUSS_CM4_PWR_CTL_VECTKEYSTAT_Msk | CPUSS_CM4_PWR_CTL_PWR_MODE_Msk);
    regValue |= _VAL2FLD(CPUSS_CM4_PWR_CTL_VECTKEYSTAT, CY_SYS_CM4_PWR_CTL_KEY_OPEN);
    regValue |= CY_SYS_CM4_STATUS_ENABLED;
    CPUSS->CM4_PWR_CTL = regValue;

    while((CPUSS->CM4_STATUS & CPUSS_CM4_STATUS_PWR_DONE_Msk) == 0UL)
    {
        /* Wait for the power mode to take effect */
    }
}

/* -------------------------------------- HAL API ------------------------------------ */

// These implementations are meant to be used only for SPM running on PSoC6 M0+ core.

void spm_hal_start_nspe(void)
{
    /* Stop in case we are in the TEST MODE */
    if ( (CY_GET_REG32(CY_SRSS_TST_MODE_ADDR) & TST_MODE_TEST_MODE_MASK) != 0UL )
    {
        IPC->STRUCT[CY_IPC_CHAN_SYSCALL_DAP].DATA = TST_MODE_ENTERED_MAGIC;
       // pc.printf("rnok: TEST MODE");
        __disable_irq();
        CPUSS->CM4_VECTOR_TABLE_BASE = CY_BL_CM4_ROM_LOOP_ADDR;
        turn_on_cm4();
        Cy_SysLib_Delay(1);
        CPUSS->CM4_VECTOR_TABLE_BASE = PSA_NON_SECURE_ROM_START;
        while((CY_GET_REG32(CY_SRSS_TST_MODE_ADDR) & TST_MODE_TEST_MODE_MASK) != 0UL);
        __enable_irq();
    }

    Cy_SysEnableCM4(PSA_NON_SECURE_ROM_START);
}

void spm_hal_memory_protection_init(void)
{
#ifndef PU_ENABLE  //+++ rnok: was #ifdef
    cy_en_prot_status_t status;

    /* smpu */
    status = smpu_protect((cy_smpu_region_config_t *)flash_spm_smpu_config, sizeof(flash_spm_smpu_config) / sizeof(flash_spm_smpu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = smpu_protect((cy_smpu_region_config_t *)sram_spm_smpu_config, sizeof(sram_spm_smpu_config) / sizeof(sram_spm_smpu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = smpu_config_unprotected(&default_smpu_master_config);
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead

    /* fixed region ppu */
    status = ppu_fixed_rg_protect((cy_ppu_fixed_rg_cfg_t *)fixed_rg_pc0_ppu_config, sizeof(fixed_rg_pc0_ppu_config) / sizeof(fixed_rg_pc0_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = ppu_fixed_rg_protect((cy_ppu_fixed_rg_cfg_t *)fixed_rg_spm_ppu_config, sizeof(fixed_rg_spm_ppu_config) / sizeof(fixed_rg_spm_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = ppu_fixed_rg_protect((cy_ppu_fixed_rg_cfg_t *)fixed_rg_any_ppu_config, sizeof(fixed_rg_any_ppu_config) / sizeof(fixed_rg_any_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    /* fixed slave ppu */
    status = ppu_fixed_sl_protect((cy_ppu_fixed_sl_cfg_t *)fixed_sl_pc0_ppu_config, sizeof(fixed_sl_pc0_ppu_config) / sizeof(fixed_sl_pc0_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = ppu_fixed_sl_protect((cy_ppu_fixed_sl_cfg_t *)fixed_sl_spm_ppu_config, sizeof(fixed_sl_spm_ppu_config) / sizeof(fixed_sl_spm_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = ppu_fixed_sl_protect((cy_ppu_fixed_sl_cfg_t *)fixed_sl_any_ppu_config, sizeof(fixed_sl_any_ppu_config) / sizeof(fixed_sl_any_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    /* programmable ppu */
    status = ppu_prog_protect((cy_ppu_prog_cfg_t *)prog_pc0_ppu_config, sizeof(prog_pc0_ppu_config) / sizeof(prog_pc0_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = ppu_prog_protect((cy_ppu_prog_cfg_t *)prog_spm_ppu_config, sizeof(prog_spm_ppu_config) / sizeof(prog_spm_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    /* fixed group ppu */
    status = ppu_fixed_gr_protect((cy_ppu_fixed_gr_cfg_t *)fixed_gr_pc0_ppu_config, sizeof(fixed_gr_pc0_ppu_config) / sizeof(fixed_gr_pc0_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = ppu_fixed_gr_protect((cy_ppu_fixed_gr_cfg_t *)fixed_gr_spm_ppu_config, sizeof(fixed_gr_spm_ppu_config) / sizeof(fixed_gr_spm_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead

    /* bus masters */
    status = bus_masters_protect((cy_bus_master_config_t *)bus_masters_config, sizeof(bus_masters_config) / sizeof(bus_masters_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
#endif // PU_ENABLE
}