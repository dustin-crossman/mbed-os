/***************************************************************************//**
* \file cy_seglcd.h
* \version 1.0
*
* \brief
* Provides an API declaration of the Segment LCD driver
*
********************************************************************************
* \copyright
* Copyright 2018, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/**
* \addtogroup group_seglcd
* \{
* The SegLCD driver provides an API to configure and access the SegLCD hardware.
*
* \section group_seglcd_more_information More Information
*
* Refer to the technical reference manual (TRM) and the device datasheet.
*
* \section group_seglcd_MISRA MISRA-C Compliance]
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>Rule number</td>
*     <td>R</td>
*     <td>Insert description</td>
*     <td>Insert reason for deviation</td>
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
* \defgroup group_seglcd_macros Macros
* \defgroup group_seglcd_functions Functions
* \{
*   \defgroup group_seglcd_functions_config     Block Configuration Functions
*   \defgroup group_seglcd_functions_display    Frame Buffer Operation Functions
* \}
* \defgroup group_seglcd_data_structures Data Structures
* \defgroup group_seglcd_enums Enumerated Types
*/

#if !defined(CY_SEGLCD_H)
#define CY_SEGLCD_H


#include "cy_device.h"
#include "cy_device_headers.h"
#include "cy_syslib.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef CY_IP_MXLCD

#if defined(__cplusplus)
extern "C" {
#endif

/** \addtogroup group_seglcd_macros
* \{
*/

/** Driver major version */
#define CY_SEGLCD_DRV_VERSION_MAJOR     1

/** Driver minor version */
#define CY_SEGLCD_DRV_VERSION_MINOR     0

/** SegLCD driver ID */
#define CY_SEGLCD_ID                    (CY_PDL_DRV_ID(0x40u))

/** Specifies the pixel in the display array which is not connected */
#define CY_SEGLCD_NOT_CON               (0xFFFFUL)

/** \cond internal */
#define CY_SEGLCD_OCTET                 (8U)
#define CY_SEGLCD_NIBBLE                (4U)

#define CY_SEGLCD_COM_Pos               (16U)
#define CY_SEGLCD_COM_Msk               ((uint32_t) 0xFUL << CY_SEGLCD_COM_Pos)
#define CY_SEGLCD_OCT_Pos               (8U)
#define CY_SEGLCD_OCT_Msk               ((uint32_t) 0x7UL << CY_SEGLCD_OCT_Pos)
#define CY_SEGLCD_NBL_Pos               (0U)
#define CY_SEGLCD_NBL_Msk               ((uint32_t) 0x7UL << CY_SEGLCD_NBL_Pos)

#define CY_SEGLCD_ITEM(trmNum, comLine)    (_VAL2FLD(CY_SEGLCD_COM, (comLine))                  | \
                                            _VAL2FLD(CY_SEGLCD_OCT, (trmNum) / CY_SEGLCD_OCTET) | \
                                            _VAL2FLD(CY_SEGLCD_NBL, (trmNum) % CY_SEGLCD_OCTET))
/** \endcond */

/**
* Makes the pixel value from the absolute LCD segment terminal number and the user defined common line number.
* It shall be used to display array definitions for \ref cy_stc_seglcd_disp_t.
*/
#define CY_SEGLCD_PIXEL(segNum, comLine)   (CY_SEGLCD_ITEM(segNum, comLine))

/**
* Makes the common line value from the absolute LCD common terminal number and the user defined common line number.
* It shall be used to common lines array definitions for \ref Cy_SegLCD_ClrDisplay and \ref Cy_SegLCD_InvDisplay.
*/
#define CY_SEGLCD_COMMON(comNum, comLine)  (CY_SEGLCD_ITEM(comNum, comLine))

/** \} group_seglcd_macros */

/** \cond internal */
#define CY_SEGLCD_ENABLE_MASK              (LCD_CONTROL_LS_EN_Msk | LCD_CONTROL_HS_EN_Msk)

#define CY_SEGLCD_5x8DM_WIDTH              (5UL)
#define CY_SEGLCD_5x8DM_HEIGHT             (8UL)

/** Internal macros that extract pixel information from the pixel value */
/* Extract the LCD common line number from the pixel */
#define CY_SEGLCD_EXTR_COM(pixel)          (_FLD2VAL(CY_SEGLCD_COM, (pixel)))
/* Extract the LCD terminal octet number from the pixel */
#define CY_SEGLCD_EXTR_OCT(pixel)          (_FLD2VAL(CY_SEGLCD_OCT, (pixel)))
/* Extract the LCD terminal nibble number from the pixel */
#define CY_SEGLCD_EXTR_NBL(pixel)          (_FLD2VAL(CY_SEGLCD_NBL, (pixel)))
/* Extract the LCD terminal number from the pixel */
#define CY_SEGLCD_EXTR_TRM(pixel)          ((CY_SEGLCD_EXTR_OCT(pixel) * CY_SEGLCD_OCTET) + \
                                             CY_SEGLCD_EXTR_NBL(pixel))

#define CY_SEGLCD_PIX_Pos(pixel)           ((CY_SEGLCD_EXTR_NBL(pixel) * CY_SEGLCD_NIBBLE) + \
                                            (CY_SEGLCD_EXTR_COM(pixel) % CY_SEGLCD_NIBBLE))
#define CY_SEGLCD_PIX_Msk(pixel)           ((uint32_t)(1UL << CY_SEGLCD_PIX_Pos(pixel)))

#define CY_SEGLCD_COM_NUM_MIN              (2U)
#define CY_SEGLCD_COM_NUM_MAX              (LCD_COM_NUM)

#define CY_SEGLCD_IS_MSK_VALID(pixel)      (0UL == ((pixel) & ((uint32_t) ~(CY_SEGLCD_COM_Msk | CY_SEGLCD_OCT_Msk | CY_SEGLCD_NBL_Msk))))
#define CY_SEGLCD_IS_OCT_VALID(pixel)      (LCD_OCTET_NUM > CY_SEGLCD_EXTR_OCT(pixel))
#define CY_SEGLCD_IS_COM_VALID(pixel)      (LCD_COM_NUM > CY_SEGLCD_EXTR_COM(pixel))
#define CY_SEGLCD_IS_PIX_VALID(pixel)      (CY_SEGLCD_IS_MSK_VALID(pixel) && CY_SEGLCD_IS_OCT_VALID(pixel) && CY_SEGLCD_IS_COM_VALID(pixel))

#define CY_SEGLCD_IS_COM_NUM_VALID(num)    ((CY_SEGLCD_COM_NUM_MIN <= (num)) && ((num) <= CY_SEGLCD_COM_NUM_MAX))

/** \endcond */


/**
* \addtogroup group_seglcd_enums
* \{
*/

/** SegLCD driver error codes */
typedef enum 
{
    CY_SEGLCD_SUCCESS   = 0x0UL,                                     /**< Returned successful */
    CY_SEGLCD_BAD_PARAM = CY_SEGLCD_ID | CY_PDL_STATUS_ERROR | 0x1UL /**< Bad parameter was passed */
} cy_en_seglcd_status_t;

/** SegLCD driver speed mode */
typedef enum 
{
    CY_SEGLCD_SPEED_LOW  = 0UL, /**<Low Speed Mode, works in Active, Sleep and DeepSleep power modes */
    CY_SEGLCD_SPEED_HIGH = 1UL  /**<High Speed Mode, works in Active and Sleep power modes only) */
} cy_en_seglcd_speed_t;

/** SegLCD driving waveform type configuration */
typedef enum 
{
    CY_SEGLCD_TYPE_A = 0x0UL, /**< Type A - Each frame addresses each COM pin only once with a balanced (DC=0) waveform. */
    CY_SEGLCD_TYPE_B = 0x1UL  /**< Type B - Each frame addresses each COM pin twice in sequence with a positive
                                            and negative waveform that together are balanced (DC=0). */
} cy_en_seglcd_wave_t;

/** SegLCD driving mode configuration */
typedef enum 
{
    CY_SEGLCD_PWM         = 0x00U, /**< PWM Mode. */
    CY_SEGLCD_CORRELATION = 0x01U  /**< Digital Correlation Mode. */
} cy_en_seglcd_drive_t;

/** SegLCD PWM bias selection */
typedef enum 
{
    CY_SEGLCD_BIAS_HALF   = 0x00U, /**< 1/2 Bias. */
    CY_SEGLCD_BIAS_THIRD  = 0x01U, /**< 1/3 Bias. */
    CY_SEGLCD_BIAS_FOURTH = 0x02U, /**< 1/4 Bias (not supported by LS generator). */
    CY_SEGLCD_BIAS_FIFTH  = 0x03U, /**< 1/5 Bias (not supported by LS generator). */
} cy_en_seglcd_bias_t;

/** SegLCD segment display types */
typedef enum
{
    CY_SEGLCD_7SEG   = (7UL),  /**< Seven-segment display */
    CY_SEGLCD_14SEG  = (14UL), /**< Fourteen-segment display */
    CY_SEGLCD_16SEG  = (16UL), /**< Fourteen-segment display */
    CY_SEGLCD_5x8DM  = (40UL), /**< Five-by-eight dot matrix display */
} cy_en_seglcd_disp_t;

/** \} group_seglcd_enums */


/***************************************
*       Configuration Structures
***************************************/


/**
* \addtogroup group_seglcd_data_structures
* \{
*/

/** Configuration structure */
typedef struct {
    cy_en_seglcd_speed_t speed;    /**< Speed mode selection, see: #cy_en_seglcd_speed_t. */
    cy_en_seglcd_wave_t  wave;     /**< Waveform type configuration, see: #cy_en_seglcd_wave_t. */
    cy_en_seglcd_drive_t drive;    /**< Driving mode configuration, see: #cy_en_seglcd_drive_t. */
    cy_en_seglcd_bias_t  bias;     /**< PWM bias selection, see: #cy_en_seglcd_bias_t. */
    uint32_t             comNum;   /**< The number of Common connections, the valid range is 2...16 */
    uint32_t             frRate;   /**< The LCD frame rate, the valid range is 30...150 */
    uint32_t             contrast; /**< The LCD contrast, the valid range is 0...100 */
    uint32_t             clkFreq;  /**< The LCD clock frequency (ignored in case of speedMode == CY_SEGLCD_SPEED_LOW), the valid range is 10000...100000000 (Hz) */
}cy_stc_seglcd_config_t;

/** Display structure */
typedef struct
{
    cy_en_seglcd_disp_t type; /**< Display type (number of pixels/segments per one display's symbol/character):
                               *   #CY_SEGLCD_7SEG for 7-segment displays, or
                               *   #CY_SEGLCD_14SEG for 14-segment displays, or
                               *   #CY_SEGLCD_16SEG for 16-segment displays, or
                               *   #CY_SEGLCD_5x8DM for 8x5 dot matrix displays
                               */
    uint32_t         symNum;  /**< Number of symbols (digits, characters) */
    uint32_t const * disPtr;  /**< Pointer to the display pixel array:
                               * For 7-segment display the array dimension is: uint32_t disp[symNum][CY_SEGLCD_7SEG]
                               * For 14-segment display the array dimension is: uint32_t disp[symNum][CY_SEGLCD_14SEG]
                               * For 16-segment display the array dimension is: uint32_t disp[symNum][CY_SEGLCD_16SEG]
                               * For dot matrix display the array dimension is: uint32_t disp[symNum][CY_SEGLCD_5x8DM_HEIGHT][CY_SEGLCD_5x8DM_WIDTH]
                               *
                               * Each array item shall be made using \ref CY_SEGLCD_PIXEL macro.
                               */
    void     const * font;    /**< Pointer to the font suitable to this display:
                               * For 7-segment display the font array is uint16_t and contains the symbol segment codes for
                               *   numbers 0...9 and letters A...F and also the 'space' (blank) symbol at the index 16.
                               * For 14-segment and 16-segment displays the font array is uint16_t and contains the symbol segment codes for
                               *   a basic ASCII symbol table starting at the 'space' (0x20) symbol and finishing at the '~' (0x7E) symbol.
                               * For 5x8 dot matrix display the font array is uint8_t and contains the symbol segment codes for
                               *   a extended ASCII symbol table starting at the 0x10 symbol and finishing at the 0xFF symbol.
                               */
}cy_stc_seglcd_disp_t;

/** \} group_seglcd_data_structures */


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
cy_en_seglcd_status_t Cy_SegLCD_Init       (LCD_Type * base, cy_stc_seglcd_config_t const * config);
                 void Cy_SegLCD_Deinit     (LCD_Type * base);
                 void Cy_SegLCD_Enable     (LCD_Type * base);
                 void Cy_SegLCD_Disable    (LCD_Type * base);

/** \} group_seglcd_functions_config */


/**
* \addtogroup group_seglcd_functions_display
* \{
*/
                cy_en_seglcd_status_t Cy_SegLCD_ClrDisplay(LCD_Type * base, uint32_t const * coms);
                cy_en_seglcd_status_t Cy_SegLCD_InvDisplay(LCD_Type * base, uint32_t const * coms);
                cy_en_seglcd_status_t Cy_SegLCD_WritePixel(LCD_Type * base, uint32_t pixel, bool value);
                                 bool Cy_SegLCD_ReadPixel (LCD_Type * base, uint32_t pixel);
__STATIC_INLINE cy_en_seglcd_status_t Cy_SegLCD_SetPixel  (LCD_Type * base, uint32_t pixel);
__STATIC_INLINE cy_en_seglcd_status_t Cy_SegLCD_ClrPixel  (LCD_Type * base, uint32_t pixel);
__STATIC_INLINE cy_en_seglcd_status_t Cy_SegLCD_InvPixel  (LCD_Type * base, uint32_t pixel);

cy_en_seglcd_status_t Cy_SegLCD_WriteChar    (LCD_Type * base, char_t      character, uint32_t position, cy_stc_seglcd_disp_t const * display);
cy_en_seglcd_status_t Cy_SegLCD_WriteString  (LCD_Type * base, char_t const * string, uint32_t position, cy_stc_seglcd_disp_t const * display);
cy_en_seglcd_status_t Cy_SegLCD_WriteNumber  (LCD_Type * base, uint32_t        value, uint32_t position, cy_stc_seglcd_disp_t const * display, bool leadingZeroes, bool hex);


/*******************************************************************************
* Function Name: Cy_SegLCD_SetPixel
****************************************************************************//**
*
* Sets (turns on) the specified pixel.
*
* \param base The base pointer to the LCD instance registers.
* \param pixel The predefined packed number that points to the pixel's location
*              in the frame buffer.
* \return The status \ref cy_en_seglcd_status_t
*         The CY_SEGLCD_BAD_PARAM is returned in case of wrong pixel value or
*         if the pixel is specified as not connected (CY_SEGLCD_NOT_CON).
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
* \param pixel The predefined packed number that points to the pixel's location
*              in the frame buffer.
* \return The status \ref cy_en_seglcd_status_t
*         The CY_SEGLCD_BAD_PARAM is returned in case of wrong pixel value or
*         if the pixel is specified as not connected (CY_SEGLCD_NOT_CON).
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
* \param pixel The predefined packed number that points to the pixel's location
*              in the frame buffer.
* \return The status \ref cy_en_seglcd_status_t
*         The CY_SEGLCD_BAD_PARAM is returned in case of wrong pixel value or
*         if the pixel is specified as not connected (CY_SEGLCD_NOT_CON).
*
*******************************************************************************/
__STATIC_INLINE cy_en_seglcd_status_t Cy_SegLCD_InvPixel(LCD_Type * base, uint32_t pixel)
{
    return (Cy_SegLCD_WritePixel(base, pixel, !Cy_SegLCD_ReadPixel(base, pixel)));
}

/** \} group_seglcd_functions_display */

/** \} group_seglcd_functions */

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXLCD */

#endif /* CY_SEGLCD_H */

/** \} group_seglcd */

/* [] END OF FILE */
