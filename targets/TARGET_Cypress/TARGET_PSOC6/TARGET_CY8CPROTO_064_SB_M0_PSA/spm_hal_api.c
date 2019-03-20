/* Copyright (c) 2017-2018 ARM Limited
 * Copyright 2018-2019 Cypress Semiconductor Corporation
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
#include <assert.h>

#include "cy_sysint.h"
#include "spm_internal.h"
#include "cy_device.h"

#ifndef PU_ENABLE
#include "cyprotection_config.h"
#endif // PU_ENABLE

#ifdef TARGET_MCUBOOT

/* mcuboot Headers */
#include "cy_device_headers.h"
#include "cycfg_peripherals.h"
#include "cycfg_pins.h"
#include "cy_wdt.h"
#include "cycfg.h"
#include "cy_prot.h"

#include "bootutil.h"
#include "bootutil_log.h"
#include "image.h"

#include "base64.h"
#include "cJSON.h"
#include "cy_jwt_policy.h"
#include "cy_jwt_bnu_policy.h"
/* ------------------ */
#include "flash_map.h"

#define PROVISIONING_JWT_ADDR           (0x100FDA00UL)

#define CY_BL_CM4_ROM_LOOP_ADDR (0x16004000UL)
#define CY_BOOTLOADER_IMG_ID_CM0P		(0UL)
#define CY_BOOTLOADER_IMG_ID_TEE_CM0P	(1UL)
#define CY_BOOTLOADER_IMG_ID_CYTF_CM0P	(2UL)
#define CY_BOOTLOADER_IMG_ID_OEMTF_CM0P	(3UL)
#define CY_BOOTLOADER_IMG_ID_CM4		(4UL)

#define CY_BOOTLOADER_MASTER_IMG_ID CY_BOOTLOADER_IMG_ID_OEMTF_CM0P

const char devName[] = FLASH_DEV_NAME;
struct device *boot_flash_device;

extern struct flash_map_entry part_map[];

/* ------------------------------ */

#define CY_SRSS_TST_MODE_ADDR           (SRSS_BASE | 0x0100UL)
#define TST_MODE_TEST_MODE_MASK         (0x80000000UL)
#define TST_MODE_ENTERED_MAGIC          (0x12344321UL)
#define CY_SYS_CM4_PWR_CTL_KEY_OPEN     (0x05FAUL)
#define CY_BL_CM4_ROM_LOOP_ADDR         (0x16004000UL)


/* MCUBoot types */

typedef struct
{
    uint32_t len;
    char str[];
}jwt_ptr_t;

/* Boot & Upgrade policy structure */
bnu_policy_t bnu_policy;


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

CY_SECTION(".cy_ramfunc") CY_NOINLINE
static void Cy_SRAM_BusyLoop(void)
{
    while(1);
}

CY_SECTION(".cy_ramfunc") CY_NOINLINE
static void Cy_SRAM_TestBitLoop(void)
{
    while((CY_GET_REG32(CY_SRSS_TST_MODE_ADDR) & TST_MODE_TEST_MODE_MASK) != 0UL);
}

static void do_boot(struct boot_rsp *rsp)
{
    uintptr_t flash_base;
    uint32_t app_addr = 0;
    int rc;

    /* The beginning of the image is the ARM vector table, containing
     * the initial stack pointer address and the reset vector
     * consecutively. Manually set the stack pointer and jump into the
     * reset vector
     */
    rc = flash_device_base(rsp->br_flash_dev_id, &flash_base);
    assert(rc == 0);

    while(!Cy_SCB_UART_IsTxComplete(SCB5))
    {
       /* Wait until UART transmission complete */
    }

    app_addr = flash_base + rsp->br_image_off + rsp->br_hdr->ih_hdr_size;

    /* Set Protection Context 6 for CM4 */
    Cy_Prot_SetActivePC(CPUSS_MS_ID_CM4, CY_PROT_PC6);

    /* Stop in case we are in the TEST MODE */
    if ( (CY_GET_REG32(CY_SRSS_TST_MODE_ADDR) & (TST_MODE_TEST_MODE_MASK)) != 0UL )
    {
        IPC->STRUCT[CY_IPC_CHAN_SYSCALL_DAP].DATA = TST_MODE_ENTERED_MAGIC;
        // BOOT_LOG_INF("TEST MODE");
        __disable_irq();
        CPUSS->CM4_VECTOR_TABLE_BASE = CY_BL_CM4_ROM_LOOP_ADDR;
        turn_on_cm4();
        Cy_SysLib_Delay(1);
        CPUSS->CM4_VECTOR_TABLE_BASE = app_addr;
        Cy_SRAM_TestBitLoop();
        __enable_irq();
    }
}
#endif /* TARGET_MCUBOOT */
/* -------------------------------------- HAL API ------------------------------------ */

// These implementations are meant to be used only for SPM running on PSoC6 M0+ core.

void spm_hal_start_nspe(void)
{
#ifdef TARGET_MCUBOOT
	/* MCUBoot integration starts here */
	
	struct boot_rsp rsp;
    int rc = 0;
	
	/* Processing of policy in JWT format */
    jwt_ptr_t *jwt = (jwt_ptr_t*)PROVISIONING_JWT_ADDR;
    rc = Cy_JWT_ParseProvisioningPacket(jwt->str, &bnu_policy,
            CY_BOOTLOADER_MASTER_IMG_ID);
    if(0 != rc)
    {
        // BOOT_LOG_ERR("Policy parsing failed with code %i", rc);
    }

    part_map[0].area.fa_off = bnu_policy.bnu_img_policy.boot_area.start-FLASH_DEVICE_BASE;
    part_map[0].area.fa_size = bnu_policy.bnu_img_policy.boot_area.size;

    part_map[1].area.fa_off = bnu_policy.bnu_img_policy.upgrade_area.start-FLASH_DEVICE_BASE;
    part_map[1].area.fa_size = bnu_policy.bnu_img_policy.upgrade_area.size;

    // BOOT_LOG_INF("Processing available images");
    rc = boot_go(&rsp);
	if (rc != 0)
    {
        // BOOT_LOG_ERR("Unable to find bootable image");
        /* indicate M4 image boot failed */
        while(1)
        {
            // Cy_GPIO_Inv(LED_PORT, LED_PIN);
            // Cy_SysLib_Delay(100);
        }
    }

    // BOOT_LOG_INF("Jumping to the image in slot 0");

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
	
	do_boot(&rsp);
	
	/* MCUBoot integration should end before start of CM4 image */
#endif /* TARGET_MCUBOOT */

    Cy_SysEnableCM4(PSA_NON_SECURE_ROM_START);
}

void spm_hal_memory_protection_init(void)
{
#ifndef PU_ENABLE //+++ rnok: was #ifdef
    cy_en_prot_status_t status = CY_PROT_SUCCESS;

    /* smpu */
    status = smpu_protect((cy_smpu_region_config_t *)flash_spm_smpu_config, sizeof(flash_spm_smpu_config) / sizeof(flash_spm_smpu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = smpu_protect((cy_smpu_region_config_t *)sram_spm_smpu_config, sizeof(sram_spm_smpu_config) / sizeof(sram_spm_smpu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = smpu_config_unprotected(&default_smpu_master_config);
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead

    /* fixed region ppu */
#ifndef INITIAL_PROTECTION_AVAILABLE
    status = ppu_fixed_rg_protect((cy_ppu_fixed_rg_cfg_t *)fixed_rg_spm_ppu_config, sizeof(fixed_rg_spm_ppu_config) / sizeof(fixed_rg_spm_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
#endif /* INITIAL_PROTECTION_AVAILABLE */
    status = ppu_fixed_rg_protect((cy_ppu_fixed_rg_cfg_t *)fixed_rg_any_ppu_config, sizeof(fixed_rg_any_ppu_config) / sizeof(fixed_rg_any_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    /* fixed slave ppu */
#ifndef INITIAL_PROTECTION_AVAILABLE
    status = ppu_fixed_sl_protect((cy_ppu_fixed_sl_cfg_t *)fixed_sl_spm_ppu_config, sizeof(fixed_sl_spm_ppu_config) / sizeof(fixed_sl_spm_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
#endif /* INITIAL_PROTECTION_AVAILABLE */
    status = ppu_fixed_sl_protect((cy_ppu_fixed_sl_cfg_t *)fixed_sl_any_ppu_config, sizeof(fixed_sl_any_ppu_config) / sizeof(fixed_sl_any_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    /* programmable ppu */
#ifndef INITIAL_PROTECTION_AVAILABLE
    status = ppu_prog_protect((cy_ppu_prog_cfg_t *)prog_spm_ppu_config, sizeof(prog_spm_ppu_config) / sizeof(prog_spm_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
#endif /* INITIAL_PROTECTION_AVAILABLE */
    /* fixed group ppu */
#ifndef INITIAL_PROTECTION_AVAILABLE
    status = ppu_fixed_gr_protect((cy_ppu_fixed_gr_cfg_t *)fixed_gr_spm_ppu_config, sizeof(fixed_gr_spm_ppu_config) / sizeof(fixed_gr_spm_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
#endif /* INITIAL_PROTECTION_AVAILABLE */

#endif /* PU_ENABLE */
}
