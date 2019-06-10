/*
* Copyright YOUR COMPANY, THE YEAR
* All Rights Reserved
* UNPUBLISHED, LICENSED SOFTWARE.
*
* CONFIDENTIAL AND PROPRIETARY INFORMATION
* WHICH IS THE PROPERTY OF your company.
*/


#include <stdio.h>
#include <string.h>

#include <cycfg_qspi_memslot.h>
#include <cycfg_peripherals.h>

#include "cy_pdl.h"
#include "cycfg.h"
#include "flash_smif.h"

/***************************************************************************
* Global constants
***************************************************************************/

#define SMIF_PRIORITY       (1u)      /* SMIF interrupt priority */
#define SMIF_TIMEOUT_1_MS   (1000ul)  /* 1 ms timeout for all blocking functions */
#define PACKET_SIZE         (64u)    /* The memory Read/Write packet */
#define ADDRESS_SIZE        (3u)      /* Memory address size */
#define SUCCESS             (0u)
#define SMIF_NVIC_IRQN      (NvicMux7_IRQn)
/***************************************************************************
* Global variables
***************************************************************************/
cy_stc_scb_uart_context_t UART_context;
cy_stc_smif_context_t SMIF_context;

/*******************************************************************************
* Function Name: SMIF_Interrupt_User
********************************************************************************
*
* The ISR for the SMIF interrupt. All Read/Write transfers to/from the external
* memory are processed inside the SMIF ISR.
*
*******************************************************************************/
void Flash_SMIF_Interrupt_User(void)
{
    Cy_SMIF_Interrupt(SMIF0, &QSPIContext);
}
cy_en_smif_status_t Flash_SMIF_QSPI_Start(void)
{
    cy_en_smif_status_t qspiStatus = CY_SMIF_BAD_PARAM;

    /* Configure SMIF interrupt */
    cy_stc_sysint_t smifIntConfig =
    {
#if (CY_CPU_CORTEX_M0P)
        .intrSrc = SMIF_NVIC_IRQN,
        .cm0pSrc = smif_interrupt_IRQn,
#else
        .intrSrc = smif_interrupt_IRQn, /* SMIF interrupt number (non M0 core)*/
#endif
        .intrPriority = SMIF_PRIORITY
    };

    /* enable interrupts */
    __enable_irq();

    /* SMIF interrupt initialization status */
    cy_en_sysint_status_t intr_init_status;
    intr_init_status = Cy_SysInt_Init(&smifIntConfig, Flash_SMIF_Interrupt_User);

    if(0 != intr_init_status)
    {
         BOOT_LOG_ERR("SMIF Interrupt initialization failed with error code %i", intr_init_status);
    }

    /*
     * cy_en_smif_status_t Cy_SMIF_Init(SMIF_Type *base,
                                    cy_stc_smif_config_t const *config,
                                    uint32_t timeout,
                                    cy_stc_smif_context_t *context)
    */
    qspiStatus = Cy_SMIF_Init(SMIF0, &QSPI_config, SMIF_TIMEOUT_1_MS, &QSPIContext);

    if(qspiStatus == CY_SMIF_SUCCESS)
    {
        Cy_SMIF_SetDataSelect(SMIF0, CY_SMIF_SLAVE_SELECT_0, CY_SMIF_DATA_SEL0);
        Cy_SMIF_Enable(SMIF0, &QSPIContext);
        BOOT_LOG_INF("SMIF Block Enabled\r\n");

        /* Map memory device to memory map */
        qspiStatus = Cy_SMIF_Memslot_Init(SMIF0, &smifBlockConfig, &QSPIContext);
    }

    if(qspiStatus == CY_SMIF_SUCCESS)
    {
        BOOT_LOG_INF("SMIF Normal Mode");
        /* Need to start from CommandMode as we will switch FlashMemory to Quad first */
        Cy_SMIF_SetMode(SMIF0, CY_SMIF_NORMAL);
    }
#if (CY_CPU_CORTEX_M0P)
	NVIC_EnableIRQ(SMIF_NVIC_IRQN);
#else
	NVIC_EnableIRQ(smif_interrupt_IRQn);
#endif
    return qspiStatus;
}

void Flash_SMIF_GetAddrBuff(uint32_t address, uint8_t * addrBuf)
{
    uint32_t numAddrBytes = smifMemConfigs[0]->deviceCfg->numOfAddrBytes;

    if(3u == numAddrBytes)
    {
        addrBuf[3] = 0x00;
        addrBuf[2] = CY_LO8(CY_LO16(address));
        addrBuf[1] = CY_HI8(CY_LO16(address));
        addrBuf[0] = CY_LO8(CY_HI16(address));
    }
    else
    if(4u == numAddrBytes)
    {
        addrBuf[3] = CY_LO8(CY_LO16(address));
        addrBuf[2] = CY_HI8(CY_LO16(address));
        addrBuf[1] = CY_LO8(CY_HI16(address));
        addrBuf[0] = CY_HI8(CY_HI16(address));
    }
}

/*******************************************************************************
* Function Name: Flash_SMIF_EnableQuadMode
****************************************************************************//**
*
* This function check if Quad mode is enabled and enable it is it is not enabled.
*
* \param baseaddr
* Holds the base address of the SMIF block registers.
*
* \param memConfig
* Memory device configuration
*
* \param smifContext
* The internal SMIF context data.
*
*******************************************************************************/
void Flash_SMIF_EnableQuadMode(SMIF_Type *baseaddr, cy_stc_smif_mem_config_t *memConfig,
                    cy_stc_smif_context_t const *smifContext)
{
    cy_en_smif_status_t status;
    uint8_t readStatus = 0;
    uint32_t statusCmd = memConfig->deviceCfg->readStsRegQeCmd->command;
    uint8_t maskQE = (uint8_t) memConfig->deviceCfg->stsRegQuadEnableMask;

    status = Cy_SMIF_Memslot_CmdReadSts(baseaddr, memConfig, &readStatus, statusCmd, smifContext);
    if(CY_SMIF_SUCCESS != status)
    {
        BOOT_LOG_ERR("Memory Module Read Status failed with error code %i", status);
    }
    else
    {
        if(maskQE != (readStatus & maskQE))
        {
            status = Cy_SMIF_Memslot_QuadEnable(baseaddr, memConfig, smifContext);
            if(CY_SMIF_SUCCESS == status)
            {
                while(Cy_SMIF_Memslot_IsBusy(baseaddr, memConfig, smifContext))
                {
                    /* Wait until the QE operation is completed */
                }
            }
            else
            {
                BOOT_LOG_ERR("Memory Module QuadEnable failed with error code %i", status);
            }
        }
    }
}


/*******************************************************************************
* Function Name: Flash_SMIF_ReadMemory
****************************************************************************//**
*
* This function reads data from the external memory in the quad mode.
* The function sends the Quad I/O Read: 0xEB command to the external memory.
*
* \param baseaddr
* Holds the base address of the SMIF block registers.
*
* \param smifContext
* The internal SMIF context data.
*
* \param rxBuffer
* The buffer for read data.
*
* \param rxSize
* The size of data to read.
*
* \param address
* The address to read data from.
*
*******************************************************************************/
int Flash_SMIF_ReadMemory(SMIF_Type *baseaddr,
                                cy_stc_smif_context_t *smifContext,
                                uint8_t rxBuffer[],
                                uint32_t rxSize,
                                uint8_t *address)
{
    cy_en_smif_status_t status;
    uint8_t rxBuffer_reg;
    cy_stc_smif_mem_device_cfg_t *device = smifMemConfigs[0]->deviceCfg;
    cy_stc_smif_mem_cmd_t *cmdreadStsRegQe = device->readStsRegQeCmd;

    /* Read data from the external memory configuration register */
    status = Cy_SMIF_Memslot_CmdReadSts(baseaddr, smifMemConfigs[0], &rxBuffer_reg, (uint8_t)cmdreadStsRegQe->command , smifContext);
    if(CY_SMIF_SUCCESS == status)
    {
        status = Cy_SMIF_Memslot_CmdRead(baseaddr, smifMemConfigs[0], address, rxBuffer, rxSize, NULL, smifContext);
        if(CY_SMIF_SUCCESS == status)
        {
            while(Cy_SMIF_BusyCheck(baseaddr))
            {
                /* Wait until the SMIF IP operation is completed. */
            }
        }
    }
    return (int)status;
}


/*******************************************************************************
* Function Name: Flash_SMIF_WriteMemory
********************************************************************************
*
* This function writes data to the external memory in the quad mode.
* The function sends the Quad Page Program: 0x38 command to the external memory.
*
* \param baseaddr
* Holds the base address of the SMIF block registers.
*
* \param smifContext
* The internal SMIF context data.
*
* \param txBuffer
* Data to write in the external memory.
*
* \param txSize
* The size of data.
*
* \param address
* The address to write data to.
*
*******************************************************************************/
int Flash_SMIF_WriteMemory(SMIF_Type *baseaddr,
                                cy_stc_smif_context_t *smifContext,
                                uint8_t txBuffer[],
                                uint32_t txSize,
                                uint8_t *address)
{
    cy_en_smif_status_t status;
    uint8_t rxBuffer_reg;
    cy_stc_smif_mem_device_cfg_t *device = smifMemConfigs[0]->deviceCfg;
    cy_stc_smif_mem_cmd_t *cmdreadStsRegQe = device->readStsRegQeCmd;
    cy_stc_smif_mem_cmd_t *cmdreadStsRegWip = device->readStsRegWipCmd;

    /* Read data from the external memory configuration register */
    status = Cy_SMIF_Memslot_CmdReadSts(baseaddr, smifMemConfigs[0], &rxBuffer_reg, (uint8_t)cmdreadStsRegQe->command , smifContext);

    if(CY_SMIF_SUCCESS == status)
    {
        /* Send Write Enable to external memory */
        status = Cy_SMIF_Memslot_CmdWriteEnable(baseaddr, smifMemConfigs[0], smifContext);
    }
    if(CY_SMIF_SUCCESS == status)
    {
        /* Quad Page Program command */
        status = Cy_SMIF_Memslot_CmdProgram(baseaddr, smifMemConfigs[0], address, txBuffer, txSize, NULL, smifContext);
    }
    if(CY_SMIF_SUCCESS == status)
    {
        while(Cy_SMIF_Memslot_IsBusy(baseaddr, (cy_stc_smif_mem_config_t*)smifMemConfigs[0], smifContext))
        {
            /* Wait until the operation is completed */
        }
        /* Read data from the external memory status register */
        status = Cy_SMIF_Memslot_CmdReadSts(baseaddr, smifMemConfigs[0], &rxBuffer_reg,
                                 (uint8_t)cmdreadStsRegWip->command , smifContext);
    }
    return (int)status;
}


/*******************************************************************************
* Function Name: Flash_SMIF_EraseMemory
********************************************************************************
*
* Erase block of external memory
*
* \param baseaddr
* Holds the base address of the SMIF block registers.
*
* \param memConfig configuration of external memory
*
* \param address
* The address to write data to.
*
* \param smifContext
* The internal SMIF context data.
*
*******************************************************************************/
int Flash_SMIF_EraseMemory(SMIF_Type *baseaddr, cy_stc_smif_mem_config_t *memConfig,
                                 uint8_t *address,
                                 cy_stc_smif_context_t const *smifContext)
{
    cy_en_smif_status_t status;

    status = Cy_SMIF_Memslot_CmdWriteEnable(baseaddr, memConfig, smifContext);
    if(CY_SMIF_SUCCESS == status)
    {
        status = Cy_SMIF_Memslot_CmdSectorErase(baseaddr, memConfig, address, smifContext);
        /* Wait until the memory is erased */
        while(Cy_SMIF_Memslot_IsBusy(baseaddr, memConfig, smifContext))
        {
            /* Wait until the Erase operation is completed */
        }
    }
    return (int)status;
}
