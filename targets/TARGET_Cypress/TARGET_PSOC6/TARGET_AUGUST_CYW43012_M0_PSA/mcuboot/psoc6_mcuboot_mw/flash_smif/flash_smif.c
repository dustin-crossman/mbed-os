/*
* Copyright YOUR COMPANY, THE YEAR
* All Rights Reserved
* UNPUBLISHED, LICENSED SOFTWARE.
*
* CONFIDENTIAL AND PROPRIETARY INFORMATION
* WHICH IS THE PROPERTY OF your company.
*/

#include "flash_smif.h"

cy_en_smif_status_t QSPI_Start(void)
{
    cy_en_smif_status_t qspiStatus = CY_SMIF_BAD_PARAM;

    qspiStatus = Cy_SMIF_Init(SMIF0, &SMIF_config, TIMEOUT_1_MS, &QSPIContext);

    if(qspiStatus == CY_SMIF_SUCCESS)
    {
        Cy_SMIF_SetDataSelect(SMIF0, CY_SMIF_SLAVE_SELECT_0, CY_SMIF_DATA_SEL0);
        Cy_SMIF_Enable(SMIF0, &QSPIContext);
        BOOT_LOG_INF("SMIF Block Enabled\r\n");
    }

    if(qspiStatus == CY_SMIF_SUCCESS)
    {
        /* Map memory device to memory map */
        qspiStatus = Cy_SMIF_Memslot_Init(SMIF0, &smifBlockConfig, &QSPIContext);
    }

    if(qspiStatus == CY_SMIF_SUCCESS)
    {
        /* Put the device in XIP mode */
        BOOT_LOG_INF("\r\nEntering XIP Mode\r\n");
        Cy_SMIF_SetMode(SMIF0, CY_SMIF_MEMORY);
    }

    return qspiStatus;
}
