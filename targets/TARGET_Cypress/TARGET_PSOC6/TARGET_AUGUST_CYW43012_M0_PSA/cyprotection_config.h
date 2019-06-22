/* mbed Microcontroller Library
* 
* \copyright

* (c) 2019, Cypress Semiconductor Corporation
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

/* Add to M0_PSA target in targets.json:
 * - INITIAL_ROT_PROTECTION_AVAILABLE - to disable pc0 related protections
 *   enabling. They are enabled by ROT if availabe.
 * - INITIAL_PROTECTION_AVAILABLE - to disable spm related protections
 *   enabling. They are enabled by a bootloader if availabe.
 */

/* Only 7 protection contexts are available in PSoC6-BLE2 */
#define ALL_PROTECTION_CONTEXTS_MASK (CY_PROT_PCMASK1 + CY_PROT_PCMASK2 +\
CY_PROT_PCMASK3 + CY_PROT_PCMASK4 + CY_PROT_PCMASK5 + CY_PROT_PCMASK6 +\
CY_PROT_PCMASK7)
#define SECURE_CONTEXTS_MASK (CY_PROT_PCMASK1 + CY_PROT_PCMASK2 +\
CY_PROT_PCMASK3 + CY_PROT_PCMASK4)

#define ALL_SUBREGIONS (0x0)
#define SECURE_IMAGE_HEADER_SIZE (0x400)

const cy_smpu_region_config_t flash_spm_smpu_config[] = {
    {   /* FLASH_PC1_SPM */
        .address = (uint32_t *)(PSA_SECURE_ROM_START - SECURE_IMAGE_HEADER_SIZE), /* 0x10080000, include header of SPE image */
        .regionSize = CY_PROT_SIZE_512KB, /* 0x80000 */
        .subregions = (CY_PROT_SUBREGION_DIS7 | CY_PROT_SUBREGION_DIS6), /* disable regions 6, 7, end protection on 0x100E0000 */
        .userPermission = CY_PROT_PERM_RWX,
        .privPermission = CY_PROT_PERM_RWX,
        .secure = false,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .prot_region = PROT_SMPU_SMPU_STRUCT4,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .pcMstMask = CY_PROT_PCMASK1,
    }
};                                   

const cy_smpu_region_config_t sram_spm_smpu_config[] = {
    {   /* SRAM_SPM_PRIV - must include SRAM_SPM_PUB area */
        .address = (uint32_t *)PSA_NON_SECURE_RAM_START, /* 0x08000000 */
        .regionSize = CY_PROT_SIZE_256KB, /* 0x40000 */
        /* 0xC7 - disable regions 0, 1, 2 (NSPE RAM) 6, 7 (FLASHBOOT)
           Range 0x08018000...0x08030000 are protected (SPE, CyBootloader) */
        .subregions = (CY_PROT_SUBREGION_DIS7 |
                       CY_PROT_SUBREGION_DIS6 |
                       CY_PROT_SUBREGION_DIS1 |
                       CY_PROT_SUBREGION_DIS0),
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RWX,
        .secure = false,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .prot_region = PROT_SMPU_SMPU_STRUCT8,
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
    .secure = false,
    .pcMatch = false,
    .pcMask = CY_PROT_PCMASK1,
};

const cy_ppu_fixed_rg_cfg_t fixed_rg_spm_ppu_config[] = {
    {   /* IPC_STRUCT3 */
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK - CY_PROT_PCMASK7,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_STRUCT3,
    },
    {   /* IPC_STRUCT4 */
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        /*.pcMask = CY_PROT_PCMASK7,*/
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK - CY_PROT_PCMASK7,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_STRUCT4,
    },
    {   /* IPC_STRUCT5 */
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
    {   /* IPC_STRUCT6 */
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        /*.pcMask = CY_PROT_PCMASK7,*/
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK - CY_PROT_PCMASK7,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_STRUCT6,
    },
    {   /* IPC_INTR_STRUCT1 */
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_INTR_STRUCT1,
    },
    {   /* IPC_INTR_STRUCT2 */
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK - CY_PROT_PCMASK7,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_IPC_INTR_STRUCT2,
    },
    {   /* CPUSS-M4.PROT.SMPU */
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_SMPU,
    },
    {   /* CPUSS-M4.PROT.MPU_CM0P */
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_MPU_CM0P,
    },
    {   /* CPUSS-M4.PROT.MPU_CRYPTO */
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_MPU_CRYPTO,
    },
    {   /* CPUSS-M4.PROT.MPU_TC */
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_MPU_TC,
    },
    {   /* DW1_DW_CH_STRUCT0 */
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_DW1_DW_CH_STRUCT0,
    },
    {   /* DW1_DW_CH_STRUCT1 */
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_DW1_DW_CH_STRUCT1,
    },
    {   /* DW1_DW_CH_STRUCT2 */
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_RG_DW1_DW_CH_STRUCT2,
    },
    {   /* DW1_DW_CH_STRUCT3 */
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
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

const cy_ppu_fixed_sl_cfg_t fixed_sl_spm_ppu_config[] = {
    {   /* CPUSS-M4.CRYPTO */
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_CRYPTO,
    },
    {   /* CPUSS-M4.CPUSS */
        .userPermission = CY_PROT_PERM_DISABLED,
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
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_CPUSS,
    },
    {   /* CPUSS-M4.IPC */
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_IPC,
    },
    {   /* CPUSS-M4.PROT */
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_PROT,
    },
    {   /* SRSS.SRSS */
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
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_GR_PPU_SL_SRSS,
    },/* SRSS.BACKUP. SAS - Needs a secure driver to support secure time. Alternate option, no secure time
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
    },*/
    {
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
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

const cy_ppu_prog_cfg_t prog_spm_ppu_config[] = {
    {   /* PERI.PERI_GROUP_STRUCT[] */
        .address = (uint32_t *)PERI_PPU_GR0,
        .regionSize = CY_PROT_SIZE_1KB,/* 0x00400 */
        .subregions = ALL_SUBREGIONS,
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = SECURE_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_PPU_PR4,
    }
};

const cy_ppu_fixed_gr_cfg_t fixed_gr_spm_ppu_config[] = {
    {   /* PERI.PERI */
        .userPermission = CY_PROT_PERM_RW,
        .privPermission = CY_PROT_PERM_RW,
        .secure = false,
        .pcMatch = false,
        .pcMask = ALL_PROTECTION_CONTEXTS_MASK,
        .userMstPermission = CY_PROT_PERM_R,
        .privMstPermission = CY_PROT_PERM_RW,
        .secureMst = false,
        .pcMstMask = SECURE_CONTEXTS_MASK,
        .pPpuStr = PERI_PPU_GR0,
    }
};

#endif /* _CYPROTECTION_CONFIG_H_ */
