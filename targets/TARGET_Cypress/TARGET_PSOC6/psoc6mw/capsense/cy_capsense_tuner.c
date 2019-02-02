/***************************************************************************//**
* \file cy_capsense_tuner.c
* \version 1.1
*
* \brief
* This file provides the source code for the Tuner module functions.
*
********************************************************************************
* \copyright
* Copyright 2018-2019, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_syslib.h"
#include "cy_capsense_structure.h"
#include "cy_capsense_tuner.h"
#include "cy_capsense_control.h"
#include "cy_capsense_common.h"


/*******************************************************************************
* Function Name: Cy_CapSense_TuInitialize
****************************************************************************//**
*
* Initializes the communication interface with the CapSense Tuner tool.
*
*******************************************************************************/
void Cy_CapSense_TuInitialize(cy_stc_capsense_context_t * context)
{
    volatile cy_stc_capsense_common_context_t * ptrCommonCxt = context->ptrCommonContext;
    ptrCommonCxt->tunerCmd = (uint16_t)CY_CAPSENSE_TU_CMD_NONE_E;
    ptrCommonCxt->tunerSt = (uint8_t)CY_CAPSENSE_TU_FSM_RUNNING;
}

/*******************************************************************************
* Function Name: Cy_CapSense_RunTuner
****************************************************************************//**
*
* Establishes synchronized operation between the CapSense middleware and 
* the CapSense Tuner tool.
*
* This function should be called periodically in the application program to 
* serve the CapSense Tuner tool requests and commands to synchronize the 
* operation, however there is no specific timing requirement for this function. 
* In most cases, the best place to call this function is after processing 
* and before next scanning.
* 
* If a user changes some parameters in the Tuner tool, a re-initialization 
* of the middleware is required. In such cases, the tuner issues a re-initialize 
* command and which is executed by this function.
* 
* If this function is not called by the application program, the middleware 
* operation is asynchronous to the Tuner tool and the following disadvantages 
* are applicable:
*
* * The raw counts displayed in the CapSense Tuner tool may be filtered 
*   and/or non-filtered. As a result, noise and SNR measurements will 
*   not be accurate.
* * The CapSense Tuner tool may read the sensor data such as raw counts 
*   from a scan multiple times. As a result, noise and SNR measurement 
*   will not be accurate.
* * The CapSense Tuner tool and Host controller should not change 
*   the parameters via the tuner interface. Changing the parameters via 
*   the Tuner interface in the async mode will result in abnormal behavior.
* * Displaying detected gestures may be missed.
*
* \note 
* Calling this function is not mandatory and required only  
* synchronizing the communication with the CapSense Tuner tool when the Tuner 
* is used.
*
* \warning 
* This function executes received commands. Two commands 
* CY_CAPSENSE_TU_CMD_ONE_SCAN_E and CY_CAPSENSE_TU_CMD_SUSPEND_E change the 
* FW tuner module state to suspend. In this state, the function waits until 
* CY_CAPSENSE_TU_CMD_RESUME_E is received. A callback mechanism of command 
* receiving should be used to avoid FW hang.
* 
* \param context
* The pointer to the CapSense context structure \ref cy_stc_capsense_context_t.
*
* \return
* Returns a status of whether a re-initialization of the middleware was 
* executed by this function or not:
* - CY_CAPSENSE_STATUS_RESTART_DONE - Based on a received command, the
* CapSense was re-initialized.
* - CY_CAPSENSE_STATUS_RESTART_NONE - No re-initialization was executed by this
* function.
*
* \funcusage
* 
* An example of synchronization with the Tuner tool using EzI2C:
* \snippet capsense\1.1\snippet\main.c snippet_Cy_CapSense_Tuner_EzI2C
* 
* An example of synchronization with the Tuner tool using UART:
* \snippet capsense\1.1\snippet\main.c snippet_Cy_CapSense_Tuner_UART
* 
*******************************************************************************/
uint32_t Cy_CapSense_RunTuner(cy_stc_capsense_context_t * context)
{
    uint32_t interruptState;
    uint32_t updateFlag = 0uL;
    uint32_t tunerStatus = CY_CAPSENSE_STATUS_RESTART_NONE;
    uint16_t tunerCommand;
    uint32_t cmdOffset;
    uint32_t cmdSize;
    uint8_t cmdCounter;

    uint8_t * tunerStructure = NULL;
    uint8_t * commandPacket = NULL;
    volatile cy_stc_capsense_common_context_t * ptrCommonCxt = context->ptrCommonContext;
    uint8_t tunerState = ptrCommonCxt->tunerSt;

    cy_capsense_tuner_send_callback_t sendCallback = context->ptrCommonContext->ptrTunerSendCallback;
    cy_capsense_tuner_receive_callback_t receiveCallback = context->ptrCommonContext->ptrTunerReceiveCallback;

    do
    {
        /* 
        * ONE_SCAN command could be interpreted as two commands:
        * RESUME till next call of this function and then
        * SUSPEND till next command receiving.
        * So, after one scan cycle tuner state is changed to suspend.
        */
        if ((uint8_t)CY_CAPSENSE_TU_FSM_ONE_SCAN == tunerState)
        {
            interruptState = Cy_SysLib_EnterCriticalSection();
            context->ptrCommonContext->tunerCmd = (uint16_t)CY_CAPSENSE_TU_CMD_SUSPEND_E;
            Cy_SysLib_ExitCriticalSection(interruptState);
        }

        /* Send Data to the CapSense Tuner tool */
        if(NULL != sendCallback)
        {
            sendCallback((void *)context);
        }

        /* Command can come from EzI2C by direct writing into data structure */
        tunerCommand = context->ptrCommonContext->tunerCmd;
        cmdCounter = context->ptrCommonContext->tunerCnt + 1u;

        /* Call user's callback function if it is registered */
        if (NULL != receiveCallback)
        {
            commandPacket = NULL;
            tunerStructure = NULL;
            receiveCallback(&commandPacket, &tunerStructure, context);

            /* If command exists and is correct then read command */
            if ((NULL != commandPacket) || (NULL != tunerStructure))
            {
                if (CY_CAPSENSE_COMMAND_OK == Cy_CapSense_CheckCommandIntegrity(commandPacket))
                {
                    tunerCommand = commandPacket[CY_CAPSENSE_COMMAND_CODE_0_IDX];
                    ptrCommonCxt->tunerCmd = tunerCommand;
                    cmdCounter = commandPacket[CY_CAPSENSE_COMMAND_CNTR_0_IDX];
                }
            }
        }

        /* Check command register */
        switch (tunerCommand)
        {
        case (uint16_t)CY_CAPSENSE_TU_CMD_SUSPEND_E:
            tunerState = (uint8_t)CY_CAPSENSE_TU_FSM_SUSPENDED;
            updateFlag = 1u;
            break;

        case (uint16_t)CY_CAPSENSE_TU_CMD_RESUME_E:
            tunerState = (uint8_t)CY_CAPSENSE_TU_FSM_RUNNING;
            updateFlag = 1u;
            break;

        case (uint16_t)CY_CAPSENSE_TU_CMD_RESTART_E:
            (void)Cy_CapSense_Enable(context);
            tunerStatus = CY_CAPSENSE_STATUS_RESTART_DONE;
            tunerState = (uint8_t)CY_CAPSENSE_TU_FSM_RUNNING;
            updateFlag = 1u;
            break;

        case (uint16_t)CY_CAPSENSE_TU_CMD_PING_E:
            tunerState = (uint8_t)CY_CAPSENSE_TU_FSM_RUNNING;
            updateFlag = 1u;
            break;

        case (uint16_t)CY_CAPSENSE_TU_CMD_ONE_SCAN_E:
            tunerState = (uint8_t)CY_CAPSENSE_TU_FSM_ONE_SCAN;
            updateFlag = 0u;
            break;

        case (uint16_t)CY_CAPSENSE_TU_CMD_WRITE_E:
            /* Tuner state is not changed */
            cmdOffset = (uint32_t)((uint32_t)commandPacket[CY_CAPSENSE_COMMAND_OFFS_0_IDX] << CY_CAPSENSE_MSB_SHIFT) |
                                   (uint32_t)commandPacket[CY_CAPSENSE_COMMAND_OFFS_1_IDX];
            cmdSize = commandPacket[CY_CAPSENSE_COMMAND_SIZE_0_IDX];

            if (1u == cmdSize)
            {
                tunerStructure[cmdOffset] = commandPacket[CY_CAPSENSE_COMMAND_DATA_0_IDX + 3u];
            }
            else if (2u == cmdSize)
            {
                tunerStructure[cmdOffset + 1u] = commandPacket[CY_CAPSENSE_COMMAND_DATA_0_IDX + 2u];
                tunerStructure[cmdOffset + 0u] = commandPacket[CY_CAPSENSE_COMMAND_DATA_0_IDX + 3u];
            }
            else
            {
                tunerStructure[cmdOffset + 3u] = commandPacket[CY_CAPSENSE_COMMAND_DATA_0_IDX + 0u];
                tunerStructure[cmdOffset + 2u] = commandPacket[CY_CAPSENSE_COMMAND_DATA_0_IDX + 1u];
                tunerStructure[cmdOffset + 1u] = commandPacket[CY_CAPSENSE_COMMAND_DATA_0_IDX + 2u];
                tunerStructure[cmdOffset + 0u] = commandPacket[CY_CAPSENSE_COMMAND_DATA_0_IDX + 3u];
            }

            updateFlag = 1u;
            break;

        default:
            break;
        }

        ptrCommonCxt->tunerSt = tunerState;

        /* Set Complete flag in command register if needed */
        if (0uL != updateFlag)
        {
            interruptState = Cy_SysLib_EnterCriticalSection();
            /* Check that command wasn't overwritten with new command */
            if (tunerCommand == ptrCommonCxt->tunerCmd)
            {
                ptrCommonCxt->tunerCmd |= CY_CAPSENSE_TU_CMD_COMPLETE_BIT;
                ptrCommonCxt->tunerCnt = cmdCounter;
            }
            Cy_SysLib_ExitCriticalSection(interruptState);
            updateFlag = 0uL;
        }
        tunerStatus = 0u;

    } while ((uint8_t)CY_CAPSENSE_TU_FSM_SUSPENDED == tunerState);
        
    return tunerStatus;
}


/*******************************************************************************
* Function Name: Cy_CapSense_CheckCommandIntegrity
****************************************************************************//**
*
* Checks command format, header, tail, CRC, etc.
*
* This function checks whether the specified packet with the size
* CY_CAPSENSE_COMMAND_PACKET_SIZE could be represented as a
* command received from the CapSense Tuner tool.
* The verification includes the following items:
* * Header
* * Tail
* * CRC
* * Command code
*
* Command format is the following:
* * Byte  0: Header 0 = 0x0D
* * Byte  1: Header 1 = 0x0A
* * Byte  2: Command code MSB = cy_en_capsense_tuner_cmd_t
* * Byte  3: Command code LSB = cy_en_capsense_tuner_cmd_t
* * Byte  4: Command counter
* * Byte  5: Size = either 1, 2 or 4
* * Byte  6: Offset MSB
* * Byte  7: Offset LSB
* * Byte  8: Data MSB
* * Byte  9: Data
* * Byte 10: Data
* * Byte 11: Data LSB
* * Byte 12: 8-bit CRC
* * Byte 13: Tail 0 = 0x00
* * Byte 14: Tail 1 = 0xFF
* * Byte 15: Tail 2 = 0xFF
*
* \param commandPacket
* The pointer to the data packet that should be verified.
*
* \return
* Returns the result of the command verification:
* - CY_CAPSENSE_COMMAND_OK - Command is correct.
* - CY_CAPSENSE_WRONG_HEADER - Wrong header.
* - CY_CAPSENSE_WRONG_TAIL - Wrong tail.
* - CY_CAPSENSE_WRONG_CRC - Wrong CRC.
* - CY_CAPSENSE_WRONG_CODE - Wrong Command code.
*
*******************************************************************************/
uint32_t Cy_CapSense_CheckCommandIntegrity(const uint8_t * commandPacket)
{
    uint32_t cmdCheckStatus = CY_CAPSENSE_COMMAND_OK;
    uint16_t crcValue;

    if (CY_CAPSENSE_COMMAND_HEAD_0 != commandPacket[CY_CAPSENSE_COMMAND_HEAD_0_IDX])
    {
        cmdCheckStatus = CY_CAPSENSE_WRONG_HEADER;
    }
    else if (CY_CAPSENSE_COMMAND_HEAD_1 != commandPacket[CY_CAPSENSE_COMMAND_HEAD_1_IDX])
    {
        cmdCheckStatus = CY_CAPSENSE_WRONG_HEADER;
    }
    else if (CY_CAPSENSE_COMMAND_TAIL_0 != commandPacket[CY_CAPSENSE_COMMAND_TAIL_0_IDX])
    {
        cmdCheckStatus = CY_CAPSENSE_WRONG_TAIL;
    }
    else if (CY_CAPSENSE_COMMAND_TAIL_1 != commandPacket[CY_CAPSENSE_COMMAND_TAIL_1_IDX])
    {
        cmdCheckStatus = CY_CAPSENSE_WRONG_TAIL;
    }
    else if (CY_CAPSENSE_COMMAND_TAIL_2 != commandPacket[CY_CAPSENSE_COMMAND_TAIL_2_IDX])
    {
        cmdCheckStatus = CY_CAPSENSE_WRONG_TAIL;
    }
    else if (((uint8_t)CY_CAPSENSE_TU_CMD_WRITE_E) < commandPacket[CY_CAPSENSE_COMMAND_CODE_0_IDX])
    {
        cmdCheckStatus = CY_CAPSENSE_WRONG_CODE;
    }
    else
    {
        crcValue = (uint16_t)((uint16_t)commandPacket[CY_CAPSENSE_COMMAND_CRC_0_IDX] << CY_CAPSENSE_MSB_SHIFT);
        crcValue |= (uint16_t)commandPacket[CY_CAPSENSE_COMMAND_CRC_1_IDX];
        if (crcValue != Cy_CapSense_CalculateCrc16(&commandPacket[0u], CY_CAPSENSE_COMMAND_CRC_DATA_SIZE))
        {
            cmdCheckStatus = CY_CAPSENSE_WRONG_CRC;
        }
        else
        {
            cmdCheckStatus = CY_CAPSENSE_COMMAND_OK;
        }
    }

    return cmdCheckStatus;
}


/*******************************************************************************
* Function Name: Cy_CapSense_CalculateCrc16
****************************************************************************//**
*
* Calculates CRC for the specified buffer and length. CRC Poly: 0xAC9A
*
* This API is used for the CRC protection of a packet received from 
* the CapSense Tuner tool. CRC polynomial is 0xAC9A. It has a Hamming 
* distance 5 for data words up to 241 bits.
*
* Reference:  "P. Koopman, T. Chakravarthy,
* "Cyclic Redundancy Code (CRC) Polynomial Selection for Embedded Networks",
* The International Conference on Dependable Systems and Networks, DSN-2004"
*
* \param ptrData
* The pointer to the data.
*
* \param len
* The length of the data in bytes.
*
* \return
* Returns a calculated CRC-16 value.
*
*******************************************************************************/
uint16 Cy_CapSense_CalculateCrc16(const uint8_t *ptrData, uint32_t len)
{
    uint32_t idx;
    uint32_t actualCrc = 0u;
    const uint16_t crcTable[] =
    {
        0x0000u, 0xAC9Au, 0xF5AEu, 0x5934u, 0x47C6u, 0xEB5Cu, 0xB268u, 0x1EF2u,
        0x8F8Cu, 0x2316u, 0x7A22u, 0xD6B8u, 0xC84Au, 0x64D0u, 0x3DE4u, 0x917Eu
    };

    for (;len-- > 0u;)
    {
        /* Process HI Nibble */
        idx = ((actualCrc >> 12u) ^ (((uint32)*ptrData) >> 4u)) & 0xFLu;
        actualCrc = crcTable[idx] ^ (actualCrc << 4u);

        /* Process LO Nibble */
        idx = ((actualCrc >> 12u) ^ (uint32)*ptrData) & 0xFLu;
        actualCrc = crcTable[idx] ^ (actualCrc << 4u);

        ptrData++;
    }

    return (uint16_t)actualCrc;
}


/* [] END OF FILE */
