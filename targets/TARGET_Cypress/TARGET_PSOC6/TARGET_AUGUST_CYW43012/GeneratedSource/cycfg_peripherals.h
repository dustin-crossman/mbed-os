/*******************************************************************************
* File Name: cycfg_peripherals.h
*
* Description:
* Peripheral Hardware Block configuration
* This file was automatically generated and should not be modified.
* 
********************************************************************************
* Copyright 2017-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
********************************************************************************/

#if !defined(CYCFG_PERIPHERALS_H)
#define CYCFG_PERIPHERALS_H

#include "cycfg_notices.h"
#include "cy_i2s.h"
#include "cy_sysclk.h"
#include "cy_sysanalog.h"
#include "cy_ctb.h"
#include "cy_sar.h"
#include "cy_scb_uart.h"
#include "cy_scb_i2c.h"
#include "cy_smif.h"
#include "cy_tcpwm_pwm.h"
#include "cycfg_routing.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define I2S_HW I2S0
#define I2S_IRQ audioss_0_interrupt_i2s_IRQn
#define OPAMP0_HW CTBM0
#define OPAMP1_HW CTBM0
#define ADC_HW SAR
#define ADC_IRQ pass_interrupt_sar_IRQn
#define ADC_CTRL (CY_SAR_VREF_PWR_100 | CY_SAR_VREF_SEL_BGR | CY_SAR_BYPASS_CAP_ENABLE | CY_SAR_NEG_SEL_VSSA_KELVIN | CY_SAR_CTRL_NEGVREF_HW | CY_SAR_CTRL_COMP_DLY_12 | CY_SAR_COMP_PWR_100 | CY_SAR_DEEPSLEEP_SARMUX_OFF | CY_SAR_SARSEQ_SWITCH_ENABLE)
#define ADC_SAMPLE (SAR_SAMPLE_CTRL_EOS_DSI_OUT_EN_Msk | CY_SAR_RIGHT_ALIGN | CY_SAR_DIFFERENTIAL_SIGNED | CY_SAR_SINGLE_ENDED_SIGNED | CY_SAR_AVG_CNT_2 | CY_SAR_AVG_MODE_SEQUENTIAL_FIXED | CY_SAR_TRIGGER_MODE_FW_ONLY)
#define ADC_CH0_CONFIG (CY_SAR_POS_PORT_ADDR_SARMUX | CY_SAR_CHAN_POS_PIN_ADDR_1 | CY_SAR_CHAN_SINGLE_ENDED | CY_SAR_CHAN_AVG_DISABLE | CY_SAR_CHAN_SAMPLE_TIME_0)
#define ADC_CH1_CONFIG (CY_SAR_POS_PORT_ADDR_SARMUX | CY_SAR_CHAN_POS_PIN_ADDR_2 | CY_SAR_CHAN_SINGLE_ENDED | CY_SAR_CHAN_AVG_DISABLE | CY_SAR_CHAN_SAMPLE_TIME_1)
#define ADC_MUX_SWITCH (CY_SAR_MUX_FW_VSSA_VMINUS | CY_SAR_MUX_FW_P1_VPLUS | CY_SAR_MUX_FW_P2_VPLUS)
#define ADC_MUX_SWITCH_HW_CTRL (CY_SAR_MUX_SQ_CTRL_VSSA  | CY_SAR_MUX_SQ_CTRL_P1 | CY_SAR_MUX_SQ_CTRL_P2)
#define ADC_VREF_MV 1200UL
#define BT_UART_HW SCB2
#define BT_UART_IRQ scb_2_interrupt_IRQn
#define UART_HW SCB6
#define UART_IRQ scb_6_interrupt_IRQn
#define I2C_HW SCB7
#define I2C_IRQ scb_7_interrupt_IRQn
#define QSPI_HW SMIF0
#define QSPI_IRQ smif_interrupt_IRQn
#define QSPI_MEMORY_MODE_ALIGMENT_ERROR (0UL)
#define QSPI_RX_DATA_FIFO_UNDERFLOW (0UL)
#define QSPI_TX_COMMAND_FIFO_OVERFLOW (0UL)
#define QSPI_TX_DATA_FIFO_OVERFLOW (0UL)
#define QSPI_RX_FIFO_TRIGEER_LEVEL (0UL)
#define QSPI_TX_FIFO_TRIGEER_LEVEL (0UL)
#define QSPI_DATALINES0_1 (1UL)
#define QSPI_DATALINES2_3 (1UL)
#define QSPI_DATALINES4_5 (0UL)
#define QSPI_DATALINES6_7 (0UL)
#define QSPI_SS0 (1UL)
#define QSPI_SS1 (0UL)
#define QSPI_SS2 (0UL)
#define QSPI_SS3 (0UL)
#define QSPI_DESELECT_DELAY 7
#define MOT_PWM_1_HW TCPWM1
#define MOT_PWM_1_NUM 16UL
#define MOT_PWM_1_MASK (1UL << 16)
#define MOT_PWM_2_HW TCPWM1
#define MOT_PWM_2_NUM 18UL
#define MOT_PWM_2_MASK (1UL << 18)

extern const cy_stc_i2s_config_t I2S_config;
extern const cy_stc_sysanalog_config_t pass_0_aref_0_config;
extern const cy_stc_ctb_opamp_config_t OPAMP0_config;
extern const cy_stc_ctb_opamp_config_t OPAMP1_config;
extern const cy_stc_sar_config_t ADC_config;
extern const cy_stc_scb_uart_config_t BT_UART_config;
extern const cy_stc_scb_uart_config_t UART_config;
extern const cy_stc_scb_i2c_config_t I2C_config;
extern const cy_stc_smif_config_t QSPI_config;
extern const cy_stc_tcpwm_pwm_config_t MOT_PWM_1_config;
extern const cy_stc_tcpwm_pwm_config_t MOT_PWM_2_config;

void init_cycfg_peripherals(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PERIPHERALS_H */
