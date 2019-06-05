/***************************************************************************//**
* \file cy_seglcd.h
* \version 1.0
*
* \brief
* Provides an API declaration of the Segment LCD driver.
*
********************************************************************************
* \copyright
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

/**
* \addtogroup group_seglcd
* \{
* The Segment LCD driver provides an API to configure and access the MXLCD hardware.
*
* The Segment LCD solution (driver + HW) can directly drive a variety of LCD glass
* at different voltage levels with multiplex ratios.
* The driver provides an easy method of configuring the MXLCD block to drive your
* custom or standard glass.
*
* Features:
* * Digital Correlation and PWM at 1/2, 1/3, 1/4 and at 1/5 bias modes supported
* * Drive STN/TN LCD glass with up to eight COMs (device specific)
* * Four priority levels for each channel
* * 30 to 150 Hz refresh rate
* * Supports both type A (standard) and type B (low power) waveforms
* * Pixel state of the display may be inverted for a negative image
* * Operation in Deep Sleep Mode from the ILO
* * All-digital contrast control using "Dead Period" digital modulation
* * Set of basic standard displays and fonts.
* * Customizable display and font structure.
*
* \section group_seglcd_configuration Configuration Considerations
*
* To start working with an LCD first initialize an MXLCD block, then initialize
* a frame buffer and then enable the block:
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_Config
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_Init
*
* Now the display structure is needed:
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_Display
*
* And now you can write characters, multi-digit numbers and strings onto the display:
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_Char
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_DecNum
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_HexNum
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_String
*
* Or even manage separate LCD pixels:
* \snippet seglcd/snippet/SegLCD_Snpt.h snippet_Cy_SegLCD_DefPixel
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_ActPixel
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_SetPixel
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_ClrPixel
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_InvPixel
*
* You can also customize basic fonts, for example:
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_CustomAsciiFont7seg
*
* You can also customize or create your own displays, for example:
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_Custom3x5
*
* And also different fonts for them:
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_CustomFont3x5
*
*
* \section group_seglcd_more_information More Information
* Refer to the technical reference manual (TRM) and the device datasheet.
*
* \section group_seglcd_MISRA MISRA-C Compliance
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>10.3</td>
*     <td>R</td>
*     <td>The value of a complex expression of integer type shall only be cast to a type of the
*         same signedness that is no wider than the underlying type of the expression.</td>
*     <td>The value got from the bitfield physically cannot exceed the enumeration
*         that describes this bitfield. So, the code is safe by design.</td>
*   </tr>
*   <tr>
*     <td>11.4</td>
*     <td>A</td>
*     <td>A cast should not be performed between a pointer to object type and
*         a different pointer to object type.</td>
*     <td>This is a fix for GCC compiler warning.</td>
*   </tr>
*   <tr>
*     <td>14.1</td>
*     <td>R</td>
*     <td>There shall be no unreachable code.</td>
*     <td>This is HOBTO parameter reading for future features support.</td>
*   </tr>
* </table>
*
* \section group_seglcd_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_seglcd_macros    Macros
* \defgroup group_seglcd_functions Functions
* \{
*   \defgroup group_seglcd_functions_config  Block Configuration Functions
*   \defgroup group_seglcd_functions_frame   Frame/Pixel Management Functions
*   \defgroup group_seglcd_functions_display Display/Character Management Functions
* \}
* \defgroup group_seglcd_data_structures  Data Structures
* \defgroup group_seglcd_globals Global   Data
* \defgroup group_seglcd_enums Enumerated Types
*/

#if !defined(CY_SEGLCD_H)
#define CY_SEGLCD_H

#include "cy_syslib.h"

#ifdef CY_IP_MXLCD

#if defined(__cplusplus)
extern "C" {
#endif

/** \addtogroup group_seglcd_macros
* \{
*/

/** Driver major version */
#define CY_SEGLCD_DRV_VERSION_MAJOR         1

/** Driver minor version */
#define CY_SEGLCD_DRV_VERSION_MINOR         0

/** SegLCD driver ID */
#define CY_SEGLCD_ID                        (CY_PDL_DRV_ID(0x40u))

/** Specifies the pixel in the display array which is not connected */
#define CY_SEGLCD_NOT_CON                   (0xFFFFFFFFUL)

/** \cond internal */
#define CY_SEGLCD_OCTET                     (8UL)
#define CY_SEGLCD_COM_Pos                   (16U)
#define CY_SEGLCD_COM_Msk                   ((uint32_t) 0xFUL << CY_SEGLCD_COM_Pos)
#define CY_SEGLCD_OCT_Pos                   (8U)
#define CY_SEGLCD_OCT_Msk                   ((uint32_t) 0x7UL << CY_SEGLCD_OCT_Pos)
#define CY_SEGLCD_NBL_Pos                   (0U)
#define CY_SEGLCD_NBL_Msk                   ((uint32_t) 0x7UL << CY_SEGLCD_NBL_Pos)

#define CY_SEGLCD_ITEM(trmNum, comLine)     (_VAL2FLD(CY_SEGLCD_COM, (comLine))                  | \
                                             _VAL2FLD(CY_SEGLCD_OCT, (trmNum) / CY_SEGLCD_OCTET) | \
                                             _VAL2FLD(CY_SEGLCD_NBL, (trmNum) % CY_SEGLCD_OCTET))
/** \endcond */


/**
* Makes the pixel value from the LCD segment terminal number and the user defined common line number.
* It should be used to display pixMap array definitions for \ref cy_stc_seglcd_disp_t.
*/
#define CY_SEGLCD_PIXEL(segNum, comLine)    (CY_SEGLCD_ITEM(segNum, comLine))

/**
* Makes the common line value from the LCD common terminal number and the user defined common line number.
* It should be used for common lines array definitions for \ref Cy_SegLCD_ClrFrame and \ref Cy_SegLCD_InvFrame.
*/
#define CY_SEGLCD_COMMON(comNum, comLine)   (CY_SEGLCD_ITEM(comNum, comLine))

/** Space/blank/empty symbol code for the numeric type of fonts */
#define CY_SEGLCD_NUM_BLANK                 ((char_t)16)

/** \} group_seglcd_macros */


/**
* \addtogroup group_seglcd_enums
* \{
*/

/** SegLCD driver error codes */
typedef enum 
{
    CY_SEGLCD_SUCCESS   = 0x0UL,                                         /**< Returned successful */
    CY_SEGLCD_BAD_PARAM = CY_SEGLCD_ID | CY_PDL_STATUS_ERROR | 0x01UL,   /**< A bad parameter was passed (display/font pointer is NULL,
                                                                          *   position is out of display area, etc.)
                                                                          */
    CY_SEGLCD_BAD_PIXEL = CY_SEGLCD_ID | CY_PDL_STATUS_ERROR | 0x02UL,   /**< The specified pixel value is wrong or the pixel is
                                                                          *   specified as 'not connected' in the display pixMap array.
                                                                          */
    CY_SEGLCD_BAD_CHAR  = CY_SEGLCD_ID | CY_PDL_STATUS_ERROR | 0x04UL,   /**< The specified character (or at least one of characters in the
                                                                          *   specified string) is not supported by the specified font.
                                                                          */
    CY_SEGLCD_EXCEED    = CY_SEGLCD_ID | CY_PDL_STATUS_WARNING | 0x08UL, /**< The string (length) or number (amount of dec/hex digits)
                                                                          *   exceeds the display limits.
                                                                          */
    CY_SEGLCD_CUSTOM    = CY_SEGLCD_ID | CY_PDL_STATUS_INFO | 0x10UL,    /**< The display/font type is custom
                                                                          *   (not defined by the \ref cy_en_seglcd_disp_t).
                                                                          */
} cy_en_seglcd_status_t;

/** SegLCD block speed mode */
typedef enum 
{
    CY_SEGLCD_SPEED_LOW  = 0x0UL, /**< Low Speed mode, works in Active, Sleep and DeepSleep power modes */
    CY_SEGLCD_SPEED_HIGH = 0x1UL  /**< High Speed mode, works in Active and Sleep power modes */
} cy_en_seglcd_speed_t;

/** SegLCD block low speed mode clock source selection  */
typedef enum
{
    CY_SEGLCD_LSCLK_LF = 0x0UL, /**< Low Frequency source, usually connected to ILO */
    CY_SEGLCD_LSCLK_MF = 0x1UL  /**< Middle Frequency source, usually connected to CLK_MF, \ref group_sysclk_mf_funcs */
} cy_en_seglcd_lsclk_t;

/** SegLCD driving waveform type */
typedef enum 
{
    CY_SEGLCD_TYPE_A = 0x0UL, /**< Type A - Each frame addresses each COM pin only once with a balanced (DC=0) waveform. */
    CY_SEGLCD_TYPE_B = 0x1UL  /**< Type B - Each frame addresses each COM pin twice in a sequence with a positive
                               *            and negative waveform that together are balanced (DC=0).
                               */
} cy_en_seglcd_wave_t;

/** SegLCD driving mode configuration */
typedef enum 
{
    CY_SEGLCD_PWM         = 0x0UL, /**< PWM mode. */
    CY_SEGLCD_CORRELATION = 0x1UL  /**< Digital Correlation mode. */
} cy_en_seglcd_drive_t;

/** SegLCD PWM bias selection */
typedef enum 
{
    CY_SEGLCD_BIAS_HALF   = 0x0UL, /**< 1/2 Bias. */
    CY_SEGLCD_BIAS_THIRD  = 0x1UL, /**< 1/3 Bias. */
    CY_SEGLCD_BIAS_FOURTH = 0x2UL, /**< 1/4 Bias (not supported for \ref CY_SEGLCD_SPEED_LOW mode in MXLCD_ver1). */
    CY_SEGLCD_BIAS_FIFTH  = 0x3UL, /**< 1/5 Bias (not supported for \ref CY_SEGLCD_SPEED_LOW mode in MXLCD_ver1). */
} cy_en_seglcd_bias_t;

/** SegLCD segment display types */
typedef enum
{
    CY_SEGLCD_BAR   = 1U,  /**< Bar Graph / Dial display */
    CY_SEGLCD_7SEG  = 7U,  /**< Seven-segment display */
    CY_SEGLCD_14SEG = 14U, /**< Fourteen-segment display */
    CY_SEGLCD_16SEG = 16U, /**< Sixteen-segment display */
    CY_SEGLCD_5X8DM = 40U  /**< Five-by-eight dot matrix display */
} cy_en_seglcd_disp_t;

/** \} group_seglcd_enums */


/** \addtogroup group_seglcd_macros
* \{
*/

/* Font map array symbol sizes in bytes. Used for the basic font definitions (\ref group_seglcd_globals) and also can be used for custom font definitions */
#define CY_SEGLCD_14SEG_FONTMAP_SIZE       (CY_SYSLIB_DIV_ROUNDUP(CY_SEGLCD_14SEG, CY_SEGLCD_OCTET)) /**< 14-segment fontMap array item size (in bytes) */
#define CY_SEGLCD_16SEG_FONTMAP_SIZE       (CY_SYSLIB_DIV_ROUNDUP(CY_SEGLCD_16SEG, CY_SEGLCD_OCTET)) /**< 16-segment fontMap array item size (in bytes) */
#define CY_SEGLCD_5X8DM_FONTMAP_SIZE       (CY_SYSLIB_DIV_ROUNDUP(CY_SEGLCD_5X8DM, CY_SEGLCD_OCTET)) /**< 5x8 dot matrix fontMap array item size (in bytes) */

/** \} group_seglcd_macros */


/***************************************
*       Configuration Structures
***************************************/


/**
* \addtogroup group_seglcd_data_structures
* \{
*/

/** Configuration structure */
typedef struct
{
    cy_en_seglcd_speed_t speed; /**< Speed mode selection, see: #cy_en_seglcd_speed_t. */
    cy_en_seglcd_wave_t   wave; /**< Waveform type configuration, see: #cy_en_seglcd_wave_t. */
    cy_en_seglcd_drive_t drive; /**< Driving mode configuration, see: #cy_en_seglcd_drive_t. */
    cy_en_seglcd_bias_t   bias; /**< PWM bias selection, see: #cy_en_seglcd_bias_t. */
    cy_en_seglcd_lsclk_t lsClk; /**< Low Speed Mode clock selection, see: #cy_en_seglcd_lsclk_t.
                                 *   This is effective for MXLCD_ver2,
                                 *   for MXLCD_ver1 it is ignored.
                                 */
    uint8_t             comNum; /**< The number of Common connections, the valid range is 2...16
                                 *   however the maximum is dependent on PSoC device family -
                                 *   there could be limitation to 4 or 8 commons, see the device TRM.
                                 */
    uint8_t             frRate; /**< The LCD frame rate, the valid range is 30...150 */
    uint8_t           contrast; /**< The LCD contrast, the valid range is 0...100 */
    uint32_t           clkFreq; /**< The LCD clock frequency (ignored for \ref CY_SEGLCD_SPEED_LOW mode),
                                 *   the valid range is 10000...100000000 (Hz)
                                 */
} cy_stc_seglcd_config_t;

/** Font structure */
typedef struct
{
    char_t            first; /**< The first character code in the fontMap array */
    char_t             last; /**< The last character code in the fontMap array */
    bool              ascii; /**< Specifies whether the font map is indexed accordingly to ASCII character codes:
                              *   true - the font is ASCII-coded alphanumeric (e.g. a basic set "space" (ASCII 0x20) - "tilde"(ASCII 0x7E));
                              *   false - the font is e.g. pure numeric (0x0...0xF only), or any other custom character set.
                              */
    uint8_t const * fontMap; /**< The pointer to the font segment/pixel map array. */
} cy_stc_seglcd_font_t;

/** Display structure */
typedef struct
{
    uint16_t                     type; /**< The display type, one of \ref cy_en_seglcd_disp_t or custom. */
    uint16_t                   symNum; /**< The number of symbols (digits, characters). */
    bool                       invert; /**< Specifies whether the display is inverted or not. */
    uint32_t           const * pixMap; /**< The pointer to the display pixel map array:
                                        *   The array dimension is: uint32_t pixMap[symNum][type].
                                        *   Each array item should be made using the \ref CY_SEGLCD_PIXEL macro.
                                        */
    cy_stc_seglcd_font_t const * font; /**< The pointer to the font structure suitable for this display.
                                        *   Note that for the \ref CY_SEGLCD_BAR displays font is not used, so can be NULL.
                                        */
} cy_stc_seglcd_disp_t;

/** \} group_seglcd_data_structures */


/** \addtogroup group_seglcd_globals
* \{
*/

extern const cy_stc_seglcd_font_t cy_segLCD_7SegFont;  /**< Basic 7-segment font */
extern const cy_stc_seglcd_font_t cy_segLCD_14SegFont; /**< Basic 14-segment font */
extern const cy_stc_seglcd_font_t cy_segLCD_16SegFont; /**< Basic 16-segment font */
extern const cy_stc_seglcd_font_t cy_segLCD_5x8DmFont; /**< Basic 5x8 dot matrix font */

/** \} group_seglcd_globals */


/***************************************
*        Function Prototypes
***************************************/


/**
* \addtogroup group_seglcd_functions
* \{
*/


/**
* \addtogroup group_seglcd_functions_config
* \{
*/
cy_en_seglcd_status_t Cy_SegLCD_Init    (LCD_Type * base,                    cy_stc_seglcd_config_t const * config);
cy_en_seglcd_status_t Cy_SegLCD_Contrast(LCD_Type * base, uint32_t contrast, cy_stc_seglcd_config_t       * config);
                 void Cy_SegLCD_Deinit  (LCD_Type * base);
                 void Cy_SegLCD_Enable  (LCD_Type * base);
                 void Cy_SegLCD_Disable (LCD_Type * base);
/** \} group_seglcd_functions_config */


/**
* \addtogroup group_seglcd_functions_display
* \{
*/
cy_en_seglcd_status_t Cy_SegLCD_WriteChar  (LCD_Type * base, char_t      character, uint32_t position, cy_stc_seglcd_disp_t const * display);
cy_en_seglcd_status_t Cy_SegLCD_WriteString(LCD_Type * base, char_t const * string, uint32_t position, cy_stc_seglcd_disp_t const * display);
cy_en_seglcd_status_t Cy_SegLCD_WriteNumber(LCD_Type * base, uint32_t        value, uint32_t position, cy_stc_seglcd_disp_t const * display, bool zeroes, bool hex);
cy_en_seglcd_status_t Cy_SegLCD_BarGraph   (LCD_Type * base, uint32_t        value, uint32_t position, cy_stc_seglcd_disp_t const * display);
/** \} group_seglcd_functions_display */


/**
* \addtogroup group_seglcd_functions_frame
* \{
*/
                cy_en_seglcd_status_t Cy_SegLCD_ClrFrame  (LCD_Type * base, uint32_t const * commons);
                cy_en_seglcd_status_t Cy_SegLCD_InvFrame  (LCD_Type * base, uint32_t const * commons);
                cy_en_seglcd_status_t Cy_SegLCD_WritePixel(LCD_Type * base, uint32_t pixel, bool value);
                                 bool Cy_SegLCD_ReadPixel (LCD_Type * base, uint32_t pixel);
__STATIC_INLINE cy_en_seglcd_status_t Cy_SegLCD_SetPixel  (LCD_Type * base, uint32_t pixel);
__STATIC_INLINE cy_en_seglcd_status_t Cy_SegLCD_ClrPixel  (LCD_Type * base, uint32_t pixel);
__STATIC_INLINE cy_en_seglcd_status_t Cy_SegLCD_InvPixel  (LCD_Type * base, uint32_t pixel);


/*******************************************************************************
* Function Name: Cy_SegLCD_SetPixel
****************************************************************************//**
*
* Sets (turns on) the specified pixel.
*
* \param base The base pointer to the LCD instance registers.
* \param pixel The predefined packed number that points to the pixel location
*              in the frame buffer.
* \return \ref cy_en_seglcd_status_t.
*
* \funcusage
* \snippet seglcd/snippet/SegLCD_Snpt.h snippet_Cy_SegLCD_DefPixel
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_SetPixel
*
*******************************************************************************/
__STATIC_INLINE cy_en_seglcd_status_t Cy_SegLCD_SetPixel(LCD_Type * base, uint32_t pixel)
{
    return (Cy_SegLCD_WritePixel(base, pixel, true));
}


/*******************************************************************************
* Function Name: Cy_SegLCD_ClrPixel
****************************************************************************//**
*
* Clears (turns off) the specified pixel.
*
* \param base The base pointer to the LCD instance registers.
* \param pixel The predefined packed number that points to the pixel location
*              in the frame buffer.
* \return \ref cy_en_seglcd_status_t.
*
* \funcusage
* \snippet seglcd/snippet/SegLCD_Snpt.h snippet_Cy_SegLCD_DefPixel
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_ClrPixel
*
*******************************************************************************/
__STATIC_INLINE cy_en_seglcd_status_t Cy_SegLCD_ClrPixel(LCD_Type * base, uint32_t pixel)
{
    return (Cy_SegLCD_WritePixel(base, pixel, false));
}


/*******************************************************************************
* Function Name: Cy_SegLCD_InvPixel
****************************************************************************//**
*
* Inverts the state of the specified pixel.
*
* \param base The base pointer to the LCD instance registers.
* \param pixel The predefined packed number that points to the pixel location
*              in the frame buffer.
* \return \ref cy_en_seglcd_status_t.
*
* \funcusage
* \snippet seglcd/snippet/SegLCD_Snpt.h snippet_Cy_SegLCD_DefPixel
* \snippet seglcd/snippet/SegLCD_Snpt.c snippet_Cy_SegLCD_InvPixel
*
*******************************************************************************/
__STATIC_INLINE cy_en_seglcd_status_t Cy_SegLCD_InvPixel(LCD_Type * base, uint32_t pixel)
{
    return (Cy_SegLCD_WritePixel(base, pixel, !Cy_SegLCD_ReadPixel(base, pixel)));
}

/** \} group_seglcd_functions_frame */

/** \} group_seglcd_functions */

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXLCD */

#endif /* CY_SEGLCD_H */

/** \} group_seglcd */

/* [] END OF FILE */
