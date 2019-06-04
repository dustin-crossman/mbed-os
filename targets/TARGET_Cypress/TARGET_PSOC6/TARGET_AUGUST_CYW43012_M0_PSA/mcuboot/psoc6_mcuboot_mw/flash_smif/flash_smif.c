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

#include "cy_pdl.h"
#include "cycfg.h"
#include "cycfg_qspi_memslot.h"
#include "flash_smif.h"

/***************************************************************************
* Global constants
***************************************************************************/

#define SMIF_PRIORITY       (1u)      /* SMIF interrupt priority */
#define TIMEOUT_1_MS        (1000ul)  /* 1 ms timeout for all blocking functions */
#define PACKET_SIZE         (64u)    /* The memory Read/Write packet */
#define ADDRESS_SIZE        (3u)      /* Memory address size */
#define SUCCESS             (0u)
/***************************************************************************
* Global variables
***************************************************************************/
cy_stc_scb_uart_context_t UART_context;
cy_stc_smif_context_t SMIF_context;

/*******************************************************************************
* Function Name: handle_error
********************************************************************************
*
* This function processes unrecoverable errors
*
*******************************************************************************/
void handle_error(void)
{
     /* Disable all interrupts */
    __disable_irq();
    while(1u) {}
}


#ifdef MCUBOOT_USE_SMIF_STAGE
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
    Cy_SMIF_Interrupt(SMIF_HW, &SMIF_context);
}
#endif


/*******************************************************************************
* Function Name: InitBuffers
****************************************************************************//**
*
* This function initializes the transfer buffers.
*
* \param txBuffer - The buffer for Write data.
*
* \param rxBuffer - The buffer for Read data.
*
*******************************************************************************/
void InitBuffers(uint8_t txBuffer[],uint8_t rxBuffer[], uint32_t bufferSize)
{
    for(uint32_t index=0; index<bufferSize; index++)
    {
        txBuffer[index] = (uint8_t) (index & 0xFF);
        rxBuffer[index] = 0;
    }
}


/*******************************************************************************
* Function Name: PrintArray
****************************************************************************//**
*
* This function prints the content of the RX buffer to the UART console.
*
* \param msg - message print before array output
*
* \param  rxBuffer - The buffer to the console output.
*
* \param  size - The size of the buffer to the console output.
*
*******************************************************************************/
void PrintArray(char * msg, uint8_t * buff, uint32_t size)
{
    printf("%s", msg);
    for(uint32_t index=0; index<size; index++)
    {
        printf("0x%02X ", (unsigned int) buff[index]);
    }
    printf("\r\n=======================\r\n");
}

/*******************************************************************************
* Function Name: CheckStatus
****************************************************************************//**
*
* Check if status is SUCCES and call handle error function
*
*******************************************************************************/
void CheckStatus(char * msg, uint32_t status)
{
    if(SUCCESS != status)
    {
        printf("%s", msg);
        handle_error();
    }
}

cy_en_smif_status_t Flash_SMIF_QSPI_Start(void)
{
    cy_en_smif_status_t qspiStatus = CY_SMIF_BAD_PARAM;

    qspiStatus = Cy_SMIF_Init(SMIF0, &SMIF_config, TIMEOUT_1_MS, &QSPIContext);

    if(qspiStatus == CY_SMIF_SUCCESS)
    {
        Cy_SMIF_SetDataSelect(SMIF0, CY_SMIF_SLAVE_SELECT_0, CY_SMIF_DATA_SEL0);
        Cy_SMIF_Enable(SMIF0, &QSPIContext);
        BOOT_LOG_INF("SMIF Block Enabled\r\n");
//    }
//
//    if(qspiStatus == CY_SMIF_SUCCESS)
//    {
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
//    cy_en_smif_status_t status;
//    uint8_t readStatus = 0;
//    uint32_t statusCmd = memConfig->deviceCfg->readStsRegQeCmd->command;
//    uint8_t maskQE = (uint8_t) memConfig->deviceCfg->stsRegQuadEnableMask;
//
//    status = Cy_SMIF_Memslot_CmdReadSts(baseaddr, memConfig, &readStatus, statusCmd,
//                                        smifContext);
//    CheckStatus("\r\n\r\nSMIF Cy_SMIF_Memslot_QuadEnable failed\r\n", status);
//
//    if(maskQE != (readStatus & maskQE))
//    {
//        /* Check user confirmation to do QE*/
//        printf("\r\nQuad is NOT enabled. Pleas press button to continue\r\n");
//        bool btnPressed = false;
//        while (!btnPressed)
//        {   /* 50 ms delay for button press check*/
//            Cy_SysLib_Delay(50u);
//            if (Cy_GPIO_Read(PIN_SW2_PORT, PIN_SW2_PIN) == 0u)
//            {
//                /* 50 ms delay for button debounce on button press */
//                Cy_SysLib_Delay(50u);
//                if (Cy_GPIO_Read(PIN_SW2_PORT, PIN_SW2_PIN) == 0u)
//                {
//                    while (Cy_GPIO_Read(PIN_SW2_PORT, PIN_SW2_PIN) == 0u)
//                    {   /* 50 ms delay for button debounce on button release */
//                        Cy_SysLib_Delay(50u);
//                    }
//                    btnPressed = true;
//                }
//
//            }
//        }
//
//        status = Cy_SMIF_Memslot_QuadEnable(baseaddr, memConfig, smifContext);
//        CheckStatus("\r\n\r\nSMIF Cy_SMIF_Memslot_QuadEnable failed\r\n", status);
//
//        while(Cy_SMIF_Memslot_IsBusy(baseaddr, memConfig, smifContext))
//        {
//            /* Wait until the QE operation is completed */
//        }
//    }
//    else
//    {
//        printf("\r\nQuad is enabled. Enabling operation skipped\r\n");
//    }
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
//    CheckStatus("\r\n\r\nSMIF Cy_SMIF_Memslot_CmdReadSts failed\r\n", status);
//
//    printf("Received Data: 0x%X\r\n", (unsigned int) rxBuffer_reg);
//    printf("\r\nQuad I/O Read (QIOR 0x%0X) \r\n", 0x38);

    if(CY_SMIF_SUCCESS == status)
    {
        /* The 4 Page program command */
        // TODO: this uses non-blocking API
        status = Cy_SMIF_Memslot_CmdRead(baseaddr, smifMemConfigs[0], address, rxBuffer, rxSize, NULL, smifContext);
//    CheckStatus("\r\n\r\nSMIF Cy_SMIF_Memslot_CmdRead failed\r\n",status);

        /* Wait until receive transaction completed */
        while(Cy_SMIF_GetTxfrStatus(SMIF0, &QSPIContext) != CY_SMIF_REC_CMPLT);
    }
    /* Send received data to the console */
//    PrintArray("Received Data: ",rxBuffer, rxSize);
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
//    CheckStatus("\r\n\r\nSMIF Cy_SMIF_Memslot_CmdReadSts failed\r\n", status);

//    printf("Received Data: 0x%X\r\n", (unsigned int) rxBuffer_reg);

    /* Send Write Enable to external memory */
        status = Cy_SMIF_Memslot_CmdWriteEnable(baseaddr, smifMemConfigs[0], smifContext);
    }
//    CheckStatus("\r\n\r\nSMIF Cy_SMIF_Memslot_CmdWriteEnable failed\r\n", status);

//    printf("\r\nQuad Page Program (QPP 0x%0X) \r\n", 0x38);

    if(CY_SMIF_SUCCESS == status)
    {
        /* Quad Page Program command */
        status = Cy_SMIF_Memslot_CmdProgram(baseaddr, smifMemConfigs[0], address, txBuffer, txSize, NULL, smifContext);
//    CheckStatus("\r\n\r\nSMIF Cy_SMIF_Memslot_CmdProgram failed\r\n", status);
//
//    PrintArray("Written Data: ", txBuffer, txSize);
    }

    if(CY_SMIF_SUCCESS == status)
    {
        while(Cy_SMIF_Memslot_IsBusy(baseaddr, (cy_stc_smif_mem_config_t*)smifMemConfigs[0], smifContext))
        {
            /* Wait until the Erase operation is completed */
        }
        /* Read data from the external memory status register */
        status = Cy_SMIF_Memslot_CmdReadSts(baseaddr, smifMemConfigs[0], &rxBuffer_reg,
                                 (uint8_t)cmdreadStsRegWip->command , smifContext);
    }
//    CheckStatus("\r\n\r\nSMIF ReadStatusReg failed\r\n", status);
//
//    printf("Received Data: 0x%X\r\n", (unsigned int) rxBuffer_reg);
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
//    CheckStatus("\r\n\r\nSMIF Cy_SMIF_Memslot_CmdWriteEnable failed\r\n", status);
        status = Cy_SMIF_Memslot_CmdSectorErase(baseaddr, memConfig, address, smifContext);

//    CheckStatus("\r\n\r\nSMIF Cy_SMIF_Memslot_CmdSectorErase failed\r\n", status);

        /* Wait until the memory is erased */
        while(Cy_SMIF_Memslot_IsBusy(baseaddr, memConfig, smifContext))
        {
            /* Wait until the Erase operation is completed */
        }
    }
    return (int)status;
}
