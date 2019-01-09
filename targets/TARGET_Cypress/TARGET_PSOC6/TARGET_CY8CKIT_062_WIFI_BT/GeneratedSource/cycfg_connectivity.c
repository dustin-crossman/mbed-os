/*******************************************************************************
* File Name: cycfg_connectivity.h
*
* Description:
* Establishes all necessary connections between hardware elements.
* This file should not be modified. It was automatically generated by 
* ModusToolbox 1.0.0
* 
********************************************************************************
* Copyright (c) 2017-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cy_device_headers.h"

#include "stdbool.h"

#include "cy_trigmux.h"

#include "cycfg_connectivity.h"

void init_cycfg_connectivity(void)
{
	Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP14_OUTPUT0, TRIG0_OUT_CPUSS_DW0_TR_IN0, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP14_OUTPUT4, TRIG0_OUT_CPUSS_DW0_TR_IN1, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG1_IN_TR_GROUP14_OUTPUT1, TRIG1_OUT_CPUSS_DW1_TR_IN1, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG1_IN_TR_GROUP14_OUTPUT7, TRIG1_OUT_CPUSS_DW1_TR_IN3, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB0, TRIG14_OUT_TR_GROUP1_INPUT44, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB1, TRIG14_OUT_TR_GROUP0_INPUT47, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB3, TRIG14_OUT_TR_GROUP0_INPUT43, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB7, TRIG14_OUT_TR_GROUP1_INPUT50, false, TRIGGER_TYPE_LEVEL);
	HSIOM->AMUX_SPLIT_CTL[4] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk |
		HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk |
		HSIOM_AMUX_SPLIT_CTL_SWITCH_AA_SL_Msk |
		HSIOM_AMUX_SPLIT_CTL_SWITCH_AA_SR_Msk;
	HSIOM->AMUX_SPLIT_CTL[2] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk |
		HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk |
		HSIOM_AMUX_SPLIT_CTL_SWITCH_AA_SL_Msk |
		HSIOM_AMUX_SPLIT_CTL_SWITCH_AA_SR_Msk;
}
