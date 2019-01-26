/*******************************************************************************
* File Name: cycfg_capsense.c
* Version: 1.1
*
* Description:
* CapSense middleware configuration
* This file should not be modified. It was automatically generated by
* CapSense Configurator 1.1.0 build 220
*
********************************************************************************
* Copyright 2019 Cypress Semiconductor Corporation
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
*******************************************************************************/

#include "cycfg_capsense.h"

#if (CY_CAPSENSE_CORE == __CORTEX_M)

#ifndef CSD_CMODPADS_PORT
#define CSD_CMODPADS_PORT 0xFF
#endif
#ifndef CSD_CMODPADS_PIN
#define CSD_CMODPADS_PIN 0xFF
#endif
#ifndef CSD_CSH_TANKPADS_PORT
#define CSD_CSH_TANKPADS_PORT 0xFF
#endif
#ifndef CSD_CSH_TANKPADS_PIN
#define CSD_CSH_TANKPADS_PIN 0xFF
#endif
#ifndef CSD_CSHIELDPADS_PORT
#define CSD_CSHIELDPADS_PORT 0xFF
#endif
#ifndef CSD_CSHIELDPADS_PIN
#define CSD_CSHIELDPADS_PIN 0xFF
#endif
#ifndef CSD_VREF_EXT_PORT
#define CSD_VREF_EXT_PORT 0xFF
#endif
#ifndef CSD_VREF_EXT_PIN
#define CSD_VREF_EXT_PIN 0xFF
#endif

#define CY_CAPSENSE_WIDGET_COUNT (3u)
#define CY_CAPSENSE_SENSOR_COUNT (7u)
#define CY_CAPSENSE_ELTD_COUNT (9u)
#define CY_CAPSENSE_PIN_COUNT (9u)
#define CY_CAPSENSE_SHIELD_PIN_COUNT (0u)
#define CY_CAPSENSE_POSITION_SIZE (1u)
#define CY_CAPSENSE_DEBOUNCE_SIZE (3u)
#define CY_CAPSENSE_NOISE_ENVELOPE_SIZE (0u)
#define CY_CAPSENSE_RAW_HISTORY_SIZE (0u)
#define CY_CAPSENSE_IIR_HISTORY_LOW_SIZE (0u)
#define CY_CAPSENSE_POSITION_FILTER_HISTORY_SIZE (0u)
#define CY_CAPSENSE_TOUCH_FILTER_HISTORY_SIZE (0u)
#define CY_CAPSENSE_DIPLEX_SIZE (0u)
#define CY_CAPSENSE_CSD_TOUCHPAD_MAX_SENSORS_SIZE (0u)
#define CY_CAPSENSE_CSX_TOUCH_BUFFER_ENABLE (0u)
#define CY_CAPSENSE_CSX_TOUCH_HISTORY_SIZE (0u)
#define CY_CAPSENSE_BALLISTIC_WIDGET_COUNT (0u)
#define CY_CAPSENSE_GESTURE_WIDGET_COUNT (0u)

static cy_stc_capsense_internal_context_t cy_capsense_internalContext;
static cy_stc_active_scan_sns_t cy_capsense_activeScanSns;

#if (CY_CAPSENSE_DEBOUNCE_SIZE > 0)
    static uint8_t cy_capsense_debounce[CY_CAPSENSE_DEBOUNCE_SIZE] = {0};
#endif

#if (CY_CAPSENSE_NOISE_ENVELOPE_SIZE > 0)
    static cy_stc_capsense_smartsense_csd_noise_envelope_t cy_capsense_noiseEnvelope[CY_CAPSENSE_NOISE_ENVELOPE_SIZE];
#endif

#if (CY_CAPSENSE_RAW_HISTORY_SIZE > 0)
    static uint16_t cy_capsense_rawFilterHistory[CY_CAPSENSE_RAW_HISTORY_SIZE] = {0};
#endif

#if (CY_CAPSENSE_IIR_HISTORY_LOW_SIZE > 0)
    static uint8_t cy_capsense_iirHistoryLow[CY_CAPSENSE_IIR_HISTORY_LOW_SIZE] = {0};
#endif

#if (CY_CAPSENSE_POSITION_FILTER_HISTORY_SIZE > 0)
    static cy_stc_capsense_position_t cy_capsense_positionFilterHistory[CY_CAPSENSE_POSITION_FILTER_HISTORY_SIZE];
#endif

#if (CY_CAPSENSE_TOUCH_FILTER_HISTORY_SIZE > 0)
    static cy_stc_capsense_touch_t cy_capsense_touchFilterHistory[CY_CAPSENSE_TOUCH_FILTER_HISTORY_SIZE] =
    {
    };
#endif

#if (CY_CAPSENSE_DIPLEX_SIZE > 0)
    static const uint8_t cy_capsense_diplexTable[CY_CAPSENSE_DIPLEX_SIZE] =
    {
    };
#endif

#if (CY_CAPSENSE_CSD_TOUCHPAD_MAX_SENSORS_SIZE > 0)
    static uint16 cy_capsense_csdTouchBuffer[CY_CAPSENSE_CSD_TOUCHPAD_MAX_SENSORS_SIZE] = {0};
#endif

#if (CY_CAPSENSE_CSX_TOUCH_BUFFER_ENABLE > 0)
    static cy_stc_capsense_csx_touch_buffer_t cy_capsense_csxTouchBuffer;
#endif

#if (CY_CAPSENSE_CSX_TOUCH_HISTORY_SIZE > 0)
    static cy_stc_capsense_csx_touch_history_t cy_capsense_csxTouchHistory[CY_CAPSENSE_CSX_TOUCH_HISTORY_SIZE] =
    {
    };
#endif

#if (CY_CAPSENSE_BALLISTIC_WIDGET_COUNT > 0)
    static cy_stc_capsense_ballistic_context_t cy_capsense_ballisticContext[CY_CAPSENSE_BALLISTIC_WIDGET_COUNT];
#endif

#if (CY_CAPSENSE_GESTURE_WIDGET_COUNT > 0)
    static cy_stc_capsense_gesture_context_t cy_capsense_gestureContext[CY_CAPSENSE_GESTURE_WIDGET_COUNT];
#endif

static const cy_stc_capsense_common_config_t cy_capsense_commonConfig =
{
    .cpuClkHz = CY_CAPSENSE_CPU_CLK,
    .periClkHz = CY_CAPSENSE_PERI_CLK,
    .vdda = CY_CAPSENSE_VDDA_MV,
    .numPin = CY_CAPSENSE_PIN_COUNT,
    .numSns = CY_CAPSENSE_SENSOR_COUNT,
    .numWd = CY_CAPSENSE_WIDGET_COUNT,
    .csdEn = CY_CAPSENSE_ENABLE,
    .csxEn = CY_CAPSENSE_ENABLE,
    .mfsEn = CY_CAPSENSE_DISABLE,
    .positionFilterEn = CY_CAPSENSE_DISABLE,
    .periDividerType = CY_CAPSENSE_PERI_DIV_TYPE,
    .periDividerIndex = CY_CAPSENSE_PERI_DIV_INDEX,
    .analogWakeupDelay = 25u,
    .ssIrefSource = CY_CAPSENSE_IREF_SRSS,
    .ssVrefSource = CY_CAPSENSE_VREF_SRSS,
    .proxTouchCoeff = 300u,
    .swSensorAutoResetEn = CY_CAPSENSE_ENABLE,
    .portCmodPadNum = CSD_CMODPADS_PORT,
    .pinCmodPad = CSD_CMODPADS_PIN,
    .portCshPadNum = CSD_CSH_TANKPADS_PORT,
    .pinCshPad = CSD_CSH_TANKPADS_PIN,
    .portShieldPadNum = CSD_CSHIELDPADS_PORT,
    .pinShieldPad = CSD_CSHIELDPADS_PIN,
    .portVrefExtPadNum = CSD_VREF_EXT_PORT,
    .pinVrefExtPad = CSD_VREF_EXT_PIN,
    .portCmodNum = Cmod_PORT_NUM,
    .idacGainTable = {
        {0x01000000u,   37500u},
        {0x03000000u,   75000u},
        {0x01400000u,  300000u},
        {0x03400000u,  600000u},
        {0x01800000u, 2400000u},
        {0x03800000u, 4800000u},
    },
    .ptrCsdBase = CSD0,
    .ptrCsdContext = &cy_csd_0_context,
    .portCmod = Cmod_PORT,
    .portCsh = NULL,
    .portCintA = CintA_PORT,
    .portCintB = CintB_PORT,
    .pinCmod = Cmod_PIN,
    .portCshNum = 0u,
    .pinCsh = 0u,
    .pinCintA = CintA_PIN,
    .pinCintB = CintB_PIN,
    .csdShieldEn = CY_CAPSENSE_DISABLE,
    .csdInactiveSnsConnection = CY_CAPSENSE_SNS_CONNECTION_GROUND,
    .csdShieldDelay = CY_CAPSENSE_SH_DELAY_0NS,
    .csdVref = 0u,
    .csdRConst = 1000u,
    .csdCTankShieldEn = CY_CAPSENSE_DISABLE,
    .csdShieldNumPin = 0u,
    .csdShieldSwRes = CY_CAPSENSE_SHIELD_SW_RES_MEDIUM,
    .csdInitSwRes = CY_CAPSENSE_INIT_SW_RES_MEDIUM,
    .csdChargeTransfer = CY_CAPSENSE_IDAC_SOURCING,
    .csdRawTarget = 85u,
    .csdAutotuneEn = CY_CAPSENSE_CSD_SS_DIS,
    .csdIdacAutocalEn = CY_CAPSENSE_ENABLE,
    .csdIdacGainInit = 4u,
    .csdIdacAutoGainEn = CY_CAPSENSE_ENABLE,
    .csdCalibrationError = 10u,
    .csdIdacGainIndexDefault = 4u,
    .csdIdacMin = 20u,
    .csdIdacCompEn = CY_CAPSENSE_ENABLE,
    .csdFineInitTime = 10u,
    .csdIdacRowColAlignEn = CY_CAPSENSE_ENABLE,
    .csdMfsDividerOffsetF1 = 1u,
    .csdMfsDividerOffsetF2 = 2u,
    .csxRawTarget = 85u,
    .csxIdacGainInit = 1u,
    .csxIdacAutocalEn = CY_CAPSENSE_ENABLE,
    .csxCalibrationError = 20u,
    .csxFineInitTime = 10u,
    .csxInitSwRes = CY_CAPSENSE_INIT_SW_RES_MEDIUM,
    .csxScanSwRes = CY_CAPSENSE_INIT_SW_RES_MEDIUM,
    .csxInitShieldSwRes = CY_CAPSENSE_SHIELD_SW_RES_MEDIUM,
    .csxScanShieldSwRes = CY_CAPSENSE_SHIELD_SW_RES_MEDIUM,
    .csxMfsDividerOffsetF1 = 1u,
    .csxMfsDividerOffsetF2 = 2u,
};

static const cy_stc_capsense_pin_config_t cy_capsense_pinConfig[CY_CAPSENSE_PIN_COUNT] =
{
    { /* Button0_Rx0 */
        Button0_Rx0_PORT,
        Button0_Rx0_PIN,
    },
    { /* Button0_Tx */
        Button0_Tx_PORT,
        Button0_Tx_PIN,
    },
    { /* Button1_Rx0 */
        Button1_Rx0_PORT,
        Button1_Rx0_PIN,
    },
    { /* Button1_Tx */
        Button1_Tx_PORT,
        Button1_Tx_PIN,
    },
    { /* LinearSlider0_Sns0 */
        LinearSlider0_Sns0_PORT,
        LinearSlider0_Sns0_PIN,
    },
    { /* LinearSlider0_Sns1 */
        LinearSlider0_Sns1_PORT,
        LinearSlider0_Sns1_PIN,
    },
    { /* LinearSlider0_Sns2 */
        LinearSlider0_Sns2_PORT,
        LinearSlider0_Sns2_PIN,
    },
    { /* LinearSlider0_Sns3 */
        LinearSlider0_Sns3_PORT,
        LinearSlider0_Sns3_PIN,
    },
    { /* LinearSlider0_Sns4 */
        LinearSlider0_Sns4_PORT,
        LinearSlider0_Sns4_PIN,
    },
};

#if (CY_CAPSENSE_SHIELD_PIN_COUNT > 0)
    static const cy_stc_capsense_pin_config_t cy_capsense_shieldPinConfig[CY_CAPSENSE_SHIELD_PIN_COUNT] =
    {
    };
#endif

static const cy_stc_capsense_electrode_config_t cy_capsense_electrodeConfig[CY_CAPSENSE_ELTD_COUNT] =
{
    { /* Button0_Rx0 */
        .ptrPin = &cy_capsense_pinConfig[0u],
        .type = CY_CAPSENSE_ELTD_TYPE_MUT_RX_E,
        .numPins = 1u,
    },
    { /* Button0_Tx */
        .ptrPin = &cy_capsense_pinConfig[1u],
        .type = CY_CAPSENSE_ELTD_TYPE_MUT_TX_E,
        .numPins = 1u,
    },
    { /* Button1_Rx0 */
        .ptrPin = &cy_capsense_pinConfig[2u],
        .type = CY_CAPSENSE_ELTD_TYPE_MUT_RX_E,
        .numPins = 1u,
    },
    { /* Button1_Tx */
        .ptrPin = &cy_capsense_pinConfig[3u],
        .type = CY_CAPSENSE_ELTD_TYPE_MUT_TX_E,
        .numPins = 1u,
    },
    { /* LinearSlider0_Sns0 */
        .ptrPin = &cy_capsense_pinConfig[4u],
        .type = CY_CAPSENSE_ELTD_TYPE_SELF_E,
        .numPins = 1u,
    },
    { /* LinearSlider0_Sns1 */
        .ptrPin = &cy_capsense_pinConfig[5u],
        .type = CY_CAPSENSE_ELTD_TYPE_SELF_E,
        .numPins = 1u,
    },
    { /* LinearSlider0_Sns2 */
        .ptrPin = &cy_capsense_pinConfig[6u],
        .type = CY_CAPSENSE_ELTD_TYPE_SELF_E,
        .numPins = 1u,
    },
    { /* LinearSlider0_Sns3 */
        .ptrPin = &cy_capsense_pinConfig[7u],
        .type = CY_CAPSENSE_ELTD_TYPE_SELF_E,
        .numPins = 1u,
    },
    { /* LinearSlider0_Sns4 */
        .ptrPin = &cy_capsense_pinConfig[8u],
        .type = CY_CAPSENSE_ELTD_TYPE_SELF_E,
        .numPins = 1u,
    },
};

static const cy_stc_capsense_widget_config_t cy_capsense_widgetConfig[CY_CAPSENSE_WIDGET_COUNT] =
{
    { /* Button0 */
        .ptrWdContext = &cy_capsense_tuner.widgetContext[0u],
        .ptrSnsContext = &cy_capsense_tuner.sensorContext[0u],
        .ptrEltdConfig = &cy_capsense_electrodeConfig[0u],
        .ptrNoiseEnvelope = NULL,
        .ptrRawFilterHistory = NULL,
        .ptrRawFilterHistoryLow = NULL,
        .iirCoeff = 128u,
        .ptrDebounceArr = &cy_capsense_debounce[0u],
        .ptrDiplexTable = NULL,
        .centroidConfig = 0u,
        .xResolution = 0u,
        .yResolution = 0u,
        .numSns = 1u,
        .numCols = 1u,
        .numRows = 1u,
        .ptrPosFilterHistory = NULL,
        .ptrCsxTouchHistory = NULL,
        .ptrCsxTouchBuffer = NULL,
        .ptrCsdTouchBuffer = NULL,
        .ptrGestureConfig = NULL,
        .ptrGestureContext = NULL,
        .ballisticConfig = {
            .accelCoeff = 9u,
            .speedCoeff = 2u,
            .divisorValue = 4u,
            .speedThresholdX = 3u,
            .speedThresholdY = 4u,
        },
        .ptrBallisticContext = NULL,
        .aiirConfig = {
            .maxK = 60u,
            .minK = 1u,
            .noMovTh = 3u,
            .littleMovTh = 7u,
            .largeMovTh = 12u,
            .divVal = 64u,
        },
        .advConfig = {
            .penultimateTh = 100u,
            .virtualSnsTh = 100u,
            .crossCouplingTh = 5u,
        },
        .posFilterConfig = 0u,
        .rawFilterConfig = 0u,
        .senseMethod = CY_CAPSENSE_SENSE_METHOD_CSX_E,
        .wdType = CY_CAPSENSE_WD_BUTTON_E,
    },
    { /* Button1 */
        .ptrWdContext = &cy_capsense_tuner.widgetContext[1u],
        .ptrSnsContext = &cy_capsense_tuner.sensorContext[1u],
        .ptrEltdConfig = &cy_capsense_electrodeConfig[2u],
        .ptrNoiseEnvelope = NULL,
        .ptrRawFilterHistory = NULL,
        .ptrRawFilterHistoryLow = NULL,
        .iirCoeff = 128u,
        .ptrDebounceArr = &cy_capsense_debounce[1u],
        .ptrDiplexTable = NULL,
        .centroidConfig = 0u,
        .xResolution = 0u,
        .yResolution = 0u,
        .numSns = 1u,
        .numCols = 1u,
        .numRows = 1u,
        .ptrPosFilterHistory = NULL,
        .ptrCsxTouchHistory = NULL,
        .ptrCsxTouchBuffer = NULL,
        .ptrCsdTouchBuffer = NULL,
        .ptrGestureConfig = NULL,
        .ptrGestureContext = NULL,
        .ballisticConfig = {
            .accelCoeff = 9u,
            .speedCoeff = 2u,
            .divisorValue = 4u,
            .speedThresholdX = 3u,
            .speedThresholdY = 4u,
        },
        .ptrBallisticContext = NULL,
        .aiirConfig = {
            .maxK = 60u,
            .minK = 1u,
            .noMovTh = 3u,
            .littleMovTh = 7u,
            .largeMovTh = 12u,
            .divVal = 64u,
        },
        .advConfig = {
            .penultimateTh = 100u,
            .virtualSnsTh = 100u,
            .crossCouplingTh = 5u,
        },
        .posFilterConfig = 0u,
        .rawFilterConfig = 0u,
        .senseMethod = CY_CAPSENSE_SENSE_METHOD_CSX_E,
        .wdType = CY_CAPSENSE_WD_BUTTON_E,
    },
    { /* LinearSlider0 */
        .ptrWdContext = &cy_capsense_tuner.widgetContext[2u],
        .ptrSnsContext = &cy_capsense_tuner.sensorContext[2u],
        .ptrEltdConfig = &cy_capsense_electrodeConfig[4u],
        .ptrNoiseEnvelope = NULL,
        .ptrRawFilterHistory = NULL,
        .ptrRawFilterHistoryLow = NULL,
        .iirCoeff = 128u,
        .ptrDebounceArr = &cy_capsense_debounce[2u],
        .ptrDiplexTable = NULL,
        .centroidConfig = 1u,
        .xResolution = 100u,
        .yResolution = 0u,
        .numSns = 5u,
        .numCols = 5u,
        .numRows = 0u,
        .ptrPosFilterHistory = NULL,
        .ptrCsxTouchHistory = NULL,
        .ptrCsxTouchBuffer = NULL,
        .ptrCsdTouchBuffer = NULL,
        .ptrGestureConfig = NULL,
        .ptrGestureContext = NULL,
        .ballisticConfig = {
            .accelCoeff = 9u,
            .speedCoeff = 2u,
            .divisorValue = 4u,
            .speedThresholdX = 3u,
            .speedThresholdY = 4u,
        },
        .ptrBallisticContext = NULL,
        .aiirConfig = {
            .maxK = 60u,
            .minK = 1u,
            .noMovTh = 3u,
            .littleMovTh = 7u,
            .largeMovTh = 12u,
            .divVal = 64u,
        },
        .advConfig = {
            .penultimateTh = 100u,
            .virtualSnsTh = 100u,
            .crossCouplingTh = 5u,
        },
        .posFilterConfig = 0u,
        .rawFilterConfig = 0u,
        .senseMethod = CY_CAPSENSE_SENSE_METHOD_CSD_E,
        .wdType = CY_CAPSENSE_WD_LINEAR_SLIDER_E,
    },
};

cy_stc_capsense_tuner_t cy_capsense_tuner =
{
    .commonContext = {
        .configId = 0xca04,
        .tunerCmd = 0u,
        .scanCounter = 0u,
        .tunerSt = 0u,
        .initDone = 0u,
        .ptrSSCallback = NULL,
        .ptrEOSCallback = NULL,
        .ptrTunerSendCallback = NULL,
        .ptrTunerReceiveCallback = NULL,
        .status = 0u,
        .timestampInterval = 1u,
        .timestamp = 0u,
        .modCsdClk = 1u,
        .modCsxClk = 1u,
        .tunerCnt = 0u,
    },
    .widgetContext = {
        {
            .fingerCap = 300u,
            .sigPFC = 0u,
            .resolution = 100u,
            .maxRawCount = 0u,
            .fingerTh = 90u,
            .proxTh = 200u,
            .lowBslnRst = 30u,
            .snsClk = 80u,
            .rowSnsClk = 2u,
            .gestureDetected = 0u,
            .gestureDirection = 0u,
            .xDelta = 0u,
            .yDelta = 0u,
            .noiseTh = 40u,
            .nNoiseTh = 40u,
            .hysteresis = 20u,
            .onDebounce = 3u,
            .snsClkSource = CY_CAPSENSE_CLK_SOURCE_AUTO_MASK,
            .idacMod = { 32u, 32u, 32u, },
            .idacGainIndex = 0u,
            .rowIdacMod = { 32u, 32u, 32u, },
            .bslnCoeff = 1u,
            .status = 0u,
            .wdTouch = {
                .ptrPosition = NULL,
                .numPosition = 0,
            },
        },
        {
            .fingerCap = 160u,
            .sigPFC = 0u,
            .resolution = 100u,
            .maxRawCount = 0u,
            .fingerTh = 100u,
            .proxTh = 200u,
            .lowBslnRst = 30u,
            .snsClk = 32u,
            .rowSnsClk = 16u,
            .gestureDetected = 0u,
            .gestureDirection = 0u,
            .xDelta = 0u,
            .yDelta = 0u,
            .noiseTh = 40u,
            .nNoiseTh = 40u,
            .hysteresis = 10u,
            .onDebounce = 3u,
            .snsClkSource = CY_CAPSENSE_CLK_SOURCE_AUTO_MASK,
            .idacMod = { 32u, 32u, 32u, },
            .idacGainIndex = 0u,
            .rowIdacMod = { 32u, 32u, 32u, },
            .bslnCoeff = 1u,
            .status = 0u,
            .wdTouch = {
                .ptrPosition = NULL,
                .numPosition = 0,
            },
        },
        {
            .fingerCap = 100u,
            .sigPFC = 0u,
            .resolution = 13u,
            .maxRawCount = 0u,
            .fingerTh = 100u,
            .proxTh = 200u,
            .lowBslnRst = 30u,
            .snsClk = 34u,
            .rowSnsClk = 2u,
            .gestureDetected = 0u,
            .gestureDirection = 0u,
            .xDelta = 0u,
            .yDelta = 0u,
            .noiseTh = 40u,
            .nNoiseTh = 40u,
            .hysteresis = 10u,
            .onDebounce = 3u,
            .snsClkSource = CY_CAPSENSE_CLK_SOURCE_DIRECT,
            .idacMod = { 32u, 32u, 32u, },
            .idacGainIndex = 0u,
            .rowIdacMod = { 32u, 32u, 32u, },
            .bslnCoeff = 1u,
            .status = 0u,
            .wdTouch = {
                .ptrPosition = &cy_capsense_tuner.position[0u],
                .numPosition = 1,
            },
        },
    },
    .sensorContext = {
        {
            .raw = 0u,
            .bsln = 0u,
            .diff = 0u,
            .status = 0u,
            .negBslnRstCnt = 0u,
            .idacComp = 32u,
            .bslnExt = 0u,
        },
        {
            .raw = 0u,
            .bsln = 0u,
            .diff = 0u,
            .status = 0u,
            .negBslnRstCnt = 0u,
            .idacComp = 32u,
            .bslnExt = 0u,
        },
        {
            .raw = 0u,
            .bsln = 0u,
            .diff = 0u,
            .status = 0u,
            .negBslnRstCnt = 0u,
            .idacComp = 32u,
            .bslnExt = 0u,
        },
        {
            .raw = 0u,
            .bsln = 0u,
            .diff = 0u,
            .status = 0u,
            .negBslnRstCnt = 0u,
            .idacComp = 32u,
            .bslnExt = 0u,
        },
        {
            .raw = 0u,
            .bsln = 0u,
            .diff = 0u,
            .status = 0u,
            .negBslnRstCnt = 0u,
            .idacComp = 32u,
            .bslnExt = 0u,
        },
        {
            .raw = 0u,
            .bsln = 0u,
            .diff = 0u,
            .status = 0u,
            .negBslnRstCnt = 0u,
            .idacComp = 32u,
            .bslnExt = 0u,
        },
        {
            .raw = 0u,
            .bsln = 0u,
            .diff = 0u,
            .status = 0u,
            .negBslnRstCnt = 0u,
            .idacComp = 32u,
            .bslnExt = 0u,
        },
    },
    .position = {
        {
            .x = 0u,
            .y = 0u,
            .z = 0u,
            .id = 0u,
        },
    },
};

cy_stc_capsense_context_t cy_capsense_context =
{
    .ptrCommonConfig = &cy_capsense_commonConfig,
    .ptrCommonContext = &cy_capsense_tuner.commonContext,
    .ptrInternalContext = &cy_capsense_internalContext,
    .ptrWdConfig = &cy_capsense_widgetConfig[0u],
    .ptrWdContext = &cy_capsense_tuner.widgetContext[0u],
    .ptrPinConfig = &cy_capsense_pinConfig[0u],
    .ptrShieldPinConfig = NULL,
    .ptrActiveScanSns = &cy_capsense_activeScanSns,
};

#endif /* CY_CAPSENSE_CORE == __CORTEX_M */

/* [] END OF FILE */

