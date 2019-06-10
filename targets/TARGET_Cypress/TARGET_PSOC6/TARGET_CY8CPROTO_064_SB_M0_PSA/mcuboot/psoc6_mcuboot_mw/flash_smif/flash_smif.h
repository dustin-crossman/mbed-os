/*
* Copyright YOUR COMPANY, THE YEAR
* All Rights Reserved
* UNPUBLISHED, LICENSED SOFTWARE.
*
* CONFIDENTIAL AND PROPRIETARY INFORMATION
* WHICH IS THE PROPERTY OF your company.
*/

#if !defined(SMIF_INIT_H)
#define SMIF_INIT_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "cy_pdl.h"
#include "cy_sysclk.h"
#include "cy_smif.h"
#include "cy_smif_memslot.h"
#include "cycfg_peripherals.h"
#include "cy_device_headers.h"
#include "cycfg_qspi_memslot.h"
#include "bootutil_log.h"

#define PACKET_SIZE   (64u)     /* The memory Read/Write packet */
#define TIMEOUT_1_MS  (1000u)

#define IS_FLASH_SMIF(x) ((x)>=(smifMemConfigs[0]->baseAddress-FLASH_DEVICE_BASE))

/* Create context for peripheral operation */
cy_stc_smif_context_t QSPIContext;

void Flash_SMIF_Interrupt_User(void);

void Flash_SMIF_GetAddrBuff(uint32_t address, uint8_t * addrBuf);

/* QSPI initialization */
cy_en_smif_status_t Flash_SMIF_QSPI_Start(void);

int Flash_SMIF_WriteMemory(SMIF_Type *baseaddr,
                            cy_stc_smif_context_t *smifContext,
                            uint8_t txBuffer[],
                            uint32_t txSize,
                            uint8_t *address);

int Flash_SMIF_WriteMemory(SMIF_Type *baseaddr,
                            cy_stc_smif_context_t *smifContext,
                            uint8_t txBuffer[],
                            uint32_t txSize,
                            uint8_t *address);

void Flash_SMIF_EnableQuadMode(SMIF_Type *baseaddr, cy_stc_smif_mem_config_t *memConfig,
                    cy_stc_smif_context_t const *smifContext);

#if defined(__cplusplus)
}
#endif


#endif /* SMIF_INIT_H */
