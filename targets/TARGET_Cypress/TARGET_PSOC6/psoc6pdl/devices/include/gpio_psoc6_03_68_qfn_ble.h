/***************************************************************************//**
* \file gpio_psoc6_03_68_qfn_ble.h
*
* \brief
* PSoC6_03 device GPIO header for 68-QFN-BLE package
*
* \note
* Generator version: 1.3.6934.1396
*
********************************************************************************
* \copyright
* Copyright 2016-2019, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _GPIO_PSOC6_03_68_QFN_BLE_H_
#define _GPIO_PSOC6_03_68_QFN_BLE_H_

/* Package type */
enum
{
    CY_GPIO_PACKAGE_QFN,
    CY_GPIO_PACKAGE_BGA,
    CY_GPIO_PACKAGE_CSP,
    CY_GPIO_PACKAGE_WLCSP,
    CY_GPIO_PACKAGE_LQFP,
    CY_GPIO_PACKAGE_TQFP,
    CY_GPIO_PACKAGE_SMT,
};

#define CY_GPIO_PACKAGE_TYPE            CY_GPIO_PACKAGE_QFN
#define CY_GPIO_PIN_COUNT               68u

/* AMUXBUS Segments */
enum
{
    AMUXBUS_SRSS_AMUXBUSA_ADFT_VDDD,
    AMUXBUS_SRSS_AMUXBUSB_ADFT_VDDD,
    AMUXBUS_CSD,
    AMUXBUS_BLESS_AMUXBUS_A,
    AMUXBUS_BLESS_AMUXBUS_B,
    AMUXBUS_LPCOMP_AMUXBUS_A,
    AMUXBUS_LPCOMP_AMUXBUS_B,
    AMUXBUS_CPUSS_AMUXBUSA,
    AMUXBUS_CPUSS_AMUXBUSB,
    AMUXBUS_CSD1,
    AMUXBUS_SAR,
    AMUXBUS_AMXBUS_A_SAR,
    AMUXBUS_AMXBUS_B_SAR,
};

/* AMUX Splitter Controls */
typedef enum
{
    AMUX_SPLIT_CTL_0                = 0x0000u,  /* Left = AMUXBUS_SRSS._ADFT_VDDD; Right = AMUXBUS_SRSS__ADFT_VDDD */
    AMUX_SPLIT_CTL_4                = 0x0004u,  /* Left = AMUXBUS_CSD; Right = AMUXBUS_CSD1 */
    AMUX_SPLIT_CTL_5                = 0x0005u   /* Left = AMUXBUS_CSD1; Right = AMUXBUS_SAR */
} cy_en_amux_split_t;

/* Port List */
/* PORT 0 (AUTOLVL) */
#define P0_0_PORT                       GPIO_PRT0
#define P0_0_PIN                        0u
#define P0_0_NUM                        0u
#define P0_1_PORT                       GPIO_PRT0
#define P0_1_PIN                        1u
#define P0_1_NUM                        1u
#define P0_2_PORT                       GPIO_PRT0
#define P0_2_PIN                        2u
#define P0_2_NUM                        2u
#define P0_3_PORT                       GPIO_PRT0
#define P0_3_PIN                        3u
#define P0_3_NUM                        3u
#define P0_4_PORT                       GPIO_PRT0
#define P0_4_PIN                        4u
#define P0_4_NUM                        4u
#define P0_5_PORT                       GPIO_PRT0
#define P0_5_PIN                        5u
#define P0_5_NUM                        5u

/* PORT 6 (AUTOLVL) */
#define P6_0_PORT                       GPIO_PRT6
#define P6_0_PIN                        0u
#define P6_0_NUM                        0u
#define P6_1_PORT                       GPIO_PRT6
#define P6_1_PIN                        1u
#define P6_1_NUM                        1u
#define P6_2_PORT                       GPIO_PRT6
#define P6_2_PIN                        2u
#define P6_2_NUM                        2u
#define P6_3_PORT                       GPIO_PRT6
#define P6_3_PIN                        3u
#define P6_3_NUM                        3u
#define P6_4_PORT                       GPIO_PRT6
#define P6_4_PIN                        4u
#define P6_4_NUM                        4u
#define P6_5_PORT                       GPIO_PRT6
#define P6_5_PIN                        5u
#define P6_5_NUM                        5u
#define P6_6_PORT                       GPIO_PRT6
#define P6_6_PIN                        6u
#define P6_6_NUM                        6u
#define P6_7_PORT                       GPIO_PRT6
#define P6_7_PIN                        7u
#define P6_7_NUM                        7u

/* PORT 7 (AUTOLVL) */
#define P7_0_PORT                       GPIO_PRT7
#define P7_0_PIN                        0u
#define P7_0_NUM                        0u
#define P7_0_AMUXSEGMENT                AMUXBUS_CSD
#define P7_1_PORT                       GPIO_PRT7
#define P7_1_PIN                        1u
#define P7_1_NUM                        1u
#define P7_1_AMUXSEGMENT                AMUXBUS_CSD
#define P7_2_PORT                       GPIO_PRT7
#define P7_2_PIN                        2u
#define P7_2_NUM                        2u
#define P7_2_AMUXSEGMENT                AMUXBUS_CSD
#define P7_3_PORT                       GPIO_PRT7
#define P7_3_PIN                        3u
#define P7_3_NUM                        3u
#define P7_3_AMUXSEGMENT                AMUXBUS_CSD
#define P7_4_PORT                       GPIO_PRT7
#define P7_4_PIN                        4u
#define P7_4_NUM                        4u
#define P7_4_AMUXSEGMENT                AMUXBUS_CSD
#define P7_5_PORT                       GPIO_PRT7
#define P7_5_PIN                        5u
#define P7_5_NUM                        5u
#define P7_5_AMUXSEGMENT                AMUXBUS_CSD
#define P7_6_PORT                       GPIO_PRT7
#define P7_6_PIN                        6u
#define P7_6_NUM                        6u
#define P7_6_AMUXSEGMENT                AMUXBUS_CSD
#define P7_7_PORT                       GPIO_PRT7
#define P7_7_PIN                        7u
#define P7_7_NUM                        7u
#define P7_7_AMUXSEGMENT                AMUXBUS_CSD

/* PORT 8 (GPIO_OVT, AUTOLVL) */
#define P8_0_PORT                       GPIO_PRT8
#define P8_0_PIN                        0u
#define P8_0_NUM                        0u
#define P8_0_AMUXSEGMENT                AMUXBUS_CSD
#define P8_1_PORT                       GPIO_PRT8
#define P8_1_PIN                        1u
#define P8_1_NUM                        1u
#define P8_1_AMUXSEGMENT                AMUXBUS_CSD
#define P8_2_PORT                       GPIO_PRT8
#define P8_2_PIN                        2u
#define P8_2_NUM                        2u
#define P8_2_AMUXSEGMENT                AMUXBUS_CSD

/* PORT 9 (AUTOLVL) */
#define P9_0_PORT                       GPIO_PRT9
#define P9_0_PIN                        0u
#define P9_0_NUM                        0u
#define P9_0_AMUXSEGMENT                AMUXBUS_SAR
#define P9_1_PORT                       GPIO_PRT9
#define P9_1_PIN                        1u
#define P9_1_NUM                        1u
#define P9_1_AMUXSEGMENT                AMUXBUS_SAR
#define P9_2_PORT                       GPIO_PRT9
#define P9_2_PIN                        2u
#define P9_2_NUM                        2u
#define P9_2_AMUXSEGMENT                AMUXBUS_SAR
#define P9_3_PORT                       GPIO_PRT9
#define P9_3_PIN                        3u
#define P9_3_NUM                        3u
#define P9_3_AMUXSEGMENT                AMUXBUS_SAR

/* PORT 10 (AUTOLVL) */
#define P10_0_PORT                      GPIO_PRT10
#define P10_0_PIN                       0u
#define P10_0_NUM                       0u
#define P10_0_AMUXSEGMENT               AMUXBUS_SAR
#define P10_1_PORT                      GPIO_PRT10
#define P10_1_PIN                       1u
#define P10_1_NUM                       1u
#define P10_1_AMUXSEGMENT               AMUXBUS_SAR

/* PORT 11 (AUTOLVL) */
#define P11_0_PORT                      GPIO_PRT11
#define P11_0_PIN                       0u
#define P11_0_NUM                       0u
#define P11_1_PORT                      GPIO_PRT11
#define P11_1_PIN                       1u
#define P11_1_NUM                       1u
#define P11_2_PORT                      GPIO_PRT11
#define P11_2_PIN                       2u
#define P11_2_NUM                       2u
#define P11_3_PORT                      GPIO_PRT11
#define P11_3_PIN                       3u
#define P11_3_NUM                       3u
#define P11_4_PORT                      GPIO_PRT11
#define P11_4_PIN                       4u
#define P11_4_NUM                       4u
#define P11_5_PORT                      GPIO_PRT11
#define P11_5_PIN                       5u
#define P11_5_NUM                       5u
#define P11_6_PORT                      GPIO_PRT11
#define P11_6_PIN                       6u
#define P11_6_NUM                       6u
#define P11_7_PORT                      GPIO_PRT11
#define P11_7_PIN                       7u
#define P11_7_NUM                       7u

/* PORT 12 (AUTOLVL) */
#define P12_6_PORT                      GPIO_PRT12
#define P12_6_PIN                       6u
#define P12_6_NUM                       6u
#define P12_7_PORT                      GPIO_PRT12
#define P12_7_PIN                       7u
#define P12_7_NUM                       7u

/* Analog Connections */
#define CSD_CMODPADD_PORT               7u
#define CSD_CMODPADD_PIN                1u
#define CSD_CMODPADS_PORT               7u
#define CSD_CMODPADS_PIN                1u
#define CSD_CSH_TANKPADD_PORT           7u
#define CSD_CSH_TANKPADD_PIN            2u
#define CSD_CSH_TANKPADS_PORT           7u
#define CSD_CSH_TANKPADS_PIN            2u
#define CSD_CSHIELDPADS_PORT            7u
#define CSD_CSHIELDPADS_PIN             7u
#define CSD_VREF_EXT_PORT               7u
#define CSD_VREF_EXT_PIN                3u
#define IOSS_ADFT0_NET_PORT             10u
#define IOSS_ADFT0_NET_PIN              0u
#define IOSS_ADFT1_NET_PORT             10u
#define IOSS_ADFT1_NET_PIN              1u
#define LPCOMP_INN_COMP0_PORT           9u
#define LPCOMP_INN_COMP0_PIN            3u
#define LPCOMP_INP_COMP0_PORT           9u
#define LPCOMP_INP_COMP0_PIN            2u
#define PASS_AREF_EXT_VREF_PORT         9u
#define PASS_AREF_EXT_VREF_PIN          3u
#define PASS_SARMUX_PADS0_PORT          10u
#define PASS_SARMUX_PADS0_PIN           0u
#define PASS_SARMUX_PADS1_PORT          10u
#define PASS_SARMUX_PADS1_PIN           1u
#define SRSS_ADFT_PIN0_PORT             10u
#define SRSS_ADFT_PIN0_PIN              0u
#define SRSS_ADFT_PIN1_PORT             10u
#define SRSS_ADFT_PIN1_PIN              1u
#define SRSS_ECO_IN_PORT                12u
#define SRSS_ECO_IN_PIN                 6u
#define SRSS_ECO_OUT_PORT               12u
#define SRSS_ECO_OUT_PIN                7u
#define SRSS_WCO_IN_PORT                0u
#define SRSS_WCO_IN_PIN                 0u
#define SRSS_WCO_OUT_PORT               0u
#define SRSS_WCO_OUT_PIN                1u

/* HSIOM Connections */
typedef enum
{
    /* Generic HSIOM connections */
    HSIOM_SEL_GPIO                  =  0,       /* GPIO controls 'out' */
    HSIOM_SEL_GPIO_DSI              =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    HSIOM_SEL_DSI_DSI               =  2,       /* DSI controls 'out' and 'output enable' */
    HSIOM_SEL_DSI_GPIO              =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    HSIOM_SEL_AMUXA                 =  4,       /* Analog mux bus A */
    HSIOM_SEL_AMUXB                 =  5,       /* Analog mux bus B */
    HSIOM_SEL_AMUXA_DSI             =  6,       /* Analog mux bus A, DSI control */
    HSIOM_SEL_AMUXB_DSI             =  7,       /* Analog mux bus B, DSI control */
    HSIOM_SEL_ACT_0                 =  8,       /* Active functionality 0 */
    HSIOM_SEL_ACT_1                 =  9,       /* Active functionality 1 */
    HSIOM_SEL_ACT_2                 = 10,       /* Active functionality 2 */
    HSIOM_SEL_ACT_3                 = 11,       /* Active functionality 3 */
    HSIOM_SEL_DS_0                  = 12,       /* DeepSleep functionality 0 */
    HSIOM_SEL_DS_1                  = 13,       /* DeepSleep functionality 1 */
    HSIOM_SEL_DS_2                  = 14,       /* DeepSleep functionality 2 */
    HSIOM_SEL_DS_3                  = 15,       /* DeepSleep functionality 3 */
    HSIOM_SEL_ACT_4                 = 16,       /* Active functionality 4 */
    HSIOM_SEL_ACT_5                 = 17,       /* Active functionality 5 */
    HSIOM_SEL_ACT_6                 = 18,       /* Active functionality 6 */
    HSIOM_SEL_ACT_7                 = 19,       /* Active functionality 7 */
    HSIOM_SEL_ACT_8                 = 20,       /* Active functionality 8 */
    HSIOM_SEL_ACT_9                 = 21,       /* Active functionality 9 */
    HSIOM_SEL_ACT_10                = 22,       /* Active functionality 10 */
    HSIOM_SEL_ACT_11                = 23,       /* Active functionality 11 */
    HSIOM_SEL_ACT_12                = 24,       /* Active functionality 12 */
    HSIOM_SEL_ACT_13                = 25,       /* Active functionality 13 */
    HSIOM_SEL_ACT_14                = 26,       /* Active functionality 14 */
    HSIOM_SEL_ACT_15                = 27,       /* Active functionality 15 */
    HSIOM_SEL_DS_4                  = 28,       /* DeepSleep functionality 4 */
    HSIOM_SEL_DS_5                  = 29,       /* DeepSleep functionality 5 */
    HSIOM_SEL_DS_6                  = 30,       /* DeepSleep functionality 6 */
    HSIOM_SEL_DS_7                  = 31,       /* DeepSleep functionality 7 */

    /* P0.0 */
    P0_0_GPIO                       =  0,       /* GPIO controls 'out' */
    P0_0_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P0_0_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P0_0_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P0_0_AMUXA                      =  4,       /* Analog mux bus A */
    P0_0_AMUXB                      =  5,       /* Analog mux bus B */
    P0_0_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P0_0_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P0_0_TCPWM0_LINE0               =  8,       /* Digital Active - tcpwm[0].line[0]:0 */
    P0_0_TCPWM1_LINE0               =  9,       /* Digital Active - tcpwm[1].line[0]:0 */
    P0_0_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:0 */
    P0_0_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:0 */
    P0_0_LCD_COM0                   = 12,       /* Digital Deep Sleep - lcd.com[0]:0 */
    P0_0_LCD_SEG0                   = 13,       /* Digital Deep Sleep - lcd.seg[0]:0 */
    P0_0_SRSS_EXT_CLK               = 16,       /* Digital Active - srss.ext_clk:0 */
    P0_0_SCB0_SPI_SELECT1           = 20,       /* Digital Active - scb[0].spi_select1:0 */
    P0_0_PERI_TR_IO_INPUT0          = 24,       /* Digital Active - peri.tr_io_input[0]:0 */

    /* P0.1 */
    P0_1_GPIO                       =  0,       /* GPIO controls 'out' */
    P0_1_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P0_1_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P0_1_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P0_1_AMUXA                      =  4,       /* Analog mux bus A */
    P0_1_AMUXB                      =  5,       /* Analog mux bus B */
    P0_1_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P0_1_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P0_1_TCPWM0_LINE_COMPL0         =  8,       /* Digital Active - tcpwm[0].line_compl[0]:0 */
    P0_1_TCPWM1_LINE_COMPL0         =  9,       /* Digital Active - tcpwm[1].line_compl[0]:0 */
    P0_1_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:1 */
    P0_1_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:1 */
    P0_1_LCD_COM1                   = 12,       /* Digital Deep Sleep - lcd.com[1]:0 */
    P0_1_LCD_SEG1                   = 13,       /* Digital Deep Sleep - lcd.seg[1]:0 */
    P0_1_SCB0_SPI_SELECT2           = 20,       /* Digital Active - scb[0].spi_select2:0 */
    P0_1_PERI_TR_IO_INPUT1          = 24,       /* Digital Active - peri.tr_io_input[1]:0 */
    P0_1_CPUSS_SWJ_TRSTN            = 29,       /* Digital Deep Sleep - cpuss.swj_trstn */

    /* P0.2 */
    P0_2_GPIO                       =  0,       /* GPIO controls 'out' */
    P0_2_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P0_2_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P0_2_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P0_2_AMUXA                      =  4,       /* Analog mux bus A */
    P0_2_AMUXB                      =  5,       /* Analog mux bus B */
    P0_2_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P0_2_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P0_2_TCPWM0_LINE1               =  8,       /* Digital Active - tcpwm[0].line[1]:0 */
    P0_2_TCPWM1_LINE1               =  9,       /* Digital Active - tcpwm[1].line[1]:0 */
    P0_2_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:2 */
    P0_2_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:2 */
    P0_2_LCD_COM2                   = 12,       /* Digital Deep Sleep - lcd.com[2]:0 */
    P0_2_LCD_SEG2                   = 13,       /* Digital Deep Sleep - lcd.seg[2]:0 */
    P0_2_SCB0_UART_RX               = 18,       /* Digital Active - scb[0].uart_rx:0 */
    P0_2_SCB0_I2C_SCL               = 19,       /* Digital Active - scb[0].i2c_scl:0 */
    P0_2_SCB0_SPI_MOSI              = 20,       /* Digital Active - scb[0].spi_mosi:0 */

    /* P0.3 */
    P0_3_GPIO                       =  0,       /* GPIO controls 'out' */
    P0_3_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P0_3_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P0_3_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P0_3_AMUXA                      =  4,       /* Analog mux bus A */
    P0_3_AMUXB                      =  5,       /* Analog mux bus B */
    P0_3_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P0_3_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P0_3_TCPWM0_LINE_COMPL1         =  8,       /* Digital Active - tcpwm[0].line_compl[1]:0 */
    P0_3_TCPWM1_LINE_COMPL1         =  9,       /* Digital Active - tcpwm[1].line_compl[1]:0 */
    P0_3_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:3 */
    P0_3_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:3 */
    P0_3_LCD_COM3                   = 12,       /* Digital Deep Sleep - lcd.com[3]:0 */
    P0_3_LCD_SEG3                   = 13,       /* Digital Deep Sleep - lcd.seg[3]:0 */
    P0_3_SCB0_UART_TX               = 18,       /* Digital Active - scb[0].uart_tx:0 */
    P0_3_SCB0_I2C_SDA               = 19,       /* Digital Active - scb[0].i2c_sda:0 */
    P0_3_SCB0_SPI_MISO              = 20,       /* Digital Active - scb[0].spi_miso:0 */

    /* P0.4 */
    P0_4_GPIO                       =  0,       /* GPIO controls 'out' */
    P0_4_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P0_4_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P0_4_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P0_4_AMUXA                      =  4,       /* Analog mux bus A */
    P0_4_AMUXB                      =  5,       /* Analog mux bus B */
    P0_4_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P0_4_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P0_4_TCPWM0_LINE2               =  8,       /* Digital Active - tcpwm[0].line[2]:0 */
    P0_4_TCPWM1_LINE2               =  9,       /* Digital Active - tcpwm[1].line[2]:0 */
    P0_4_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:4 */
    P0_4_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:4 */
    P0_4_LCD_COM4                   = 12,       /* Digital Deep Sleep - lcd.com[4] */
    P0_4_LCD_SEG4                   = 13,       /* Digital Deep Sleep - lcd.seg[4] */
    P0_4_SCB0_UART_RTS              = 18,       /* Digital Active - scb[0].uart_rts:0 */
    P0_4_SCB0_SPI_CLK               = 20,       /* Digital Active - scb[0].spi_clk:0 */
    P0_4_PERI_TR_IO_INPUT2          = 24,       /* Digital Active - peri.tr_io_input[2]:0 */
    P0_4_PERI_TR_IO_OUTPUT0         = 25,       /* Digital Active - peri.tr_io_output[0]:2 */

    /* P0.5 */
    P0_5_GPIO                       =  0,       /* GPIO controls 'out' */
    P0_5_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P0_5_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P0_5_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P0_5_AMUXA                      =  4,       /* Analog mux bus A */
    P0_5_AMUXB                      =  5,       /* Analog mux bus B */
    P0_5_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P0_5_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P0_5_TCPWM0_LINE_COMPL2         =  8,       /* Digital Active - tcpwm[0].line_compl[2]:0 */
    P0_5_TCPWM1_LINE_COMPL2         =  9,       /* Digital Active - tcpwm[1].line_compl[2]:0 */
    P0_5_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:5 */
    P0_5_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:5 */
    P0_5_LCD_COM5                   = 12,       /* Digital Deep Sleep - lcd.com[5] */
    P0_5_LCD_SEG5                   = 13,       /* Digital Deep Sleep - lcd.seg[5] */
    P0_5_SRSS_EXT_CLK               = 16,       /* Digital Active - srss.ext_clk:1 */
    P0_5_SCB0_UART_CTS              = 18,       /* Digital Active - scb[0].uart_cts:0 */
    P0_5_SCB0_SPI_SELECT0           = 20,       /* Digital Active - scb[0].spi_select0:0 */
    P0_5_PERI_TR_IO_INPUT3          = 24,       /* Digital Active - peri.tr_io_input[3]:0 */
    P0_5_PERI_TR_IO_OUTPUT1         = 25,       /* Digital Active - peri.tr_io_output[1]:2 */

    /* P7.1 */
    P7_1_GPIO                       =  0,       /* GPIO controls 'out' */
    P7_1_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P7_1_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P7_1_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P7_1_AMUXA                      =  4,       /* Analog mux bus A */
    P7_1_AMUXB                      =  5,       /* Analog mux bus B */
    P7_1_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P7_1_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P7_1_TCPWM0_LINE_COMPL0         =  8,       /* Digital Active - tcpwm[0].line_compl[0]:4 */
    P7_1_TCPWM1_LINE_COMPL0         =  9,       /* Digital Active - tcpwm[1].line_compl[0]:2 */
    P7_1_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:33 */
    P7_1_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:33 */
    P7_1_LCD_COM33                  = 12,       /* Digital Deep Sleep - lcd.com[33] */
    P7_1_LCD_SEG33                  = 13,       /* Digital Deep Sleep - lcd.seg[33] */
    P7_1_SCB4_UART_TX               = 18,       /* Digital Active - scb[4].uart_tx:0 */
    P7_1_SCB4_I2C_SDA               = 19,       /* Digital Active - scb[4].i2c_sda:0 */
    P7_1_SCB4_SPI_MISO              = 20,       /* Digital Active - scb[4].spi_miso:0 */
    P7_1_PERI_TR_IO_INPUT15         = 24,       /* Digital Active - peri.tr_io_input[15]:0 */

    /* P7.2 */
    P7_2_GPIO                       =  0,       /* GPIO controls 'out' */
    P7_2_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P7_2_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P7_2_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P7_2_AMUXA                      =  4,       /* Analog mux bus A */
    P7_2_AMUXB                      =  5,       /* Analog mux bus B */
    P7_2_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P7_2_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P7_2_TCPWM0_LINE1               =  8,       /* Digital Active - tcpwm[0].line[1]:4 */
    P7_2_TCPWM1_LINE1               =  9,       /* Digital Active - tcpwm[1].line[1]:2 */
    P7_2_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:34 */
    P7_2_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:34 */
    P7_2_LCD_COM34                  = 12,       /* Digital Deep Sleep - lcd.com[34] */
    P7_2_LCD_SEG34                  = 13,       /* Digital Deep Sleep - lcd.seg[34] */
    P7_2_SCB4_UART_RTS              = 18,       /* Digital Active - scb[4].uart_rts:0 */
    P7_2_SCB4_SPI_CLK               = 20,       /* Digital Active - scb[4].spi_clk:0 */

    /* P7.3 */
    P7_3_GPIO                       =  0,       /* GPIO controls 'out' */
    P7_3_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P7_3_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P7_3_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P7_3_AMUXA                      =  4,       /* Analog mux bus A */
    P7_3_AMUXB                      =  5,       /* Analog mux bus B */
    P7_3_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P7_3_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P7_3_TCPWM0_LINE_COMPL1         =  8,       /* Digital Active - tcpwm[0].line_compl[1]:4 */
    P7_3_TCPWM1_LINE_COMPL1         =  9,       /* Digital Active - tcpwm[1].line_compl[1]:2 */
    P7_3_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:35 */
    P7_3_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:35 */
    P7_3_LCD_COM35                  = 12,       /* Digital Deep Sleep - lcd.com[35] */
    P7_3_LCD_SEG35                  = 13,       /* Digital Deep Sleep - lcd.seg[35] */
    P7_3_SCB4_UART_CTS              = 18,       /* Digital Active - scb[4].uart_cts:0 */
    P7_3_SCB4_SPI_SELECT0           = 20,       /* Digital Active - scb[4].spi_select0:0 */

    /* P7.4 */
    P7_4_GPIO                       =  0,       /* GPIO controls 'out' */
    P7_4_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P7_4_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P7_4_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P7_4_AMUXA                      =  4,       /* Analog mux bus A */
    P7_4_AMUXB                      =  5,       /* Analog mux bus B */
    P7_4_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P7_4_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P7_4_TCPWM0_LINE2               =  8,       /* Digital Active - tcpwm[0].line[2]:4 */
    P7_4_TCPWM1_LINE2               =  9,       /* Digital Active - tcpwm[1].line[2]:2 */
    P7_4_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:36 */
    P7_4_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:36 */
    P7_4_LCD_COM36                  = 12,       /* Digital Deep Sleep - lcd.com[36] */
    P7_4_LCD_SEG36                  = 13,       /* Digital Deep Sleep - lcd.seg[36] */
    P7_4_SCB4_SPI_SELECT1           = 20,       /* Digital Active - scb[4].spi_select1:0 */
    P7_4_BLESS_EXT_LNA_RX_CTL_OUT   = 26,       /* Digital Active - bless.ext_lna_rx_ctl_out */
    P7_4_CPUSS_TRACE_DATA3          = 27,       /* Digital Active - cpuss.trace_data[3]:2 */

    /* P7.5 */
    P7_5_GPIO                       =  0,       /* GPIO controls 'out' */
    P7_5_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P7_5_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P7_5_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P7_5_AMUXA                      =  4,       /* Analog mux bus A */
    P7_5_AMUXB                      =  5,       /* Analog mux bus B */
    P7_5_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P7_5_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P7_5_TCPWM0_LINE_COMPL2         =  8,       /* Digital Active - tcpwm[0].line_compl[2]:4 */
    P7_5_TCPWM1_LINE_COMPL2         =  9,       /* Digital Active - tcpwm[1].line_compl[2]:2 */
    P7_5_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:37 */
    P7_5_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:37 */
    P7_5_LCD_COM37                  = 12,       /* Digital Deep Sleep - lcd.com[37] */
    P7_5_LCD_SEG37                  = 13,       /* Digital Deep Sleep - lcd.seg[37] */
    P7_5_SCB4_SPI_SELECT2           = 20,       /* Digital Active - scb[4].spi_select2:0 */
    P7_5_BLESS_EXT_PA_TX_CTL_OUT    = 26,       /* Digital Active - bless.ext_pa_tx_ctl_out */
    P7_5_CPUSS_TRACE_DATA2          = 27,       /* Digital Active - cpuss.trace_data[2]:2 */

    /* P7.6 */
    P7_6_GPIO                       =  0,       /* GPIO controls 'out' */
    P7_6_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P7_6_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P7_6_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P7_6_AMUXA                      =  4,       /* Analog mux bus A */
    P7_6_AMUXB                      =  5,       /* Analog mux bus B */
    P7_6_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P7_6_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P7_6_TCPWM0_LINE3               =  8,       /* Digital Active - tcpwm[0].line[3]:4 */
    P7_6_TCPWM1_LINE3               =  9,       /* Digital Active - tcpwm[1].line[3]:2 */
    P7_6_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:38 */
    P7_6_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:38 */
    P7_6_LCD_COM38                  = 12,       /* Digital Deep Sleep - lcd.com[38] */
    P7_6_LCD_SEG38                  = 13,       /* Digital Deep Sleep - lcd.seg[38] */
    P7_6_SCB4_SPI_SELECT3           = 20,       /* Digital Active - scb[4].spi_select3:0 */
    P7_6_BLESS_EXT_PA_LNA_CHIP_EN_OUT = 26,     /* Digital Active - bless.ext_pa_lna_chip_en_out */
    P7_6_CPUSS_TRACE_DATA1          = 27,       /* Digital Active - cpuss.trace_data[1]:2 */

    /* P7.7 */
    P7_7_GPIO                       =  0,       /* GPIO controls 'out' */
    P7_7_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P7_7_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P7_7_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P7_7_AMUXA                      =  4,       /* Analog mux bus A */
    P7_7_AMUXB                      =  5,       /* Analog mux bus B */
    P7_7_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P7_7_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P7_7_TCPWM0_LINE_COMPL3         =  8,       /* Digital Active - tcpwm[0].line_compl[3]:4 */
    P7_7_TCPWM1_LINE_COMPL3         =  9,       /* Digital Active - tcpwm[1].line_compl[3]:2 */
    P7_7_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:39 */
    P7_7_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:39 */
    P7_7_LCD_COM39                  = 12,       /* Digital Deep Sleep - lcd.com[39] */
    P7_7_LCD_SEG39                  = 13,       /* Digital Deep Sleep - lcd.seg[39] */
    P7_7_CPUSS_CLK_FM_PUMP          = 21,       /* Digital Active - cpuss.clk_fm_pump */
    P7_7_CPUSS_TRACE_DATA0          = 27,       /* Digital Active - cpuss.trace_data[0]:2 */

    /* P8.0 */
    P8_0_GPIO                       =  0,       /* GPIO controls 'out' */
    P8_0_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P8_0_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P8_0_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P8_0_AMUXA                      =  4,       /* Analog mux bus A */
    P8_0_AMUXB                      =  5,       /* Analog mux bus B */
    P8_0_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P8_0_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P8_0_TCPWM0_LINE0               =  8,       /* Digital Active - tcpwm[0].line[0]:5 */
    P8_0_TCPWM1_LINE4               =  9,       /* Digital Active - tcpwm[1].line[4]:2 */
    P8_0_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:40 */
    P8_0_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:40 */
    P8_0_LCD_COM40                  = 12,       /* Digital Deep Sleep - lcd.com[40] */
    P8_0_LCD_SEG40                  = 13,       /* Digital Deep Sleep - lcd.seg[40] */
    P8_0_SCB4_UART_RX               = 18,       /* Digital Active - scb[4].uart_rx:1 */
    P8_0_SCB4_I2C_SCL               = 19,       /* Digital Active - scb[4].i2c_scl:1 */
    P8_0_PERI_TR_IO_INPUT16         = 24,       /* Digital Active - peri.tr_io_input[16]:0 */

    /* P8.1 */
    P8_1_GPIO                       =  0,       /* GPIO controls 'out' */
    P8_1_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P8_1_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P8_1_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P8_1_AMUXA                      =  4,       /* Analog mux bus A */
    P8_1_AMUXB                      =  5,       /* Analog mux bus B */
    P8_1_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P8_1_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P8_1_TCPWM0_LINE_COMPL0         =  8,       /* Digital Active - tcpwm[0].line_compl[0]:5 */
    P8_1_TCPWM1_LINE_COMPL4         =  9,       /* Digital Active - tcpwm[1].line_compl[4]:2 */
    P8_1_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:41 */
    P8_1_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:41 */
    P8_1_LCD_COM41                  = 12,       /* Digital Deep Sleep - lcd.com[41] */
    P8_1_LCD_SEG41                  = 13,       /* Digital Deep Sleep - lcd.seg[41] */
    P8_1_LPCOMP_DSI_COMP1           = 15,       /* Digital Deep Sleep - lpcomp.dsi_comp1:0 */
    P8_1_SCB4_UART_TX               = 18,       /* Digital Active - scb[4].uart_tx:1 */
    P8_1_SCB4_I2C_SDA               = 19,       /* Digital Active - scb[4].i2c_sda:1 */
    P8_1_PERI_TR_IO_INPUT17         = 24,       /* Digital Active - peri.tr_io_input[17]:0 */

    /* P8.2 */
    P8_2_GPIO                       =  0,       /* GPIO controls 'out' */
    P8_2_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P8_2_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P8_2_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P8_2_AMUXA                      =  4,       /* Analog mux bus A */
    P8_2_AMUXB                      =  5,       /* Analog mux bus B */
    P8_2_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P8_2_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P8_2_TCPWM0_LINE1               =  8,       /* Digital Active - tcpwm[0].line[1]:5 */
    P8_2_TCPWM1_LINE5               =  9,       /* Digital Active - tcpwm[1].line[5]:2 */
    P8_2_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:42 */
    P8_2_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:42 */
    P8_2_LCD_COM42                  = 12,       /* Digital Deep Sleep - lcd.com[42] */
    P8_2_LCD_SEG42                  = 13,       /* Digital Deep Sleep - lcd.seg[42] */
    P8_2_LPCOMP_DSI_COMP0           = 15,       /* Digital Deep Sleep - lpcomp.dsi_comp0:0 */

    /* P9.0 */
    P9_0_GPIO                       =  0,       /* GPIO controls 'out' */
    P9_0_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P9_0_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P9_0_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P9_0_AMUXA                      =  4,       /* Analog mux bus A */
    P9_0_AMUXB                      =  5,       /* Analog mux bus B */
    P9_0_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P9_0_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P9_0_TCPWM0_LINE_COMPL1         =  8,       /* Digital Active - tcpwm[0].line_compl[1]:5 */
    P9_0_TCPWM1_LINE_COMPL5         =  9,       /* Digital Active - tcpwm[1].line_compl[5]:2 */
    P9_0_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:43 */
    P9_0_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:43 */
    P9_0_LCD_COM43                  = 12,       /* Digital Deep Sleep - lcd.com[43] */
    P9_0_LCD_SEG43                  = 13,       /* Digital Deep Sleep - lcd.seg[43] */
    P9_0_SCB2_UART_RX               = 18,       /* Digital Active - scb[2].uart_rx:1 */
    P9_0_SCB2_I2C_SCL               = 19,       /* Digital Active - scb[2].i2c_scl:1 */
    P9_0_SCB2_SPI_MOSI              = 20,       /* Digital Active - scb[2].spi_mosi:1 */
    P9_0_PERI_TR_IO_INPUT18         = 24,       /* Digital Active - peri.tr_io_input[18]:0 */
    P9_0_CPUSS_TRACE_DATA3          = 27,       /* Digital Active - cpuss.trace_data[3]:0 */

    /* P9.1 */
    P9_1_GPIO                       =  0,       /* GPIO controls 'out' */
    P9_1_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P9_1_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P9_1_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P9_1_AMUXA                      =  4,       /* Analog mux bus A */
    P9_1_AMUXB                      =  5,       /* Analog mux bus B */
    P9_1_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P9_1_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P9_1_TCPWM0_LINE2               =  8,       /* Digital Active - tcpwm[0].line[2]:5 */
    P9_1_TCPWM1_LINE6               =  9,       /* Digital Active - tcpwm[1].line[6]:2 */
    P9_1_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:44 */
    P9_1_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:44 */
    P9_1_LCD_COM44                  = 12,       /* Digital Deep Sleep - lcd.com[44] */
    P9_1_LCD_SEG44                  = 13,       /* Digital Deep Sleep - lcd.seg[44] */
    P9_1_SCB2_UART_TX               = 18,       /* Digital Active - scb[2].uart_tx:1 */
    P9_1_SCB2_I2C_SDA               = 19,       /* Digital Active - scb[2].i2c_sda:1 */
    P9_1_SCB2_SPI_MISO              = 20,       /* Digital Active - scb[2].spi_miso:1 */
    P9_1_PERI_TR_IO_INPUT19         = 24,       /* Digital Active - peri.tr_io_input[19]:0 */
    P9_1_CPUSS_TRACE_DATA2          = 27,       /* Digital Active - cpuss.trace_data[2]:0 */
    P9_1_SRSS_DDFT_PIN_IN0          = 31,       /* Digital Deep Sleep - srss.ddft_pin_in[0]:1 */

    /* P9.2 */
    P9_2_GPIO                       =  0,       /* GPIO controls 'out' */
    P9_2_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P9_2_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P9_2_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P9_2_AMUXA                      =  4,       /* Analog mux bus A */
    P9_2_AMUXB                      =  5,       /* Analog mux bus B */
    P9_2_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P9_2_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P9_2_TCPWM0_LINE_COMPL2         =  8,       /* Digital Active - tcpwm[0].line_compl[2]:5 */
    P9_2_TCPWM1_LINE_COMPL6         =  9,       /* Digital Active - tcpwm[1].line_compl[6]:2 */
    P9_2_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:45 */
    P9_2_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:45 */
    P9_2_LCD_COM45                  = 12,       /* Digital Deep Sleep - lcd.com[45] */
    P9_2_LCD_SEG45                  = 13,       /* Digital Deep Sleep - lcd.seg[45] */
    P9_2_SCB2_UART_RTS              = 18,       /* Digital Active - scb[2].uart_rts:1 */
    P9_2_SCB2_SPI_CLK               = 20,       /* Digital Active - scb[2].spi_clk:1 */
    P9_2_CANFD0_TTCAN_RX0           = 23,       /* Digital Active - canfd[0].ttcan_rx[0] */
    P9_2_CPUSS_TRACE_DATA1          = 27,       /* Digital Active - cpuss.trace_data[1]:0 */

    /* P9.3 */
    P9_3_GPIO                       =  0,       /* GPIO controls 'out' */
    P9_3_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P9_3_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P9_3_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P9_3_AMUXA                      =  4,       /* Analog mux bus A */
    P9_3_AMUXB                      =  5,       /* Analog mux bus B */
    P9_3_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P9_3_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P9_3_TCPWM0_LINE3               =  8,       /* Digital Active - tcpwm[0].line[3]:5 */
    P9_3_TCPWM1_LINE7               =  9,       /* Digital Active - tcpwm[1].line[7]:2 */
    P9_3_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:46 */
    P9_3_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:46 */
    P9_3_LCD_COM46                  = 12,       /* Digital Deep Sleep - lcd.com[46] */
    P9_3_LCD_SEG46                  = 13,       /* Digital Deep Sleep - lcd.seg[46] */
    P9_3_SCB2_UART_CTS              = 18,       /* Digital Active - scb[2].uart_cts:1 */
    P9_3_SCB2_SPI_SELECT0           = 20,       /* Digital Active - scb[2].spi_select0:1 */
    P9_3_CANFD0_TTCAN_TX0           = 23,       /* Digital Active - canfd[0].ttcan_tx[0] */
    P9_3_CPUSS_TRACE_DATA0          = 27,       /* Digital Active - cpuss.trace_data[0]:0 */
    P9_3_SRSS_DDFT_PIN_IN1          = 31,       /* Digital Deep Sleep - srss.ddft_pin_in[1]:1 */

    /* P10.0 */
    P10_0_GPIO                      =  0,       /* GPIO controls 'out' */
    P10_0_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P10_0_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P10_0_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P10_0_AMUXA                     =  4,       /* Analog mux bus A */
    P10_0_AMUXB                     =  5,       /* Analog mux bus B */
    P10_0_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P10_0_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P10_0_TCPWM0_LINE_COMPL3        =  8,       /* Digital Active - tcpwm[0].line_compl[3]:5 */
    P10_0_TCPWM1_LINE_COMPL7        =  9,       /* Digital Active - tcpwm[1].line_compl[7]:2 */
    P10_0_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:47 */
    P10_0_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:47 */
    P10_0_LCD_COM47                 = 12,       /* Digital Deep Sleep - lcd.com[47] */
    P10_0_LCD_SEG47                 = 13,       /* Digital Deep Sleep - lcd.seg[47] */
    P10_0_SCB1_UART_RX              = 18,       /* Digital Active - scb[1].uart_rx:1 */
    P10_0_SCB1_I2C_SCL              = 19,       /* Digital Active - scb[1].i2c_scl:1 */
    P10_0_SCB1_SPI_MOSI             = 20,       /* Digital Active - scb[1].spi_mosi:1 */
    P10_0_PERI_TR_IO_INPUT20        = 24,       /* Digital Active - peri.tr_io_input[20]:0 */

    /* P10.1 */
    P10_1_GPIO                      =  0,       /* GPIO controls 'out' */
    P10_1_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P10_1_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P10_1_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P10_1_AMUXA                     =  4,       /* Analog mux bus A */
    P10_1_AMUXB                     =  5,       /* Analog mux bus B */
    P10_1_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P10_1_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P10_1_TCPWM0_LINE0              =  8,       /* Digital Active - tcpwm[0].line[0]:6 */
    P10_1_TCPWM1_LINE0              =  9,       /* Digital Active - tcpwm[1].line[0]:3 */
    P10_1_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:48 */
    P10_1_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:48 */
    P10_1_LCD_COM48                 = 12,       /* Digital Deep Sleep - lcd.com[48] */
    P10_1_LCD_SEG48                 = 13,       /* Digital Deep Sleep - lcd.seg[48] */
    P10_1_SCB1_UART_TX              = 18,       /* Digital Active - scb[1].uart_tx:1 */
    P10_1_SCB1_I2C_SDA              = 19,       /* Digital Active - scb[1].i2c_sda:1 */
    P10_1_SCB1_SPI_MISO             = 20,       /* Digital Active - scb[1].spi_miso:1 */
    P10_1_PERI_TR_IO_INPUT21        = 24,       /* Digital Active - peri.tr_io_input[21]:0 */

    /* P11.0 */
    P11_0_GPIO                      =  0,       /* GPIO controls 'out' */
    P11_0_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P11_0_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P11_0_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P11_0_AMUXA                     =  4,       /* Analog mux bus A */
    P11_0_AMUXB                     =  5,       /* Analog mux bus B */
    P11_0_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P11_0_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P11_0_TCPWM0_LINE2              =  8,       /* Digital Active - tcpwm[0].line[2]:6 */
    P11_0_TCPWM1_LINE2              =  9,       /* Digital Active - tcpwm[1].line[2]:3 */
    P11_0_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:52 */
    P11_0_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:52 */
    P11_0_LCD_COM52                 = 12,       /* Digital Deep Sleep - lcd.com[52] */
    P11_0_LCD_SEG52                 = 13,       /* Digital Deep Sleep - lcd.seg[52] */
    P11_0_SMIF_SPI_SELECT2          = 17,       /* Digital Active - smif.spi_select2 */
    P11_0_SCB5_UART_RX              = 18,       /* Digital Active - scb[5].uart_rx:0 */
    P11_0_SCB5_I2C_SCL              = 19,       /* Digital Active - scb[5].i2c_scl:0 */
    P11_0_SCB5_SPI_MOSI             = 20,       /* Digital Active - scb[5].spi_mosi:0 */
    P11_0_PERI_TR_IO_INPUT22        = 24,       /* Digital Active - peri.tr_io_input[22]:0 */

    /* P11.1 */
    P11_1_GPIO                      =  0,       /* GPIO controls 'out' */
    P11_1_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P11_1_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P11_1_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P11_1_AMUXA                     =  4,       /* Analog mux bus A */
    P11_1_AMUXB                     =  5,       /* Analog mux bus B */
    P11_1_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P11_1_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P11_1_TCPWM0_LINE_COMPL2        =  8,       /* Digital Active - tcpwm[0].line_compl[2]:6 */
    P11_1_TCPWM1_LINE_COMPL2        =  9,       /* Digital Active - tcpwm[1].line_compl[2]:3 */
    P11_1_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:53 */
    P11_1_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:53 */
    P11_1_LCD_COM53                 = 12,       /* Digital Deep Sleep - lcd.com[53] */
    P11_1_LCD_SEG53                 = 13,       /* Digital Deep Sleep - lcd.seg[53] */
    P11_1_SMIF_SPI_SELECT1          = 17,       /* Digital Active - smif.spi_select1 */
    P11_1_SCB5_UART_TX              = 18,       /* Digital Active - scb[5].uart_tx:0 */
    P11_1_SCB5_I2C_SDA              = 19,       /* Digital Active - scb[5].i2c_sda:0 */
    P11_1_SCB5_SPI_MISO             = 20,       /* Digital Active - scb[5].spi_miso:0 */
    P11_1_PERI_TR_IO_INPUT23        = 24,       /* Digital Active - peri.tr_io_input[23]:0 */

    /* P11.2 */
    P11_2_GPIO                      =  0,       /* GPIO controls 'out' */
    P11_2_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P11_2_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P11_2_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P11_2_AMUXA                     =  4,       /* Analog mux bus A */
    P11_2_AMUXB                     =  5,       /* Analog mux bus B */
    P11_2_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P11_2_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P11_2_TCPWM0_LINE3              =  8,       /* Digital Active - tcpwm[0].line[3]:6 */
    P11_2_TCPWM1_LINE3              =  9,       /* Digital Active - tcpwm[1].line[3]:3 */
    P11_2_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:54 */
    P11_2_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:54 */
    P11_2_LCD_COM54                 = 12,       /* Digital Deep Sleep - lcd.com[54] */
    P11_2_LCD_SEG54                 = 13,       /* Digital Deep Sleep - lcd.seg[54] */
    P11_2_SMIF_SPI_SELECT0          = 17,       /* Digital Active - smif.spi_select0 */
    P11_2_SCB5_UART_RTS             = 18,       /* Digital Active - scb[5].uart_rts:0 */
    P11_2_SCB5_SPI_CLK              = 20,       /* Digital Active - scb[5].spi_clk:0 */

    /* P11.3 */
    P11_3_GPIO                      =  0,       /* GPIO controls 'out' */
    P11_3_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P11_3_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P11_3_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P11_3_AMUXA                     =  4,       /* Analog mux bus A */
    P11_3_AMUXB                     =  5,       /* Analog mux bus B */
    P11_3_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P11_3_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P11_3_TCPWM0_LINE_COMPL3        =  8,       /* Digital Active - tcpwm[0].line_compl[3]:6 */
    P11_3_TCPWM1_LINE_COMPL3        =  9,       /* Digital Active - tcpwm[1].line_compl[3]:3 */
    P11_3_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:55 */
    P11_3_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:55 */
    P11_3_LCD_COM55                 = 12,       /* Digital Deep Sleep - lcd.com[55] */
    P11_3_LCD_SEG55                 = 13,       /* Digital Deep Sleep - lcd.seg[55] */
    P11_3_SMIF_SPI_DATA3            = 17,       /* Digital Active - smif.spi_data3 */
    P11_3_SCB5_UART_CTS             = 18,       /* Digital Active - scb[5].uart_cts:0 */
    P11_3_SCB5_SPI_SELECT0          = 20,       /* Digital Active - scb[5].spi_select0:0 */
    P11_3_PERI_TR_IO_OUTPUT0        = 25,       /* Digital Active - peri.tr_io_output[0]:0 */

    /* P11.4 */
    P11_4_GPIO                      =  0,       /* GPIO controls 'out' */
    P11_4_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P11_4_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P11_4_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P11_4_AMUXA                     =  4,       /* Analog mux bus A */
    P11_4_AMUXB                     =  5,       /* Analog mux bus B */
    P11_4_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P11_4_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P11_4_TCPWM0_LINE0              =  8,       /* Digital Active - tcpwm[0].line[0]:7 */
    P11_4_TCPWM1_LINE4              =  9,       /* Digital Active - tcpwm[1].line[4]:3 */
    P11_4_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:56 */
    P11_4_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:56 */
    P11_4_LCD_COM56                 = 12,       /* Digital Deep Sleep - lcd.com[56] */
    P11_4_LCD_SEG56                 = 13,       /* Digital Deep Sleep - lcd.seg[56] */
    P11_4_SMIF_SPI_DATA2            = 17,       /* Digital Active - smif.spi_data2 */
    P11_4_SCB1_UART_RX              = 18,       /* Digital Active - scb[1].uart_rx:2 */
    P11_4_SCB1_I2C_SCL              = 19,       /* Digital Active - scb[1].i2c_scl:2 */
    P11_4_SCB1_SPI_MOSI             = 20,       /* Digital Active - scb[1].spi_mosi:2 */
    P11_4_PERI_TR_IO_OUTPUT1        = 25,       /* Digital Active - peri.tr_io_output[1]:0 */

    /* P11.5 */
    P11_5_GPIO                      =  0,       /* GPIO controls 'out' */
    P11_5_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P11_5_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P11_5_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P11_5_AMUXA                     =  4,       /* Analog mux bus A */
    P11_5_AMUXB                     =  5,       /* Analog mux bus B */
    P11_5_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P11_5_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P11_5_TCPWM0_LINE_COMPL0        =  8,       /* Digital Active - tcpwm[0].line_compl[0]:7 */
    P11_5_TCPWM1_LINE_COMPL4        =  9,       /* Digital Active - tcpwm[1].line_compl[4]:3 */
    P11_5_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:57 */
    P11_5_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:57 */
    P11_5_LCD_COM57                 = 12,       /* Digital Deep Sleep - lcd.com[57] */
    P11_5_LCD_SEG57                 = 13,       /* Digital Deep Sleep - lcd.seg[57] */
    P11_5_SMIF_SPI_DATA1            = 17,       /* Digital Active - smif.spi_data1 */
    P11_5_SCB1_UART_TX              = 18,       /* Digital Active - scb[1].uart_tx:2 */
    P11_5_SCB1_I2C_SDA              = 19,       /* Digital Active - scb[1].i2c_sda:2 */
    P11_5_SCB1_SPI_MISO             = 20,       /* Digital Active - scb[1].spi_miso:2 */

    /* P11.6 */
    P11_6_GPIO                      =  0,       /* GPIO controls 'out' */
    P11_6_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P11_6_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P11_6_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P11_6_AMUXA                     =  4,       /* Analog mux bus A */
    P11_6_AMUXB                     =  5,       /* Analog mux bus B */
    P11_6_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P11_6_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P11_6_TCPWM0_LINE1              =  8,       /* Digital Active - tcpwm[0].line[1]:7 */
    P11_6_TCPWM1_LINE5              =  9,       /* Digital Active - tcpwm[1].line[5]:3 */
    P11_6_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:58 */
    P11_6_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:58 */
    P11_6_LCD_COM58                 = 12,       /* Digital Deep Sleep - lcd.com[58] */
    P11_6_LCD_SEG58                 = 13,       /* Digital Deep Sleep - lcd.seg[58] */
    P11_6_SMIF_SPI_DATA0            = 17,       /* Digital Active - smif.spi_data0 */
    P11_6_SCB1_UART_RTS             = 18,       /* Digital Active - scb[1].uart_rts:1 */
    P11_6_SCB1_SPI_CLK              = 20,       /* Digital Active - scb[1].spi_clk:2 */

    /* P11.7 */
    P11_7_GPIO                      =  0,       /* GPIO controls 'out' */
    P11_7_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P11_7_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P11_7_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P11_7_AMUXA                     =  4,       /* Analog mux bus A */
    P11_7_AMUXB                     =  5,       /* Analog mux bus B */
    P11_7_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P11_7_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P11_7_TCPWM0_LINE_COMPL1        =  8,       /* Digital Active - tcpwm[0].line_compl[1]:7 */
    P11_7_TCPWM1_LINE_COMPL5        =  9,       /* Digital Active - tcpwm[1].line_compl[5]:3 */
    P11_7_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:59 */
    P11_7_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:59 */
    P11_7_LCD_COM59                 = 12,       /* Digital Deep Sleep - lcd.com[59] */
    P11_7_LCD_SEG59                 = 13,       /* Digital Deep Sleep - lcd.seg[59] */
    P11_7_SMIF_SPI_CLK              = 17,       /* Digital Active - smif.spi_clk */
    P11_7_SCB1_UART_CTS             = 18,       /* Digital Active - scb[1].uart_cts:1 */
    P11_7_SCB1_SPI_SELECT0          = 20,       /* Digital Active - scb[1].spi_select0:2 */

    /* P12.6 */
    P12_6_GPIO                      =  0,       /* GPIO controls 'out' */
    P12_6_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P12_6_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P12_6_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P12_6_AMUXA                     =  4,       /* Analog mux bus A */
    P12_6_AMUXB                     =  5,       /* Analog mux bus B */
    P12_6_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P12_6_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P12_6_TCPWM0_LINE3              =  8,       /* Digital Active - tcpwm[0].line[3]:7 */
    P12_6_TCPWM1_LINE7              =  9,       /* Digital Active - tcpwm[1].line[7]:3 */
    P12_6_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:62 */
    P12_6_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:62 */
    P12_6_LCD_COM2                  = 12,       /* Digital Deep Sleep - lcd.com[2]:1 */
    P12_6_LCD_SEG2                  = 13,       /* Digital Deep Sleep - lcd.seg[2]:1 */

    /* P12.7 */
    P12_7_GPIO                      =  0,       /* GPIO controls 'out' */
    P12_7_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P12_7_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P12_7_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P12_7_AMUXA                     =  4,       /* Analog mux bus A */
    P12_7_AMUXB                     =  5,       /* Analog mux bus B */
    P12_7_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P12_7_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P12_7_TCPWM0_LINE_COMPL3        =  8,       /* Digital Active - tcpwm[0].line_compl[3]:7 */
    P12_7_TCPWM1_LINE_COMPL7        =  9,       /* Digital Active - tcpwm[1].line_compl[7]:3 */
    P12_7_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:63 */
    P12_7_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:63 */
    P12_7_LCD_COM3                  = 12,       /* Digital Deep Sleep - lcd.com[3]:1 */
    P12_7_LCD_SEG3                  = 13        /* Digital Deep Sleep - lcd.seg[3]:1 */
} en_hsiom_sel_t;

#endif /* _GPIO_PSOC6_03_68_QFN_BLE_H_ */


/* [] END OF FILE */
