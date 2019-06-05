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

/* Create context for peripheral operation */
cy_stc_smif_context_t QSPIContext;

/* QSPI initialization */
cy_en_smif_status_t QSPI_Start(void);

#if defined(__cplusplus)
}
#endif


#endif /* SMIF_INIT_H */
