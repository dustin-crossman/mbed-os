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

#include "flash_smif.h"

#ifdef PU_ENABLE
#include "cyprotection_config.h"
#endif // PU_ENABLE

#ifdef TARGET_MCUBOOT

#include "cy_policy.h"
/* mcuboot Headers */
#include "cy_device_headers.h"
#include "cycfg_peripherals.h"
#include "cycfg_pins.h"
#include "cy_wdt.h"
#include "cycfg.h"
#include "cy_prot.h"

#include "flash_map/flash_map.h"
#include "sysflash/sysflash.h"
#include "flash_psoc6.h"
#include "fb_device.h"
#include "os/os.h"

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
#define CY_BOOTLOADER_IMG_ID_CM0P       (0UL)
#define CY_BOOTLOADER_IMG_ID_SPE_CM0P   (1UL)
#define CY_BOOTLOADER_IMG_ID_CYTF_CM0P  (2UL)
#define CY_BOOTLOADER_IMG_ID_OEMTF_CM0P (3UL)
#define CY_BOOTLOADER_IMG_ID_CM4        (4UL)

#define CY_BOOTLOADER_MASTER_IMG_ID CY_BOOTLOADER_IMG_ID_OEMTF_CM0P

const char devName[] = FLASH_DEV_NAME;
struct device *boot_flash_device;

extern struct flash_map_entry part_map[];

const struct device_config psoc6_flash_dev_cfg =
{
    devName,
    &psoc6_flash_init,
#ifdef CONFIG_DEVICE_POWER_MANAGEMENT
    &psoc6_flash_pm_ctl,
#endif
    NULL
};

const struct device psoc6_flash_device =
{
    (struct device_config*)&psoc6_flash_dev_cfg,
    (void *)&psoc6_flash_api,
    NULL,
};
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

void cy_assert(int expr)
{
    if(0 == expr)
    {
        BOOT_LOG_ERR("There is an error occurred during bootloader flow. MCU stopped.");

        if((CY_GET_REG32(CY_SRSS_TST_MODE_ADDR) & TST_MODE_TEST_MODE_MASK) != 0UL)
        {
            IPC->STRUCT[CY_IPC_CHAN_SYSCALL_DAP].DATA = TST_MODE_ENTERED_MAGIC;
            BOOT_LOG_INF("TEST MODE");
            __disable_irq();
        }

        Cy_SRAM_BusyLoop();
    }
}

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

    app_addr = flash_base + rsp->br_image_off + rsp->br_hdr->ih_hdr_size;

    /* Set Protection Context 6 for CM4 */
    Cy_Prot_SetActivePC(CPUSS_MS_ID_CM4, CY_PROT_PC6);

    /* Stop in case we are in the TEST MODE */
    if ( (CY_GET_REG32(CY_SRSS_TST_MODE_ADDR) & (TST_MODE_TEST_MODE_MASK)) != 0UL )
    {
        IPC->STRUCT[CY_IPC_CHAN_SYSCALL_DAP].DATA = TST_MODE_ENTERED_MAGIC;
        BOOT_LOG_INF("SPE: TEST MODE");
        __disable_irq();
        CPUSS->CM4_VECTOR_TABLE_BASE = CY_BL_CM4_ROM_LOOP_ADDR;
        turn_on_cm4();
        Cy_SysLib_Delay(1);
        CPUSS->CM4_VECTOR_TABLE_BASE = app_addr;
        Cy_SRAM_TestBitLoop();
        __enable_irq();
    }
    /* It is aligned to 0x400 (256 records in vector table*4bytes each) */
    BOOT_LOG_INF("Cy_SysEnableCM4");

    IPC->STRUCT[CY_IPC_CHAN_SYSCALL_DAP].DATA = TST_MODE_ENTERED_MAGIC;
    BOOT_LOG_INF("TEST BIT SET !");

#if(MCUBOOT_LOG_LEVEL != 0)
    while(!Cy_SCB_UART_IsTxComplete(SCB5))
    {
       /* Wait until UART transmission complete */
    }
#endif
    Cy_SysEnableCM4(app_addr);
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

#ifdef MCUBOOT_USE_SMIF_STAGE
    cy_en_smif_status_t qspi_status = CY_SMIF_CMD_NOT_FOUND;

    /* Configure SMIF interrupt */
    cy_stc_sysint_t smifIntConfig =
    {
       .intrSrc = smif_interrupt_IRQn,     /* SMIF interrupt */
       .intrPriority = (1u)       /* SMIF interrupt priority */
    };

    /* SMIF interrupt initialization status */
    cy_en_sysint_status_t intr_init_status;
    intr_init_status = Cy_SysInt_Init(&smifIntConfig, Flash_SMIF_Interrupt_User);

    if(0 != intr_init_status)
    {
         BOOT_LOG_ERR("SMIF Interrupt initialization failed with error code %i", intr_init_status);
    }

    qspi_status = Flash_SMIF_QSPI_Start();

    if(0 != qspi_status)
    {
         BOOT_LOG_ERR("SMIF block failed to start with error code %i", qspi_status);
    }
#if defined(MCUBOOT_USE_SMIF_XIP)
    BOOT_LOG_INF("SMIF Memory Mode");
    /* Forcing default mode to Memory/XIP. */
    Cy_SMIF_SetMode(SMIF0, CY_SMIF_MEMORY);
#else
    BOOT_LOG_INF("SMIF Normal Mode");
    /* Default mode is Memory/XIP. Switching to Normal/CMD */
    Cy_SMIF_SetMode(SMIF0, CY_SMIF_NORMAL);
#endif

#endif

    boot_flash_device = (struct device*)&psoc6_flash_device;

#if(MCUBOOT_POLICY == MCUBOOT_POLICY_JWT)
    /* Processing of policy in JWT format */
    uint32_t jwtLen;
    char *jwt;

    BOOT_LOG_INF("/******************************************************/");
    BOOT_LOG_INF(" SPE + MCUBoot Application ");
    BOOT_LOG_INF("/******************************************************/");
    BOOT_LOG_INF("Processing JWT Policy");
    rc = Cy_JWT_GetProvisioningDetails(FB_POLICY_JWT, &jwt, &jwtLen);
    if(0 != rc)
    {
         BOOT_LOG_ERR("1: Policy parsing failed with code %i", rc);
    }
    if(0 == rc)
    {
        rc = Cy_JWT_ParseProvisioningPacket(jwt, &bnu_policy,
                  CY_BOOTLOADER_IMG_ID_SPE_CM0P); /* SPE IMG ID = 1*/
    }
    if(0 != rc)
    {
         BOOT_LOG_ERR("2: Policy parsing failed with code %i", rc);

         part_map[0].area.fa_off     = MCUBOOT_POLICY_FLASH_AREA_0_START-FLASH_DEVICE_BASE;
         part_map[0].area.fa_size    = MCUBOOT_POLICY_FLASH_AREA_SIZE;

         part_map[1].area.fa_off     = MCUBOOT_POLICY_FLASH_AREA_1_START-FLASH_DEVICE_BASE;
         part_map[1].area.fa_size    = MCUBOOT_POLICY_FLASH_AREA_SIZE;

         bnu_policy.bnu_img_policy.boot_auth[0]      = MCUBOOT_POLICY_BOOT_AUTH;
         bnu_policy.bnu_img_policy.upgrade_auth[0]   = MCUBOOT_POLICY_UPGRADE_AUTH;
         bnu_policy.bnu_img_policy.id                = MCUBOOT_POLICY_IMG_ID;
         bnu_policy.bnu_img_policy.upgrade           = MCUBOOT_POLICY_UPGRADE;
    }
    else
    {
        part_map[0].area.fa_off     = bnu_policy.bnu_img_policy.boot_area.start-FLASH_DEVICE_BASE;
        part_map[0].area.fa_size    = bnu_policy.bnu_img_policy.boot_area.size;

        part_map[1].area.fa_off     = bnu_policy.bnu_img_policy.upgrade_area.start-FLASH_DEVICE_BASE;
        part_map[1].area.fa_size    = bnu_policy.bnu_img_policy.upgrade_area.size;
    }
#else /* (MCUBOOT_POLICY_JWT != MCUBOOT_POLICY_JWT) */
    /* (MCUBOOT_POLICY_JWT == MCUBOOT_POLICY_HDR) */
    part_map[0].area.fa_off     = MCUBOOT_POLICY_FLASH_AREA_0_START-FLASH_DEVICE_BASE;
    part_map[0].area.fa_size    = MCUBOOT_POLICY_FLASH_AREA_SIZE;

    part_map[1].area.fa_off     = MCUBOOT_POLICY_FLASH_AREA_1_START-FLASH_DEVICE_BASE;
    part_map[1].area.fa_size    = MCUBOOT_POLICY_FLASH_AREA_SIZE;

    bnu_policy.bnu_img_policy.boot_auth[0]      = MCUBOOT_POLICY_BOOT_AUTH;
    bnu_policy.bnu_img_policy.upgrade_auth[0]   = MCUBOOT_POLICY_UPGRADE_AUTH;
    bnu_policy.bnu_img_policy.id                = MCUBOOT_POLICY_IMG_ID;
    bnu_policy.bnu_img_policy.upgrade           = MCUBOOT_POLICY_UPGRADE;
#endif

    if((CY_GET_REG32(CY_SRSS_TST_MODE_ADDR) & TST_MODE_TEST_MODE_MASK) != 0UL)
    {
        IPC->STRUCT[CY_IPC_CHAN_SYSCALL_DAP].DATA = TST_MODE_ENTERED_MAGIC;
        BOOT_LOG_INF("TEST MODE");
        __disable_irq();
    } 
    BOOT_LOG_INF("Processing available images");
    rc = boot_go(&rsp);
    if (rc != 0)
    {
        /* indicate M4 image boot failed */
        BOOT_LOG_ERR("Unable to find bootable image");
        while(1);
    }

    /* MCUBoot integration should end before start of CM4 image */
    BOOT_LOG_INF("Jumping to the image in slot 0");
    do_boot(&rsp);

#endif /* TARGET_MCUBOOT */
}

void spm_hal_memory_protection_init(void)
{
#ifdef PU_ENABLE
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
/* TODO: Temporary commented, because it is configured by FlashBoot to fix some silicon issues,
   /* fixed group ppu */
/*#ifndef INITIAL_PROTECTION_AVAILABLE
   status = ppu_fixed_gr_protect((cy_ppu_fixed_gr_cfg_t *)fixed_gr_spm_ppu_config, sizeof(fixed_gr_spm_ppu_config) / sizeof(fixed_gr_spm_ppu_config[0]));
   CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
#endif /* INITIAL_PROTECTION_AVAILABLE */

#endif /* PU_ENABLE */
}
