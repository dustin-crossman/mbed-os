/***************************************************************************//**
* \file cy_seglcd.c
* \version 1.0
*
* \brief
* Provides an API implementation of the SegLCD driver
*
********************************************************************************
* \copyright
* Copyright 2018, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_seglcd.h"

#ifdef CY_IP_MXLCD

#if defined(__cplusplus)
extern "C" {
#endif


/** \cond internal */
/* Calculate a / b with rounding to the nearest integer. a and b must have the same sign. */
#define CY_SEGLCD_DIV_ROUND(a, b) (((a) + ((b) / 2u)) / (b))


/*******************************************************************************
* Function Name: GetDataRegPtr
****************************************************************************//**
*
* Returns a pointer to the data frame register for specified pixel.
* Used by \ref Cy_SegLCD_WritePixel and \ref Cy_SegLCD_ReadPixel functions.
*
* \param base  The base pointer to the LCD instance registers.
* \param pixel The predefined packed number that points to the pixel's location
*              in the frame buffer.
*
*******************************************************************************/
static volatile uint32_t * GetDataRegPtr(LCD_Type * base, uint32_t pixel);
static volatile uint32_t * GetDataRegPtr(LCD_Type * base, uint32_t pixel)
{
    volatile uint32_t * locDataPtr[] = {LCD_DATA0(base), LCD_DATA1(base), LCD_DATA2(base), LCD_DATA3(base)};

    return (&(locDataPtr[CY_SEGLCD_EXTR_COM(pixel) / CY_SEGLCD_NIBBLE][CY_SEGLCD_EXTR_OCT(pixel)]));
}

/** \endcond */


/*******************************************************************************
* Function Name: Cy_SegLCD_Init
****************************************************************************//**
*
* Initialize/Restore default Segment LCD configuration.
*
* \param base   The base pointer to the LCD instance registers.
* \param config The pointer to a configuration structure.
* \return The status \ref cy_en_seglcd_status_t
*         The CY_SEGLCD_BAD_PARAM is returned if either some parameter is NULL
*         or some config setting is incorrect (out of valid range).
*
* Side Effects:
*  Block will be disabled to change settings.
*
*******************************************************************************/
cy_en_seglcd_status_t Cy_SegLCD_Init(LCD_Type * base, cy_stc_seglcd_config_t const * config)
{
    cy_en_seglcd_status_t retVal = CY_SEGLCD_SUCCESS;
    
    if((NULL == base) || 
       (NULL == config) || 
       (!CY_SEGLCD_IS_COM_NUM_VALID(config->comNum)))
    {
        retVal = CY_SEGLCD_BAD_PARAM;
    }
    else
    {
        uint32_t locCheck = (CY_SEGLCD_SPEED_LOW == config->speed) ? (0x000000FFUL) : (0x0000FFFFUL);
        uint32_t locSubfr = CY_SEGLCD_DIV_ROUND((CY_SEGLCD_DIV_ROUND(config->clkFreq, config->frRate * 4UL * config->comNum) - 1UL) * config->contrast, 100UL);
        uint32_t locDead = CY_SEGLCD_DIV_ROUND(CY_SEGLCD_DIV_ROUND(config->clkFreq * (100UL - config->contrast), config->frRate), 100UL);
        
        if ((locSubfr > locCheck) || (locDead > locCheck))
        {
            retVal = CY_SEGLCD_BAD_PARAM;
        }
        else
        {
            Cy_SegLCD_Disable(base);
            
            /* Calculate sub-frame and dead time dividers */
            LCD_DIVIDER(base) = _VAL2FLD(LCD_DIVIDER_SUBFR_DIV, locSubfr) |
                                _VAL2FLD(LCD_DIVIDER_DEAD_DIV,  locDead);
                                  
            /* Set the rest of settings */
            LCD_CONTROL(base) = _VAL2FLD(LCD_CONTROL_LCD_MODE, config->speed) |
                                _VAL2FLD(LCD_CONTROL_TYPE,     config->wave)  |
                                _VAL2FLD(LCD_CONTROL_OP_MODE,  config->drive) |
                                _VAL2FLD(LCD_CONTROL_BIAS,     config->bias)  |
        /* Convert the comNum from the user's range (2-16) into the machine range (0-14). */
                                _VAL2FLD(LCD_CONTROL_COM_NUM,  config->comNum - 2UL);
        }
    }
    
    return (retVal);
}


/*******************************************************************************
* Function Name: Cy_SegLCD_Deinit
****************************************************************************//**
*
* Deinitializes the LCD block (resets the registers to default state).
*
* \param base   The base pointer to the LCD instance registers.
*
*******************************************************************************/
void Cy_SegLCD_Deinit(LCD_Type * base)
{
    uint32_t i;
    
    LCD_CONTROL(base) = 0UL;
    LCD_DIVIDER(base) = 0UL;
    
    for (i = 0UL; i < LCD_OCTET_NUM; i++)
    {
        LCD_DATA0(base)[i] = 0UL;
        LCD_DATA1(base)[i] = 0UL;
        LCD_DATA2(base)[i] = 0UL;
        LCD_DATA3(base)[i] = 0UL;
    }
}


/*******************************************************************************
* Function Name: Cy_SegLCD_Enable
****************************************************************************//**
*
* Enables the Segment LCD block.
*
* \param base The base pointer to the LCD instance registers.
*
*******************************************************************************/
void Cy_SegLCD_Enable(LCD_Type * base)
{
    if (_FLD2BOOL(LCD_CONTROL_LCD_MODE, LCD_CONTROL(base)))
    {
        LCD_CONTROL(base) = (LCD_CONTROL(base) & ((uint32_t)(~CY_SEGLCD_ENABLE_MASK))) | _BOOL2FLD(LCD_CONTROL_HS_EN, true);
    }
    else /* CY_SEGLCD_SPEED_HIGH */
    {
        LCD_CONTROL(base) = (LCD_CONTROL(base) & ((uint32_t)(~CY_SEGLCD_ENABLE_MASK))) | _BOOL2FLD(LCD_CONTROL_LS_EN, true);

        while (!_FLD2BOOL(LCD_CONTROL_LS_EN_STAT, LCD_CONTROL(base)))
        {
            /* Wait for enabled status */
        }
    }
}


/*******************************************************************************
* Function Name: Cy_SegLCD_Disable
****************************************************************************//**
*
* Disables the Segment LCD block.
*
* \param base The base pointer to the LCD instance registers.
*
*******************************************************************************/
void Cy_SegLCD_Disable(LCD_Type * base)
{
    if (_FLD2BOOL(LCD_CONTROL_LCD_MODE, LCD_CONTROL(base)))
    {
        LCD_CONTROL(base) &= (uint32_t) ~LCD_CONTROL_HS_EN_Msk;
    }
    else /* CY_SEGLCD_SPEED_HIGH */
    {
        if (_FLD2BOOL(LCD_CONTROL_LS_EN, LCD_CONTROL(base)))
        {
            while (!_FLD2BOOL(LCD_CONTROL_LS_EN_STAT, LCD_CONTROL(base)))
            {
                /* Wait for enabled status */
            }

            LCD_CONTROL(base) &= (uint32_t) ~LCD_CONTROL_LS_EN_Msk;

            while (_FLD2BOOL(LCD_CONTROL_LS_EN_STAT, LCD_CONTROL(base)))
            {
                /* Wait for disabled status */
            }
        }
    }
}


/*******************************************************************************
* Function Name: Cy_SegLCD_ClrDisplay
****************************************************************************//**
*
* Clears the display and initiates the common lines.
* Should be called after \ref Cy_SegLCD_Init before \ref Cy_SegLCD_Enable.
*
* \param base    The base pointer to the LCD instance registers.
* \param commons The pointer to array of common lines.
*                The array size is specified by \ref cy_stc_seglcd_config_t.comNum.
*                Each common line value shall be made using 
*                \ref CY_SEGLCD_COMMON macro.
*
* \return The status \ref cy_en_seglcd_status_t.
*         The CY_SEGLCD_BAD_PARAM is returned in case of wrong common line value.
*
*******************************************************************************/
cy_en_seglcd_status_t Cy_SegLCD_ClrDisplay(LCD_Type * base, uint32_t const * commons)
{
    cy_en_seglcd_status_t retVal = CY_SEGLCD_SUCCESS;
    uint32_t i;
    /* Convert the locComNum from the machine range (0-14) into the natural range (2-16). */
    uint32_t locComNum = _FLD2VAL(LCD_CONTROL_COM_NUM, LCD_CONTROL(base)) + 2UL;

    /* Clear entire frame buffer to all zeroes */
    for (i = 0UL; i < LCD_OCTET_NUM; i++)
    {
        LCD_DATA0(base)[i] = 0UL;
        LCD_DATA1(base)[i] = 0UL;
        LCD_DATA2(base)[i] = 0UL;
        LCD_DATA3(base)[i] = 0UL;
    }

    /* Reinitialize the commons */
    for (i = 0UL; (i < locComNum) && (CY_SEGLCD_SUCCESS == retVal); i++)
    {
        retVal |= Cy_SegLCD_WritePixel(base, commons[i], true);
    }

    return(retVal);
}


/*******************************************************************************
* Function Name: Cy_SegLCD_InvDisplay
****************************************************************************//**
*
* Inverts the display (all the display pixel states are inverted).
*
* \param base    The base pointer to the LCD instance registers.
* \param commons The pointer to array of common lines.
*                The array size is specified by \ref cy_stc_seglcd_config_t.comNum.
*                Each common line value shall be made using 
*                \ref CY_SEGLCD_COMMON macro.
*
* \return The status \ref cy_en_seglcd_status_t.
*         The CY_SEGLCD_BAD_PARAM is returned in case of wrong common line value.
*
*******************************************************************************/
cy_en_seglcd_status_t Cy_SegLCD_InvDisplay(LCD_Type * base, uint32_t const * commons)
{
    cy_en_seglcd_status_t retVal = CY_SEGLCD_SUCCESS;
    uint32_t i;
    /* Convert the locComNum from the machine range (0-14) into the natural range (2-16). */
    uint32_t locComNum = _FLD2VAL(LCD_CONTROL_COM_NUM, LCD_CONTROL(base)) + 2UL;

    /* Invert entire frame buffer */
    for (i = 0UL; i < LCD_OCTET_NUM; i++)
    {
        LCD_DATA0(base)[i] = (uint32_t) ~LCD_DATA0(base)[i];
        LCD_DATA0(base)[i] = (uint32_t) ~LCD_DATA0(base)[i];
        LCD_DATA0(base)[i] = (uint32_t) ~LCD_DATA0(base)[i];
        LCD_DATA0(base)[i] = (uint32_t) ~LCD_DATA0(base)[i];
    }
    
    /* Reinitialize the commons */
    for (i = 0UL; (i < locComNum) && (CY_SEGLCD_SUCCESS == retVal); i++)
    {
        uint32_t locCom = commons[i] & ((uint32_t) ~CY_SEGLCD_COM_Msk);
        uint32_t j = 0UL;
        /* Clear commons pin data after frame buffer inverting */
        for (j = 0UL; (j < locComNum) && (CY_SEGLCD_SUCCESS == retVal); j++)
        {
            retVal |= Cy_SegLCD_WritePixel(base, locCom | ((uint32_t)(j << CY_SEGLCD_COM_Pos)), false);
        }

        /* Set pin data for commons */
        retVal |= Cy_SegLCD_WritePixel(base, commons[i], true);
    }
    
    return(retVal);
}


/*******************************************************************************
* Function Name: Cy_SegLCD_WritePixel
****************************************************************************//**
*
*  Sets or clears a particular pixel in the frame buffer.
*
* \param base The base pointer to the LCD instance registers.
* \param pixel The predefined packed number that points to the pixel's location
*              in the frame buffer.
* \param value Specifies pixel on/off state.
* \return The status \ref cy_en_seglcd_status_t
*         The CY_SEGLCD_BAD_PARAM is returned in case of wrong pixel value or
*         if the pixel is specified as not connected (CY_SEGLCD_NOT_CON).
*
*******************************************************************************/
cy_en_seglcd_status_t Cy_SegLCD_WritePixel(LCD_Type * base, uint32_t pixel, bool value)
{
    cy_en_seglcd_status_t retVal = CY_SEGLCD_BAD_PARAM;
    
    if (CY_SEGLCD_IS_PIX_VALID(pixel))
    {
        /* Extract frame buffer register */
        volatile uint32_t * locPtr = GetDataRegPtr(base, pixel);

        /* Write new bit value to the frame buffer. */
        *locPtr = (*locPtr & ~CY_SEGLCD_PIX_Msk(pixel)) | (value ? CY_SEGLCD_PIX_Msk(pixel) : 0UL);

        retVal = CY_SEGLCD_SUCCESS;
    }

    return(retVal);
}


/*******************************************************************************
* Function Name: Cy_SegLCD_ReadPixel
****************************************************************************//**
*
* Gets state of a specified pixel in the frame buffer.
*
* \param base The base pointer to the LCD instance registers.
* \param pixel The predefined packed number that points to the pixel's location
*              in the frame buffer. Each pixel value shall be made using
*              \ref CY_SEGLCD_PIXEL macro.
* \return The current status of the specified pixel in the frame buffer.
*
*******************************************************************************/
bool Cy_SegLCD_ReadPixel(LCD_Type * base, uint32_t pixel)
{
    bool retVal = false;
    
    if (CY_SEGLCD_IS_PIX_VALID(pixel))
    {
        /* Extract frame buffer register */
        volatile uint32_t * locPtr = GetDataRegPtr(base, pixel);
    
        retVal = (0UL != (*locPtr & CY_SEGLCD_PIX_Msk(pixel)));
    }
    else
    {
        CY_ASSERT_L3(false);
    }

    return (retVal);
}


/*******************************************************************************
* Function Name: Cy_SegLCD_WriteChar
****************************************************************************//**
*
* Internal function, displays an 8-bit character on a 7-, 14- or 16-segment
* display character.
*
* \param base The base pointer to the LCD instance registers.
* \param character The ASCII code of the character to display.
* \param position The position of the character/digit on display. 
*                 Zero is the most left character/digit of the specified
*                 display \ref cy_stc_seglcd_disp_t.
* \param display The structure which defines a display \ref cy_stc_seglcd_disp_t.
* \return The status \ref cy_en_seglcd_status_t
*         The CY_SEGLCD_BAD_PARAM is returned in case of wrong pixel value in
*         the character array or if any pixel is specified as not connected.
*
*******************************************************************************/
cy_en_seglcd_status_t Cy_SegLCD_WriteChar(LCD_Type * base,
                                            char_t   character,
                                          uint32_t   position,
                        cy_stc_seglcd_disp_t const * display)
{
    cy_en_seglcd_status_t retVal = CY_SEGLCD_SUCCESS;
    
    switch (display->type)
    {
        case CY_SEGLCD_7SEG:
            if (character <= (char_t)0x10)
            {
                /* The character is correct for 7-segment */
            }
            else if (('0' <= character) && (character <= '9'))
            {
                character -= '0'; /* ASCII code for dec numbers 0...9 */
            }
            else if (('A' <= character) && (character <= 'F'))
            {
                character -= 'A' - (char_t)0xA; /* ASCII code for hex numbers A...F */
            }
            else
            {
                character = (char_t)0x10; /* The index of the blank character for 7-segment for all the rest ASCII characters */
            }
            break;

        case CY_SEGLCD_14SEG:
        case CY_SEGLCD_16SEG:
            if (character > '~') /* if out of ASCII range */
            {
                character = ' '; /* blank (space) character */
            }

            if (character < (char_t)10) /* for dec numbers 0...9 */
            {
                character += ('0' - ' ');
            }
            else if (character < (char_t)0x10) /* for hex numbers A...F */
            {
                character += ('A' - ' ' - (char_t)0xA);
            }
            else
            {
                character -= ' '; /* Shift the char starting index to the 'space' */
            }
            break;

        case CY_SEGLCD_5x8DM:
            if (character < (char_t)10) /* for dec numbers 0...9 */
            {
                character += ('0' - 16);
            }
            else if (character < (char_t)0x10) /* for hex numbers A...F */
            {
                character += ('A' - 16 - (char_t)0xA);
            }
            else
            {
                character -= 16; /* Shift the char starting index */
            }
            break;

        default:
            retVal = CY_SEGLCD_BAD_PARAM;
            break;
    }

    if (CY_SEGLCD_SUCCESS == retVal)
    {
        if (CY_SEGLCD_5x8DM == display->type)
        {
            uint32_t i;
            uint32_t j;

            for (i = 0UL; (i < CY_SEGLCD_5x8DM_WIDTH) && (CY_SEGLCD_SUCCESS == retVal); i++)
            {
                for (j = 0UL; (j < CY_SEGLCD_5x8DM_HEIGHT) && (CY_SEGLCD_SUCCESS == retVal); j++)
                {
                    retVal |= Cy_SegLCD_WritePixel(base, *(display->disPtr + i + j * CY_SEGLCD_5x8DM_WIDTH + position * (uint32_t)display->type), 0U != ((((uint8_t*)display->font)[i + (uint8_t)character * CY_SEGLCD_5x8DM_WIDTH] >> j) & 1U));
                }
            }
        }
        else /* 7seg, 14seg or 16seg */
        {
            uint32_t i;

            for (i = 0UL; (i < (uint32_t)display->type) && ((CY_SEGLCD_SUCCESS == retVal)); i++)
            {
                retVal |= Cy_SegLCD_WritePixel(base, *(display->disPtr + (position * (uint32_t)display->type) + i), 0U != ((((uint16_t*)display->font)[(uint8_t)character] >> i) & 1U));
            }
        }
    }
    
    return(retVal);
}


/*******************************************************************************
* Function Name: Cy_SegLCD_WriteNumber
****************************************************************************//**
*
* Displays an integer value on an array of 7-segment display digits.
* Sign conversion, sign display, decimal points, and other custom features shall
* be handled by application-specific user code.
*
* \param base The base pointer to the LCD instance registers.
* \param value Unsigned integer value to be displayed.
* \param position The position of the least significant digit as counted right
*            to left starting at the least significant digit of the specified
*            display \ref cy_stc_seglcd_disp_t. If the display contains fewer
*            digits then the value requires to be displayed then the most
*            significant digit/s are not displayed.
* \param display The structure which defines a display \ref cy_stc_seglcd_disp_t.
* \param leadingZeroes If true - all the unused digits at the left of displayed
*                                value are zeroes.
*                      If true - all the mentioned above digits are blank.
* \param hex If true - the value is displayed in hexadecimal format.
* \return The status \ref cy_en_seglcd_status_t
*         The CY_SEGLCD_BAD_PARAM is returned in case of wrong pixel value in
*         the display structure or if any pixel is specified as not connected.
*
*******************************************************************************/
cy_en_seglcd_status_t Cy_SegLCD_WriteNumber(LCD_Type * base,
                                            uint32_t   value,
                                            uint32_t   position,
                          cy_stc_seglcd_disp_t const * display,
                                                bool   leadingZeroes,
                                                bool   hex)
{
    cy_en_seglcd_status_t retVal = CY_SEGLCD_BAD_PARAM;

    if ((NULL != display) &&
        (position < display->symNum))
    {
        char_t locChar;
        uint32_t i;
        uint32_t locDiv = (hex) ? 0x10UL : 10UL; /* Digit weight for hex/dec counting systems */
        retVal = CY_SEGLCD_SUCCESS;

        /* While current digit position in the range of display keep processing the output */
        for (i = position; (i < display->symNum) && (CY_SEGLCD_SUCCESS == retVal); i--)
        {
            if ((0UL == value) && (i != position) && (!leadingZeroes))
            {
                /* In case of 'no leading zeros' each extra digit will be cleared */
                locChar = ' ';
            }
            else
            {
                /* Display subsequent digit or '0' if 'no leading zeros' mode */
                locChar = (char_t)(value % locDiv);
            }

            retVal |= Cy_SegLCD_WriteChar(base, locChar, i, display);

            value = value / locDiv;
        }
    }

    return(retVal);
}


/*******************************************************************************
* Function Name: Cy_SegLCD_WriteString
****************************************************************************//**
*
* Internal function, writes character strings on 14- and 16-segment displays.
*
* \param base The base pointer to the LCD instance registers.
* \param string Pointer to the null terminated character string.
* \param position The Position of the first character as counted left to right
*             starting at 0 on the left. If the defined display contains fewer
*             characters then the string requires for display, the extra
*             characters will not be displayed.
* \param display Pointer to the display structure \ref cy_stc_seglcd_disp_t.
* \return The status \ref cy_en_seglcd_status_t
*         The CY_SEGLCD_BAD_PARAM is returned in case of wrong pixel value in
*         the character array or if any pixel is specified as not connected.
*
*******************************************************************************/
cy_en_seglcd_status_t Cy_SegLCD_WriteString(LCD_Type * base,
                                        char_t const * string,
                                            uint32_t   position,
                          cy_stc_seglcd_disp_t const * display)
{
    cy_en_seglcd_status_t retVal = CY_SEGLCD_SUCCESS;
    uint32_t i = 0UL;
    
    for (; ('\0' != string[i]) && (position < display->symNum) && (CY_SEGLCD_SUCCESS == retVal); position++)
    {
        retVal |= Cy_SegLCD_WriteChar(base, string[i], position, display);
        i++;
    }

    return(retVal);
}


#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXLCD */

/* [] END OF FILE */
