/****************************************?***********************************//**
* \file rollback_prot.c
* \version 1.0
*
* \brief
*  This is the source code implementing rollback protection feature.
*
*****************************************?***************************************
* \copyright
* Copyright 2019, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*****************************************?**************************************/

#include <stdint.h>
#include <string.h>

#include "cy_syslib.h"
#include "cy_ipc_drv.h"

#include "bootutil/image.h"
#include "cy_jwt_policy.h"
#include "rollback_prot.h"

/** Rollback counter read/write SysCall opcode */
#define RB_COUNTER_SYSCALL_OPCODE        (0x36UL << 24UL)

/** Rollback counter read/write SysCall return codes */
#define CY_FB_SYSCALL_SUCCESS           (0xA0000000UL)

/** Timeout values */
#define RB_COUNTER_SYSCALL_TIMEOUT      (15000UL)

/** Rollback counter read or write code for SysCall */
#define RB_COUNTER_READ                 (0x00UL << 8UL)
#define RB_COUNTER_WRITE                (0x01UL << 8UL)

/** Rollback counter number for SysCall */
#define RB_COUNTER_N(x)                 ((x) << 16UL)

struct image_header_rollback
{
    uint32_t ih_magic;
    uint32_t ih_load_addr;
    uint16_t ih_hdr_size; /* Size of image header (bytes). */
    uint8_t id;           /* Image ID */
    uint8_t counter;      /* Rollback counter */
    uint32_t ih_img_size; /* Does not include header. */
    uint32_t ih_flags;    /* IMAGE_F_[...]. */
    struct image_version ih_ver;
    uint32_t _pad2;
};

extern bnu_policy_t bnu_policy;

int Cy_UpdateRollbackCounter(uint8_t counter_n, uint32_t value);
uint8_t Cy_GetCounterFromHeader(struct image_header *ih);
int Cy_ReadRollbackCounter(uint8_t counter_n, uint32_t *value);
uint32_t Cy_SysCall_RBCounter(uint32_t *syscallCmd);

int cy_compare_rollback_counter(struct image_header *ih)
{
    uint32_t cntr_image;
    uint32_t cntr_nvm;
    int rc = 0;

    cntr_image = Cy_GetCounterFromHeader(ih);
    rc = Cy_ReadRollbackCounter(bnu_policy.bnu_img_policy.monotonic, &cntr_nvm);

    if(0 == rc)
    {
        if(cntr_image < cntr_nvm)
        {
            rc = -1;
        }
    }
    else
    {
        rc = -1;
    }

    return rc;
}

int cy_write_rollback_counter(struct image_header *ih)
{
    uint32_t cntr_image;
    int rc = 0;

    cntr_image = Cy_GetCounterFromHeader(ih);
    rc = Cy_UpdateRollbackCounter(bnu_policy.bnu_img_policy.monotonic, cntr_image);

    return rc;
}

uint8_t Cy_GetCounterFromHeader(struct image_header *ih)
{
    struct image_header_rollback *ih_rollback =
            (struct image_header_rollback*)ih;

    return ih_rollback->counter;
}

int Cy_ReadRollbackCounter(uint8_t counter_n, uint32_t *value)
{
    int rc = 0;
    uint32_t syscallCmd[2];
    uint32_t status = 0;
    uint32_t temp = 0;

    syscallCmd[0] = RB_COUNTER_SYSCALL_OPCODE +
                     RB_COUNTER_N(counter_n) + RB_COUNTER_READ;
    syscallCmd[1] = (uint32_t)&temp;

    status = Cy_SysCall_RBCounter(syscallCmd);

    if(0 != status)
    {
        rc = status;
    }
    else
    {
        *value = temp;
    }

    return rc;
}

int Cy_UpdateRollbackCounter(uint8_t counter_n, uint32_t value)
{
    int rc = 0;
    uint32_t syscallCmd[2];
    uint32_t status = 0;

    syscallCmd[0] = RB_COUNTER_SYSCALL_OPCODE +
                     RB_COUNTER_N(counter_n) + RB_COUNTER_WRITE;
    syscallCmd[1] = (uint32_t)&value;

    status = Cy_SysCall_RBCounter(syscallCmd);

    if(0 != status)
    {
        rc = status;
    }

    return rc;
}

uint32_t Cy_SysCall_RBCounter(uint32_t *syscallCmd)
{
    int status = 0;
    uint32_t timeout = 0;

    IPC->STRUCT[CY_IPC_CHAN_SYSCALL].DATA =  (uint32_t)syscallCmd;

    while(((IPC->STRUCT[CY_IPC_CHAN_SYSCALL].ACQUIRE &
            IPC_STRUCT_ACQUIRE_SUCCESS_Msk) == 0) &&
            (timeout < RB_COUNTER_SYSCALL_TIMEOUT))
    {
        ++timeout;
    }

    if(timeout < RB_COUNTER_SYSCALL_TIMEOUT)
    {
        timeout = 0;

        IPC->STRUCT[CY_IPC_CHAN_SYSCALL].NOTIFY = 1;

        while(((IPC->STRUCT[CY_IPC_CHAN_SYSCALL].LOCK_STATUS &
                IPC_STRUCT_ACQUIRE_SUCCESS_Msk) != 0) &&
                (timeout < RB_COUNTER_SYSCALL_TIMEOUT))
        {
            ++timeout;
        }

        if(timeout < RB_COUNTER_SYSCALL_TIMEOUT)
        {
            if(CY_FB_SYSCALL_SUCCESS != syscallCmd[0])
            {
                status = syscallCmd[0];
            }
        }
        else
        {
            status = -1;
        }
    }
    else
    {
        status = -1;
    }
    return status;
}
