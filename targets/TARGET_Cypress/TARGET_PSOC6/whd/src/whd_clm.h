/*
 * $ Copyright Cypress Semiconductor Apache2 $
 */

/** @file
 *  Provides generic clm blob file download functionality
 */
#ifndef _WHD_INTERNAL_CLM_H_
#define _WHD_INTERNAL_CLM_H_

#include "whd.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
*                      Macros
******************************************************/


/******************************************************
*             Function prototypes
******************************************************/
whd_result_t whd_process_clm_data(whd_interface_t ifp);

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif /* ifndef _WHD_INTERNAL_CLM_H_ */

