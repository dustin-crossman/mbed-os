/*******************************************************************************
* File Name: cycfg_peripherals.c
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

#include "cycfg_peripherals.h"

#define MOT_PWM_1_INPUT_DISABLED 0x7U
#define MOT_PWM_2_INPUT_DISABLED 0x7U

const cy_stc_i2s_config_t I2S_config = 
{
	.txEnabled = true,
	.rxEnabled = false,
	.txDmaTrigger = false,
	.rxDmaTrigger = false,
	.clkDiv = 36,
	.extClk = false,
	.txMasterMode = 1,
	.txAlignment = CY_I2S_I2S_MODE,
	.txWsPulseWidth = CY_I2S_WS_ONE_CHANNEL_LENGTH,
	.txWatchdogEnable = false,
	.txWatchdogValue = 4294967295,
	.txSdoLatchingTime = 0,
	.txSckoInversion = 0,
	.txSckiInversion = 0,
	.txChannels = 2,
	.txChannelLength = CY_I2S_LEN16,
	.txWordLength = CY_I2S_LEN16,
	.txOverheadValue = CY_I2S_OVHDATA_ZERO,
	.txFifoTriggerLevel = 0,
	.rxMasterMode = 1,
	.rxAlignment = CY_I2S_I2S_MODE,
	.rxWsPulseWidth = CY_I2S_WS_ONE_CHANNEL_LENGTH,
	.rxWatchdogEnable = false,
	.rxWatchdogValue = 4294967295,
	.rxSdiLatchingTime = 0,
	.rxSckoInversion = 0,
	.rxSckiInversion = 0,
	.rxChannels = 2,
	.rxChannelLength = CY_I2S_LEN16,
	.rxWordLength = CY_I2S_LEN16,
	.rxSignExtension = 0,
	.rxFifoTriggerLevel = 0,
};
const cy_stc_sysanalog_config_t pass_0_aref_0_config = 
{
	.startup = CY_SYSANALOG_STARTUP_FAST,
	.iztat = CY_SYSANALOG_IZTAT_SOURCE_LOCAL,
	.vref = CY_SYSANALOG_VREF_SOURCE_LOCAL_1_2V,
	.deepSleep = CY_SYSANALOG_DEEPSLEEP_DISABLE,
};
const cy_stc_ctb_opamp_config_t OPAMP0_config = 
{
	.oaPower = CY_CTB_POWER_MEDIUM,
	.oaMode = CY_CTB_MODE_OPAMP1X,
	.oaPump = CY_CTB_PUMP_ENABLE,
	.oaCompEdge = CY_CTB_COMP_EDGE_BOTH,
	.oaCompLevel = CY_CTB_COMP_DSI_TRIGGER_OUT_PULSE,
	.oaCompBypass = CY_CTB_COMP_BYPASS_SYNC,
	.oaCompHyst = CY_CTB_COMP_HYST_DISABLE,
	.oaCompIntrEn = false,
};
const cy_stc_ctb_opamp_config_t OPAMP1_config = 
{
	.oaPower = CY_CTB_POWER_HIGH,
	.oaMode = CY_CTB_MODE_OPAMP10X,
	.oaPump = CY_CTB_PUMP_ENABLE,
	.oaCompEdge = CY_CTB_COMP_EDGE_BOTH,
	.oaCompLevel = CY_CTB_COMP_DSI_TRIGGER_OUT_PULSE,
	.oaCompBypass = CY_CTB_COMP_BYPASS_SYNC,
	.oaCompHyst = CY_CTB_COMP_HYST_DISABLE,
	.oaCompIntrEn = false,
};
const cy_stc_sar_config_t ADC_config = 
{
	.ctrl = (uint32_t) ADC_CTRL,
	.sampleCtrl = (uint32_t) ADC_SAMPLE,
	.sampleTime01 = (802UL << CY_SAR_SAMPLE_TIME0_SHIFT) | (4UL << CY_SAR_SAMPLE_TIME1_SHIFT),
	.sampleTime23 = (2UL << CY_SAR_SAMPLE_TIME2_SHIFT) | (2UL << CY_SAR_SAMPLE_TIME3_SHIFT),
	.rangeThres = (3584UL << CY_SAR_RANGE_HIGH_SHIFT) | (512UL << CY_SAR_RANGE_LOW_SHIFT),
	.rangeCond = CY_SAR_RANGE_COND_BELOW,
	.chanEn = 3UL,
	.chanConfig = {(uint32_t) ADC_CH0_CONFIG, (uint32_t) ADC_CH1_CONFIG, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL},
	.intrMask = (uint32_t) CY_SAR_INTR_EOS_MASK,
	.satIntrMask = 0UL,
	.rangeIntrMask = 0UL,
	.muxSwitch = ADC_MUX_SWITCH,
	.muxSwitchSqCtrl = ADC_MUX_SWITCH_HW_CTRL,
	.configRouting = true,
	.vrefMvValue = ADC_VREF_MV,
};
const cy_stc_scb_uart_config_t BT_UART_config = 
{
	.uartMode = CY_SCB_UART_STANDARD,
	.enableMutliProcessorMode = false,
	.smartCardRetryOnNack = false,
	.irdaInvertRx = false,
	.irdaEnableLowPowerReceiver = false,
	.oversample = 8,
	.enableMsbFirst = false,
	.dataWidth = 8UL,
	.parity = CY_SCB_UART_PARITY_NONE,
	.stopBits = CY_SCB_UART_STOP_BITS_1,
	.enableInputFilter = false,
	.breakWidth = 11UL,
	.dropOnFrameError = false,
	.dropOnParityError = false,
	.receiverAddress = 0x0UL,
	.receiverAddressMask = 0x0UL,
	.acceptAddrInFifo = false,
	.enableCts = true,
	.ctsPolarity = CY_SCB_UART_ACTIVE_LOW,
	.rtsRxFifoLevel = 63,
	.rtsPolarity = CY_SCB_UART_ACTIVE_LOW,
	.rxFifoTriggerLevel = 63UL,
	.rxFifoIntEnableMask = 0UL,
	.txFifoTriggerLevel = 63UL,
	.txFifoIntEnableMask = 0UL,
};
const cy_stc_scb_uart_config_t UART_config = 
{
	.uartMode = CY_SCB_UART_STANDARD,
	.enableMutliProcessorMode = false,
	.smartCardRetryOnNack = false,
	.irdaInvertRx = false,
	.irdaEnableLowPowerReceiver = false,
	.oversample = 8,
	.enableMsbFirst = false,
	.dataWidth = 8UL,
	.parity = CY_SCB_UART_PARITY_NONE,
	.stopBits = CY_SCB_UART_STOP_BITS_1,
	.enableInputFilter = false,
	.breakWidth = 11UL,
	.dropOnFrameError = false,
	.dropOnParityError = false,
	.receiverAddress = 0x0UL,
	.receiverAddressMask = 0x0UL,
	.acceptAddrInFifo = false,
	.enableCts = false,
	.ctsPolarity = CY_SCB_UART_ACTIVE_LOW,
	.rtsRxFifoLevel = 0UL,
	.rtsPolarity = CY_SCB_UART_ACTIVE_LOW,
	.rxFifoTriggerLevel = 63UL,
	.rxFifoIntEnableMask = 0UL,
	.txFifoTriggerLevel = 63UL,
	.txFifoIntEnableMask = 0UL,
};
const cy_stc_scb_i2c_config_t I2C_config = 
{
	.i2cMode = CY_SCB_I2C_SLAVE,
	.useRxFifo = true,
	.useTxFifo = true,
	.slaveAddress = 8,
	.slaveAddressMask = 254,
	.acceptAddrInFifo = false,
	.ackGeneralAddr = false,
	.enableWakeFromSleep = false,
	.enableDigitalFilter = false,
	.lowPhaseDutyCycle = 0,
	.highPhaseDutyCycle = 0,
};
const cy_stc_smif_config_t QSPI_config = 
{
	.mode = (uint32_t)CY_SMIF_NORMAL,
	.deselectDelay = QSPI_DESELECT_DELAY,
	.rxClockSel = (uint32_t)CY_SMIF_SEL_INV_INTERNAL_CLK,
	.blockEvent = (uint32_t)CY_SMIF_BUS_ERROR,
};
const cy_stc_tcpwm_pwm_config_t MOT_PWM_1_config = 
{
	.pwmMode = CY_TCPWM_PWM_MODE_PWM,
	.clockPrescaler = CY_TCPWM_PWM_PRESCALER_DIVBY_1,
	.pwmAlignment = CY_TCPWM_PWM_LEFT_ALIGN,
	.deadTimeClocks = 0,
	.runMode = CY_TCPWM_PWM_CONTINUOUS,
	.period0 = 100,
	.period1 = 32768,
	.enablePeriodSwap = false,
	.compare0 = 0,
	.compare1 = 16384,
	.enableCompareSwap = false,
	.interruptSources = CY_TCPWM_INT_NONE,
	.invertPWMOut = CY_TCPWM_PWM_INVERT_DISABLE,
	.invertPWMOutN = CY_TCPWM_PWM_INVERT_DISABLE,
	.killMode = CY_TCPWM_PWM_STOP_ON_KILL,
	.swapInputMode = MOT_PWM_1_INPUT_DISABLED & 0x3U,
	.swapInput = CY_TCPWM_INPUT_0,
	.reloadInputMode = MOT_PWM_1_INPUT_DISABLED & 0x3U,
	.reloadInput = CY_TCPWM_INPUT_0,
	.startInputMode = MOT_PWM_1_INPUT_DISABLED & 0x3U,
	.startInput = CY_TCPWM_INPUT_0,
	.killInputMode = MOT_PWM_1_INPUT_DISABLED & 0x3U,
	.killInput = CY_TCPWM_INPUT_0,
	.countInputMode = MOT_PWM_1_INPUT_DISABLED & 0x3U,
	.countInput = CY_TCPWM_INPUT_1,
};
const cy_stc_tcpwm_pwm_config_t MOT_PWM_2_config = 
{
	.pwmMode = CY_TCPWM_PWM_MODE_PWM,
	.clockPrescaler = CY_TCPWM_PWM_PRESCALER_DIVBY_1,
	.pwmAlignment = CY_TCPWM_PWM_LEFT_ALIGN,
	.deadTimeClocks = 0,
	.runMode = CY_TCPWM_PWM_CONTINUOUS,
	.period0 = 100,
	.period1 = 32768,
	.enablePeriodSwap = false,
	.compare0 = 0,
	.compare1 = 16384,
	.enableCompareSwap = false,
	.interruptSources = CY_TCPWM_INT_NONE,
	.invertPWMOut = CY_TCPWM_PWM_INVERT_DISABLE,
	.invertPWMOutN = CY_TCPWM_PWM_INVERT_DISABLE,
	.killMode = CY_TCPWM_PWM_STOP_ON_KILL,
	.swapInputMode = MOT_PWM_2_INPUT_DISABLED & 0x3U,
	.swapInput = CY_TCPWM_INPUT_0,
	.reloadInputMode = MOT_PWM_2_INPUT_DISABLED & 0x3U,
	.reloadInput = CY_TCPWM_INPUT_0,
	.startInputMode = MOT_PWM_2_INPUT_DISABLED & 0x3U,
	.startInput = CY_TCPWM_INPUT_0,
	.killInputMode = MOT_PWM_2_INPUT_DISABLED & 0x3U,
	.killInput = CY_TCPWM_INPUT_0,
	.countInputMode = MOT_PWM_2_INPUT_DISABLED & 0x3U,
	.countInput = CY_TCPWM_INPUT_1,
};


void init_cycfg_peripherals(void)
{
	Cy_SysClk_PeriphAssignDivider(PCLK_PASS_CLOCK_PUMP_PERI, CY_SYSCLK_DIV_8_BIT, 2U);
	Cy_CTB_SetDeepSleepMode(CTBM0, CY_CTB_DEEPSLEEP_DISABLE);

	Cy_SysClk_PeriphAssignDivider(PCLK_PASS_CLOCK_SAR, CY_SYSCLK_DIV_8_BIT, 3U);

	Cy_SysClk_PeriphAssignDivider(PCLK_SCB2_CLOCK, CY_SYSCLK_DIV_16_BIT, 0U);

	Cy_SysClk_PeriphAssignDivider(PCLK_SCB6_CLOCK, CY_SYSCLK_DIV_16_BIT, 1U);

	Cy_SysClk_PeriphAssignDivider(PCLK_SCB7_CLOCK, CY_SYSCLK_DIV_8_BIT, 4U);

	Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM1_CLOCKS16, CY_SYSCLK_DIV_8_BIT, 1U);

	Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM1_CLOCKS18, CY_SYSCLK_DIV_8_BIT, 1U);

	Cy_SysClk_PeriphAssignDivider(PCLK_UDB_CLOCKS0, CY_SYSCLK_DIV_8_BIT, 0u);
}
