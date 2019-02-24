/* mbed Microcontroller Library
* 
* \copyright

* (c) 2018, Cypress Semiconductor Corporation
* or a subsidiary of Cypress Semiconductor Corporation. All rights
* reserved.
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

#ifndef _CYPROTECTION_CONFIG_H_
#define _CYPROTECTION_CONFIG_H_

#include "cyprotection.h"

<<<<<<< HEAD
/* Add to M0_PSA target in targets.json:
 * - INITIAL_ROT_PROTECTION_AVAILABLE - to disable pc0 related protections
 *   enabling. They are enabled by ROT if availabe.
 * - INITIAL_PROTECTION_AVAILABLE - to disable spm related protections
 *   enabling. They are enabled by a bootloader if availabe.
 */

=======
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
/* Only 7 protection contexts are available in PSoC6-BLE2 */
#define ALL_PROTECTION_CONTEXTS_MASK (CY_PROT_PCMASK1 + CY_PROT_PCMASK2 +\
CY_PROT_PCMASK3 + CY_PROT_PCMASK4 + CY_PROT_PCMASK5 + CY_PROT_PCMASK6 +\
CY_PROT_PCMASK7)
#define SECURE_CONTEXTS_MASK (CY_PROT_PCMASK1 + CY_PROT_PCMASK2 +\
CY_PROT_PCMASK3 + CY_PROT_PCMASK4)

#define ALL_SUBREGIONS (0x0)

const cy_smpu_region_config_t flash_spm_smpu_config[] = {
<<<<<<< HEAD
    {   /* FLASH_PC1_SPM */
        .address = (uint32_t *)PSA_SECURE_ROM_START, /* 0x10000000 */
        .regionSize = CY_PROT_SIZE_512KB, /* 0x80000 */
        .subregions = ALL_SUBREGIONS,
        .userPermission = CY_PROT_PERM_RWX,
=======
    /* FLASH_PC1_SPM */
    {
        .address = (uint32_t *)(0x10000000),
        .regionSize = CY_PROT_SIZE_512KB,
        .subregions = ALL_SUBREGIONS,
        .userPermission = CY_PROT_PERM_RX,
        /*.privPermission = CY_PROT_PERM_RX,*/
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .privPermission = CY_PROT_PERM_RWX,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
<<<<<<< HEAD
        .prot_region = PROT_SMPU_SMPU_STRUCT5,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .pcMstMask = CY_PROT_PCMASK1,
=======
        .prot_region = PROT_SMPU_SMPU_STRUCT7,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
    }
};

const cy_smpu_region_config_t sram_spm_smpu_config[] = {
<<<<<<< HEAD
    {   /* SRAM_SPM_PRIV - must include SRAM_SPM_PUB area */
        .address = (uint32_t *)PSA_SECURE_RAM_START, /* 0x08020000 */
        .regionSize = CY_PROT_SIZE_64KB, /* 0x10000 */
        .subregions = ALL_SUBREGIONS, /* 0xC0, /*Size 0xC000 ALL_SUBREGIONS,*/
        .userPermission = CY_PROT_PERM_DISABLED,
=======
    /* SRAM_SPM_PRIV */
    {
        .address = (uint32_t *)(0x08000000),
        .regionSize = CY_PROT_SIZE_64KB,
        .subregions = ALL_SUBREGIONS,
        .userPermission = CY_PROT_PERM_DISABLED,
        /*.privPermission = CY_PROT_PERM_RW,*/
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .privPermission = CY_PROT_PERM_RWX,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
<<<<<<< HEAD
        .prot_region = PROT_SMPU_SMPU_STRUCT9,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .pcMstMask = CY_PROT_PCMASK1,
    }
};

/* Only privileged secure PC=1 master can change the SMPUs */
const cy_stc_smpu_cfg_t default_smpu_master_config = {
    .address = NULL, /* Not used */
    .regionSize = CY_PROT_SIZE_256B, /* Not used */
    .subregions = ALL_SUBREGIONS, /* Not used */
    .userPermission = CY_PROT_PERM_R,
    .privPermission = CY_PROT_PERM_RW,
    .secure = true,
    .pcMatch = false,
    .pcMask = CY_PROT_PCMASK1,
};

const cy_ppu_fixed_rg_cfg_t fixed_rg_pc0_ppu_config[] = {
    {   /* CM0+ SysCall */
        .userPermission = CY_PROT_PERM_RW,
=======
        .prot_region = PROT_SMPU_SMPU_STRUCT12,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
    }
};

const cy_ppu_fixed_rg_cfg_t fixed_rg_pc0_ppu_config[] = {
    {
        .userPermission = CY_PROT_PERM_DISABLED,
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_STRUCT0,
    },
<<<<<<< HEAD
    {   /* CM4 SysCall */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK - CY_PROT_PCMASK7,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_STRUCT1,
    },
<<<<<<< HEAD
    {   /* DAP SysCall */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
<<<<<<< HEAD
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
=======
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK - CY_PROT_PCMASK7,
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_STRUCT2,
    },
<<<<<<< HEAD
    {   /* CM0+ NMI */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = 0,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_INTR_STRUCT0,
    }
};

const cy_ppu_fixed_rg_cfg_t fixed_rg_spm_ppu_config[] = {
<<<<<<< HEAD
    {   /* IPC_STRUCT3 */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK - CY_PROT_PCMASK7,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_STRUCT3,
    },
<<<<<<< HEAD
    {   /* IPC_STRUCT4 */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        /*.pcMask = CY_PROT_PCMASK7,*/
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK - CY_PROT_PCMASK7,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_STRUCT4,
    },
<<<<<<< HEAD
    {   /* IPC_STRUCT5 */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK - CY_PROT_PCMASK7,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_STRUCT5,
    },
<<<<<<< HEAD
    {   /* IPC_STRUCT6 */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        /*.pcMask = CY_PROT_PCMASK7,*/
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK - CY_PROT_PCMASK7,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_STRUCT6,
    },
<<<<<<< HEAD
    {   /* IPC_INTR_STRUCT1 */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_INTR_STRUCT1,
    },
<<<<<<< HEAD
    {   /* IPC_INTR_STRUCT2 */
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK - CY_PROT_PCMASK7,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_INTR_STRUCT2,
    },
    {   /* CPUSS-M4.PROT.SMPU */
=======
    {
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK - CY_PROT_PCMASK7,
        .pPpuStr = PERI_GR_PPU_RG_IPC_INTR_STRUCT2,
    },
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_SMPU,
    },
<<<<<<< HEAD
    {   /* CPUSS-M4.PROT.MPU_CM0P */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_MPU_CM0P,
    },
<<<<<<< HEAD
    {   /* CPUSS-M4.PROT.MPU_CRYPTO */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_MPU_CRYPTO,
    },
<<<<<<< HEAD
    {   /* CPUSS-M4.PROT.MPU_TC */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_MPU_TC,
    },
<<<<<<< HEAD
    {   /* DW1_DW_CH_STRUCT0 */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_DW1_DW_CH_STRUCT0,
    },
<<<<<<< HEAD
    {   /* DW1_DW_CH_STRUCT1 */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_DW1_DW_CH_STRUCT1,
    },
<<<<<<< HEAD
    {   /* DW1_DW_CH_STRUCT2 */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_DW1_DW_CH_STRUCT2,
    },
<<<<<<< HEAD
    {   /* DW1_DW_CH_STRUCT3 */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_DW1_DW_CH_STRUCT3,
    }
};

const cy_ppu_fixed_rg_cfg_t fixed_rg_any_ppu_config[] = {
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_STRUCT7,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_INTR_STRUCT3,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_INTR_STRUCT4,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_INTR_STRUCT5,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_INTR_STRUCT6,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_INTR_STRUCT7,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_MPU_CM4,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_DW0_DW_CH_STRUCT0,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_DW0_DW_CH_STRUCT1,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_DW0_DW_CH_STRUCT2,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_DW0_DW_CH_STRUCT3,
    }
};

const cy_ppu_fixed_sl_cfg_t fixed_sl_pc0_ppu_config[] = {
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_PERI_GR1,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_PERI_GR2,
    },
<<<<<<< HEAD
    {   /* Flash controller */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        /*.userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_DISABLED,*/
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_FLASHC,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_EFUSE,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_PERI_GR3,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_PERI_GR4,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_PERI_GR6,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_PERI_GR9,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_PERI_GR10,
    }
};

const cy_ppu_fixed_sl_cfg_t fixed_sl_spm_ppu_config[] = {
<<<<<<< HEAD
    {   /* CPUSS-M4.CRYPTO */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_CRYPTO,
    },
<<<<<<< HEAD
    {   /* CPUSS-M4.CPUSS */
        .userPermission = CY_PROT_PERM_DISABLED,
=======
    {
        .userPermission = CY_PROT_PERM_RW,
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        /*
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
         */
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_CPUSS,
    },
<<<<<<< HEAD
    {   /* CPUSS-M4.IPC */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_IPC,
    },
<<<<<<< HEAD
    {   /* CPUSS-M4.PROT */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_PROT,
    },
<<<<<<< HEAD
    {   /* SRSS.SRSS */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        /*
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
         */
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_SRSS,
<<<<<<< HEAD
    },/* SRSS.BACKUP. SAS - Needs a secure driver to support secure time. Alternate option, no secure time
=======
    },
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
    {
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_BACKUP,
<<<<<<< HEAD
    },*/
=======
    },
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
    {
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_DW1,
    }/*,
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_SMIF,
    }*/
};

const cy_ppu_fixed_sl_cfg_t fixed_sl_any_ppu_config[] = {
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_FAULT,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_DW0,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_PROFILE,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_HSIOM,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_GPIO,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_SMARTIO,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_UDB,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_LPCOMP,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_CSD,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_TCPWM0,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_TCPWM1,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_LCD,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_BLE,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_USBFS,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_SMIF,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_SCB0,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_SCB1,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_SCB2,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_SCB3,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_SCB4,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_SCB5,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_SCB6,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_SCB7,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_SCB8,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_PASS,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_I2S,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_RW,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_PDM,
    }
};

const cy_ppu_prog_cfg_t prog_pc0_ppu_config[] = {
    {
<<<<<<< HEAD
        .address = (uint32_t *)(CPUSS_BASE),
        .regionSize = CY_PROT_SIZE_2KB,/* 0x40210300 + 0x4FF */
        .subregions = 0x07,
=======
        .address = (uint32_t *)0x40210300,
        .regionSize = CY_PROT_SIZE_2KB,/* 0x4FF */
        .subregions = 0xE0,
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_R,
        .privPermission = CY_PROT_PERM_R,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_PPU_PR0,
    },
    {
<<<<<<< HEAD
        .address = (uint32_t *)(CPUSS_BASE + 0x8000),
=======
        .address = (uint32_t *)0x40218000,
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .regionSize = CY_PROT_SIZE_32KB,/* 0x08000 */
        .subregions = ALL_SUBREGIONS,
        .userPermission = CY_PROT_PERM_R,
        .privPermission = CY_PROT_PERM_R,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_PPU_PR1,
    },
    {
<<<<<<< HEAD
        .address = (uint32_t *)EFUSE_BASE,
        .regionSize = CY_PROT_SIZE_2KB,/* 0x0800 */
=======
        .address = (uint32_t *)0x402C0000,
        .regionSize = CY_PROT_SIZE_32KB,/* 0x08000 */
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .subregions = ALL_SUBREGIONS,
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_DISABLED,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_PPU_PR2,
    },
    {
<<<<<<< HEAD
        .address = (uint32_t *)(EFUSE_BASE + 0x0800),
        .regionSize = CY_PROT_SIZE_256B,/* 0x402C0840 + 0x00C0 */
        .subregions = 0x03,
=======
        .address = (uint32_t *)0x402C0840,
        .regionSize = CY_PROT_SIZE_256B,/* 0x00C0 */
        .subregions = 0xC0,
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_DISABLED,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_PPU_PR3,
    }
};

const cy_ppu_prog_cfg_t prog_spm_ppu_config[] = {
<<<<<<< HEAD
    {   /* PERI.PERI_GROUP_STRUCT[] */
        .address = (uint32_t *)PERI_PPU_GR0,
=======
    {
        .address = (uint32_t *)0x40010000,
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .regionSize = CY_PROT_SIZE_1KB,/* 0x00400 */
        .subregions = ALL_SUBREGIONS,
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = true,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_PPU_PR4,
    }
};

const cy_ppu_fixed_gr_cfg_t fixed_gr_pc0_ppu_config[] = {
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_PPU_GR1,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_PPU_GR2,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_PPU_GR3,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_PPU_GR4,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_PPU_GR6,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_PPU_GR9,
    },
    {
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_R,
        .secureMst = false,
        .pcMstMask = ALL_PROTECTION_CONTEXTS_MASK,
        .pPpuStr = PERI_PPU_GR10,
    }
};

const cy_ppu_fixed_gr_cfg_t fixed_gr_spm_ppu_config[] = {
<<<<<<< HEAD
    {   /* PERI.PERI */
=======
    {
>>>>>>> 829c3e7865... Copied files for CY8CKIT_062_WIFI_BT_M0_PSA from FUTURE_SEQUANA
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = true,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_PPU_GR0,
    }
};

const cy_bus_master_config_t bus_masters_config[] = {
    {
        .busMaster = CPUSS_MS_ID_CM4,
        .privileged = true,
        .secure = false,
        .pcMask = CY_PROT_PCMASK6,
        .act_pc = CY_PROT_PC6,
    },
    {
        .busMaster = CPUSS_MS_ID_TC,
        .privileged = false,
        .secure = false,
        .pcMask = CY_PROT_PCMASK7,
        .act_pc = CY_PROT_PC7,
    },
    {
        .busMaster = CPUSS_MS_ID_CRYPTO,
        .privileged = true,
        .secure = true,
        .pcMask = SECURE_CONTEXTS_MASK,
        .act_pc = CY_PROT_PC1,
    },
    {
        .busMaster = CPUSS_MS_ID_CM0,
        .privileged = true,
        .secure = true,
        .pcMask = SECURE_CONTEXTS_MASK,
        .act_pc = CY_PROT_PC1,
    }
};

#endif /* _CYPROTECTION_CONFIG_H_ */
