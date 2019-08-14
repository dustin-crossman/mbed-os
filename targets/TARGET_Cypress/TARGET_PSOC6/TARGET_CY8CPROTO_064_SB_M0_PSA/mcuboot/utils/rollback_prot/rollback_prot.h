/***************************************************************************//**
* \file rollback_prot.h
* \version 1.0
*
* \brief
*  This is the header file complementing the source for rollback protection
*  feature implementation.
*
********************************************************************************
* \copyright
* Copyright 2018, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _ROLLBACK_PROT_H_
#define _ROLLBACK_PROT_H_

int cy_compare_rollback_counter(struct image_header *ih);
int cy_write_rollback_counter(struct image_header *ih);
int Cy_ReadRollbackCounter(uint8_t counter_n, uint32_t *value);
int Cy_UpdateRollbackCounter(uint8_t counter_n, uint32_t value);

#endif
