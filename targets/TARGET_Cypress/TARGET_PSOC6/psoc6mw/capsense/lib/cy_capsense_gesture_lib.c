/***************************************************************************//**
* \file cy_gesture.c
* \version 1.0
*
* \brief
*  Gesture implementation file.
*
********************************************************************************
* \copyright
* Copyright 2018, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_capsense_gesture_lib.h"


/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* 
* Rotate constants:
* U - Up        (0b00)
* D - Down      (0b01)
* D - Right     (0b10)
* L - Left      (0b11)
*/
/* Clockwise sequence */
#define CY_CAPSENSE_GESTURE_ROTATE_ORDER_U_R_D_L                    (0x27u)
#define CY_CAPSENSE_GESTURE_ROTATE_ORDER_L_U_R_D                    (0xC9u)
#define CY_CAPSENSE_GESTURE_ROTATE_ORDER_D_L_U_R                    (0x72u)
#define CY_CAPSENSE_GESTURE_ROTATE_ORDER_R_D_L_U                    (0x9Cu)

/* Counter clockwise sequence */
#define CY_CAPSENSE_GESTURE_ROTATE_ORDER_U_L_D_R                    (0x36u)
#define CY_CAPSENSE_GESTURE_ROTATE_ORDER_R_U_L_D                    (0x8Du)
#define CY_CAPSENSE_GESTURE_ROTATE_ORDER_D_R_U_L                    (0x63u)
#define CY_CAPSENSE_GESTURE_ROTATE_ORDER_L_D_R_U                    (0xD8u)

#define CY_CAPSENSE_GESTURE_ROTATE_LAST_DIRECTION_MASK              (0x03u)

/* Flick constants */
#define CY_CAPSENSE_TAN_22_5                                        (53u)
#define CY_CAPSENSE_TAN_67_5                                        (309u)
#define CY_CAPSENSE_TAN_BASE_SHIFT                                  (7u)

/* Constants for arctan calculation */
#define CY_CAPSENSE_GESTURE_ARCTAN_COEFF_A                          (54595)
#define CY_CAPSENSE_GESTURE_ARCTAN_COEFF_B                          (45066)
#define CY_CAPSENSE_GESTURE_ARCTAN_COEFF_C                          (47)

#define CY_CAPSENSE_GESTURE_DIRECTION_UNKNOWN                       (0xFFu)

#define ABS_DIFF(a,b)                                               (((a)>(b))?((a)-(b)):((b)-(a)))

#define CY_CAPSENSE_GESTURE_MULTIPLE_TOUCHES                        (0x04U)

/*******************************************************************************
* Gesture State Machine
*******************************************************************************/
#define CY_CAPSENSE_GESTURE_STATE_IDLE                              (0x00U)
#define CY_CAPSENSE_GESTURE_FIRST_TOUCH                             (0x01U)
#define CY_CAPSENSE_GESTURE_FIRST_LIFTOFF                           (0x02U)
#define CY_CAPSENSE_GESTURE_SECOND_TOUCH                            (0x03U)
#define CY_CAPSENSE_GESTURE_DETECTED                                (0x04U)
#define CY_CAPSENSE_GESTURE_INVALID                                 (0xFFU)


/*******************************************************************************
* Static Function Prototypes
*******************************************************************************/
static void Cy_CapSense_Gesture_Filtering(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context);
static void Cy_CapSense_Gesture_DecodeOneFgrSingleClick(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context);
static void Cy_CapSense_Gesture_DecodeOneFgrDoubleClick(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context);
static void Cy_CapSense_Gesture_DecodeOneFgrClickDrag(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context);
static void Cy_CapSense_Gesture_DecodeTwoFgrSingleClick(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context);
static void Cy_CapSense_Gesture_DecodeOneFgrScroll(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context);
static void Cy_CapSense_Gesture_DecodeTwoFgrScroll(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context);
static void Cy_CapSense_Gesture_DecodeOneFgrFlick(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context);
static void Cy_CapSense_Gesture_DecodeOneFgrEdgeSwipe(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context);
static void Cy_CapSense_Gesture_DecodeTwoFgrZoom(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context);
static void Cy_CapSense_Gesture_DecodeOneFgrRotate(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context);

static uint32_t Cy_CapSense_Gesture_DetermineQuadrant(
                uint16_t currX, uint16_t currY, 
                uint16_t prevX, uint16_t prevY);
static uint32_t Cy_CapSense_Gesture_DetermineEdge(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context);
static uint32_t Cy_CapSense_Gesture_CheckAngleWithinThresh(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context);
static uint32_t Cy_CapSense_Gesture_ArcTanOctant(uint32_t x, uint32_t y);
static int32_t Cy_CapSense_Gesture_GetAngle(uint32_t deltaX, uint32_t deltaY);
static uint32_t Cy_CapSense_Gesture_CheckDisplacement(
                uint32_t threshold,
                cy_stc_capsense_gesture_position_t * pos1,
                cy_stc_capsense_gesture_position_t * pos2);


/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_ResetState
****************************************************************************//**
*
* Initializes internal variables and states.
*
* \param config
* The pointer to the gesture context structure 
* \ref cy_stc_capsense_gesture_context_t.
*
*******************************************************************************/
void Cy_CapSense_Gesture_ResetState(
                cy_stc_capsense_gesture_context_t * context)
{
    context->numPosition = 0U;
    context->numPositionLast = 0U;
    context->detected = 0u;
    context->direction = 0u;
    /* 
    * Init could happens when touch is on the panel. So, reseting gesture 
    * state machines to invalid state prevents false gesture detection 
    */
    context->ofscContext.state = CY_CAPSENSE_GESTURE_INVALID;
    context->ofdcContext.state = CY_CAPSENSE_GESTURE_INVALID;
    context->ofcdContext.state = CY_CAPSENSE_GESTURE_INVALID;
    context->tfscContext.state = CY_CAPSENSE_GESTURE_INVALID;
    context->ofslContext.state = CY_CAPSENSE_GESTURE_INVALID;
    context->tfslContext.state = CY_CAPSENSE_GESTURE_INVALID;
    context->offlContext.state = CY_CAPSENSE_GESTURE_INVALID;
    context->ofesContext.state = CY_CAPSENSE_GESTURE_INVALID;
    context->tfzmContext.state = CY_CAPSENSE_GESTURE_INVALID;
    context->ofrtContext.state = CY_CAPSENSE_GESTURE_INVALID;
}


/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_Decode
****************************************************************************//**
*
* Decodes all enabled gestures. This function is called each scanning cycle.
*
* \param timestamp
* Current timestamp.
*
* \param touchNumber
* The number of touches. Also this parameter defines the size of position array.
*
* \param position
* The pointer to the array of positions \ref cy_stc_capsense_gesture_position_t.
*
* \param config
* The pointer to the gesture configuration structure 
* \ref cy_stc_capsense_gesture_config_t.
*
* \param context
* The pointer to the gesture context structure 
* \ref cy_stc_capsense_gesture_context_t.
*
*******************************************************************************/
void Cy_CapSense_Gesture_Decode(
                uint32_t timestamp, 
                uint32_t touchNumber, 
                const cy_stc_capsense_gesture_position_t * position, 
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context)
{
    uint32_t gestureMask = config->gestureEnableMask;

    /* Reset report values */
    context->direction = 0u;
    context->detected = 0u;
    
    if ((CY_CAPSENSE_GESTURE_MULTIPLE_TOUCHES <= context->numPosition) ||
        (CY_CAPSENSE_GESTURE_MULTIPLE_TOUCHES <= context->numPositionLast))
    {
        /* Reset gesture state machines if multiple touches are detected */
        Cy_CapSense_Gesture_ResetState(context);
    }
    
    /* Preprocessing */
    context->timestamp = timestamp;
    context->numPosition = touchNumber;
    context->position1 = * position;
    context->position2 = *(position + 1u);
    
    /* Gesture decoding */
    if (0u != (gestureMask & CY_CAPSENSE_GESTURE_ONE_FNGR_SINGLE_CLICK_MASK)) 
    { 
        Cy_CapSense_Gesture_DecodeOneFgrSingleClick(config, context); 
    }
    if (0u != (gestureMask & CY_CAPSENSE_GESTURE_ONE_FNGR_DOUBLE_CLICK_MASK)) 
    { 
        Cy_CapSense_Gesture_DecodeOneFgrDoubleClick(config, context); 
    }
    if (0u != (gestureMask & CY_CAPSENSE_GESTURE_ONE_FNGR_CLICK_DRAG_MASK)) 
    { 
        Cy_CapSense_Gesture_DecodeOneFgrClickDrag(config, context); 
    }
    if (0u != (gestureMask & CY_CAPSENSE_GESTURE_TWO_FNGR_SINGLE_CLICK_MASK)) 
    { 
        Cy_CapSense_Gesture_DecodeTwoFgrSingleClick(config, context); 
    }
    if (0u != (gestureMask & CY_CAPSENSE_GESTURE_ONE_FNGR_SCROLL_MASK)) 
    { 
        Cy_CapSense_Gesture_DecodeOneFgrScroll(config, context); 
    }
    if (0u != (gestureMask & CY_CAPSENSE_GESTURE_TWO_FNGR_SCROLL_MASK)) 
    { 
        Cy_CapSense_Gesture_DecodeTwoFgrScroll(config, context); 
    }
    if (0u != (gestureMask & CY_CAPSENSE_GESTURE_ONE_FNGR_FLICK_MASK)) 
    { 
        Cy_CapSense_Gesture_DecodeOneFgrFlick(config, context); 
    }
    if (0u != (gestureMask & CY_CAPSENSE_GESTURE_ONE_FNGR_EDGE_SWIPE_MASK)) 
    { 
        Cy_CapSense_Gesture_DecodeOneFgrEdgeSwipe(config, context); 
    }
    if (0u != (gestureMask & CY_CAPSENSE_GESTURE_TWO_FNGR_ZOOM_MASK)) 
    { 
        Cy_CapSense_Gesture_DecodeTwoFgrZoom(config, context); 
    }
    if (0u != (gestureMask & CY_CAPSENSE_GESTURE_ONE_FNGR_ROTATE_MASK)) 
    { 
        Cy_CapSense_Gesture_DecodeOneFgrRotate(config, context); 
    }
    if (context->numPositionLast < context->numPosition)
    {
        context->detected |= CY_CAPSENSE_GESTURE_TOUCHDOWN_MASK;
    }
    if (context->numPositionLast > context->numPosition)
    {
        context->detected |= CY_CAPSENSE_GESTURE_LIFTOFF_MASK;
    }
    if (0u != (gestureMask & CY_CAPSENSE_GESTURE_FILTERING_MASK))
    {
        Cy_CapSense_Gesture_Filtering(config, context);
    }
    
    /* Post processing */
    context->numPositionLast = context->numPosition;
    context->positionLast1 = context->position1;
    context->positionLast2 = context->position2;
}


/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_Filtering
****************************************************************************//**
*
* Performs detected gesture filtering. The gesture priority defined as follow:
*   * most important
* * two-finger zoom
* * two-finger scroll
* * one-finger rotate
* * one-finger edge swipe
* * one-finger flick
* * one-finger scroll
* * two-finger single click
* * one-finger click and drag
* * one-finger double click
* * one-finger single click
* * touchdown
* * liftoff
*   * least important
*
* Some of gestures cannot be detected simultaneously.
*
* \param config
* The pointer to the gesture configuration structure 
* \ref cy_stc_capsense_gesture_config_t.
*
* \param context
* The pointer to the gesture context structure 
* \ref cy_stc_capsense_gesture_context_t.
*
*******************************************************************************/
static void Cy_CapSense_Gesture_Filtering(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context)
{
    uint32_t gesDetection = context->detected;
    
    /* Filter gesture if at least one gesture is detected */
    if (0u != (gesDetection & CY_CAPSENSE_GESTURE_ALL_GESTURES_MASK))
    {
        /* 
        * TWO-FINGER Group Priority:
        * - ZOOM
        * - SCROLL 
        */
        if (0u != (gesDetection & CY_CAPSENSE_GESTURE_TWO_FNGR_ZOOM_MASK))
        {
            gesDetection = CY_CAPSENSE_GESTURE_TWO_FNGR_ZOOM_MASK;
        }
        else if (0u != (gesDetection & CY_CAPSENSE_GESTURE_TWO_FNGR_SCROLL_MASK))
        {
            gesDetection = CY_CAPSENSE_GESTURE_TWO_FNGR_SCROLL_MASK;
        }
        /* 
        * ONE-FINGER Group Priority:
        * The descending order of strength conditions:
        * - ROTATE (x4 movement distance in correct order)
        * - EDGE (first touch location + movement distance + timeout)
        * - FLICK (movement distance + timeout)
        * - SCROLL (movement distance)
        */
        else if (0u != (gesDetection & CY_CAPSENSE_GESTURE_ONE_FNGR_ROTATE_MASK))
        {
            gesDetection = CY_CAPSENSE_GESTURE_ONE_FNGR_ROTATE_MASK;
        }
        else if (0u != (gesDetection & CY_CAPSENSE_GESTURE_ONE_FNGR_EDGE_SWIPE_MASK))
        {
            gesDetection = CY_CAPSENSE_GESTURE_ONE_FNGR_EDGE_SWIPE_MASK;
        }
        else if (0u != (gesDetection & CY_CAPSENSE_GESTURE_ONE_FNGR_FLICK_MASK))
        {
            gesDetection = CY_CAPSENSE_GESTURE_ONE_FNGR_FLICK_MASK;
        }
        else if (0u != (gesDetection & CY_CAPSENSE_GESTURE_ONE_FNGR_SCROLL_MASK))
        {
            gesDetection = CY_CAPSENSE_GESTURE_ONE_FNGR_SCROLL_MASK;
        }
        /* 
        * Click Group Priority:
        * - DOUBLE CLICK overwrites SINGLE CLICK
        */
        else if (0u != (gesDetection & CY_CAPSENSE_GESTURE_TWO_FNGR_SINGLE_CLICK_MASK))
        {
            gesDetection = CY_CAPSENSE_GESTURE_TWO_FNGR_SINGLE_CLICK_MASK;
        }
        else if (0u != (gesDetection & CY_CAPSENSE_GESTURE_ONE_FNGR_CLICK_DRAG_MASK))
        {
            gesDetection = CY_CAPSENSE_GESTURE_ONE_FNGR_CLICK_DRAG_MASK;
        }
        else if (0u != (gesDetection & CY_CAPSENSE_GESTURE_ONE_FNGR_DOUBLE_CLICK_MASK))
        {
            gesDetection = CY_CAPSENSE_GESTURE_ONE_FNGR_DOUBLE_CLICK_MASK;
        }
        else if (0u != (gesDetection & CY_CAPSENSE_GESTURE_ONE_FNGR_SINGLE_CLICK_MASK))
        {
            gesDetection = CY_CAPSENSE_GESTURE_ONE_FNGR_SINGLE_CLICK_MASK;
        }
        else
        {
            gesDetection = CY_CAPSENSE_GESTURE_NO_GESTURE;
        }
        /* 
        * Action Priority:
        * TOUCHDOWN and LIFTOFF cannot be detected simultaneously and
        * are overwritten by any detected gesture.
        */
        context->detected = (uint32_t)gesDetection;
    }
}


/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_DecodeOneFgrSingleClick
****************************************************************************//**
*
* Performs decoding of one-finger single click gesture. This function is called 
* each scanning cycle.
*
* \param config
* The pointer to the gesture configuration structure 
* \ref cy_stc_capsense_gesture_config_t.
*
* \param context
* The pointer to the gesture context structure 
* \ref cy_stc_capsense_gesture_context_t.
*
*******************************************************************************/
static void Cy_CapSense_Gesture_DecodeOneFgrSingleClick(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context)
{
    uint32_t duration;
    cy_stc_capsense_ofsc_context_t * cxt = &context->ofscContext;
    
    if (context->numPosition >= 2U)
    {
        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
    }
    else
    {
        switch(cxt->state)
        {
            case CY_CAPSENSE_GESTURE_STATE_IDLE:
                if (context->numPosition == 1u)
                {
                    cxt->touchStartTime1 = context->timestamp;
                    cxt->touchStartPosition1 = context->position1;
                    cxt->state = CY_CAPSENSE_GESTURE_FIRST_TOUCH;
                }
                break;
                
            case CY_CAPSENSE_GESTURE_FIRST_TOUCH:
                if (context->numPosition == 0u)
                {
                    /* There could be single click gesture. Check conditions. */
                    duration = context->timestamp - cxt->touchStartTime1;
                    if ((config->clickTimeoutMax >= duration) &&
                        (config->clickTimeoutMin <= duration) &&
                        (0u != Cy_CapSense_Gesture_CheckDisplacement(config->clickDistanceMax, 
                                                                    &context->positionLast1, 
                                                                    &cxt->touchStartPosition1)))
                    {
                        /* Single click meets all conditions */
                        context->detected |= CY_CAPSENSE_GESTURE_ONE_FNGR_SINGLE_CLICK_MASK;
                    }
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                }
                break;
                
            case CY_CAPSENSE_GESTURE_INVALID:
                if (context->numPosition == 0u)
                {
                    /* No touches: Reset state */
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                }
                break;

            default:
                cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                break;
        }
    }
}


/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_DecodeOneFgrDoubleClick
****************************************************************************//**
*
* Performs decoding of one-finger double click gesture. This function is called 
* each scanning cycle.
*
* \param config
* The pointer to the gesture configuration structure 
* \ref cy_stc_capsense_gesture_config_t.
*
* \param context
* The pointer to the gesture context structure 
* \ref cy_stc_capsense_gesture_context_t.
*
*******************************************************************************/
static void Cy_CapSense_Gesture_DecodeOneFgrDoubleClick(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context)
{
    uint32_t duration;
    cy_stc_capsense_ofdc_context_t * cxt = &context->ofdcContext;
    
    if (context->numPosition >= 2U)
    {
        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
    }
    else
    {
        switch(cxt->state)
        {
            case CY_CAPSENSE_GESTURE_STATE_IDLE:
                if (context->numPosition == 1u)
                {
                    cxt->touchStartTime1 = context->timestamp;
                    cxt->touchStartPosition1 = context->position1;
                    cxt->state = CY_CAPSENSE_GESTURE_FIRST_TOUCH;
                }
                break;
                
            case CY_CAPSENSE_GESTURE_FIRST_TOUCH:
                if (context->numPosition == 0u)
                {
                    /* There could be single click gesture. Check conditions. */
                    duration = context->timestamp - cxt->touchStartTime1;
                    if ((config->clickTimeoutMax >= duration) &&
                        (config->clickTimeoutMin <= duration) &&
                        (0u != Cy_CapSense_Gesture_CheckDisplacement(config->clickDistanceMax, 
                                                                    &context->positionLast1, 
                                                                    &cxt->touchStartPosition1)))
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_FIRST_LIFTOFF;
                        cxt->touchStartPosition1 = context->positionLast1;
                        cxt->touchStartTime1 = context->timestamp;
                    }
                    else
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                    }
                }
                break;
                                
            case CY_CAPSENSE_GESTURE_FIRST_LIFTOFF:
                if (context->numPosition == 0u)
                {
                    duration = context->timestamp - cxt->touchStartTime1; /* Check second touch interval */
                    if (config->secondClickIntervalMax < duration)
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;   /* Timeout */
                    }
                }
                else
                {
                    /* Check conditions */
                    duration = context->timestamp - cxt->touchStartTime1;
                    if ((config->secondClickIntervalMax < duration) ||
                        (config->secondClickIntervalMin > duration) ||
                        (0u == Cy_CapSense_Gesture_CheckDisplacement(config->secondClickDistanceMax, 
                                                                    &context->position1, 
                                                                    &cxt->touchStartPosition1)))
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_FIRST_TOUCH;
                    }
                    else
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_SECOND_TOUCH;
                    }
                    cxt->touchStartTime1 = context->timestamp;
                    cxt->touchStartPosition1 = context->position1;
                }
                break;
                
            case CY_CAPSENSE_GESTURE_SECOND_TOUCH:
                if (context->numPosition == 0u)
                {
                    /* There could be single click gesture. Check conditions. */
                    duration = context->timestamp - cxt->touchStartTime1;
                    if ((config->clickTimeoutMax >= duration) &&
                        (config->clickTimeoutMin <= duration) &&
                        (0u != Cy_CapSense_Gesture_CheckDisplacement(config->clickDistanceMax, 
                                                                    &context->positionLast1, 
                                                                    &cxt->touchStartPosition1)))
                    {
                        /* Double click meets all conditions */
                        context->detected |= CY_CAPSENSE_GESTURE_ONE_FNGR_DOUBLE_CLICK_MASK;
                    }
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                }
                break;
                                
            case CY_CAPSENSE_GESTURE_INVALID:
                if (context->numPosition == 0u)
                {
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                }
                break;

            default:
                cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                break;
        }
    }
}


/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_DecodeOneFgrClickDrag
****************************************************************************//**
*
* Performs decoding of one-finger click and drag gesture. This function is 
* called each scanning cycle.
*
* \param config
* The pointer to the gesture configuration structure 
* \ref cy_stc_capsense_gesture_config_t.
*
* \param context
* The pointer to the gesture context structure 
* \ref cy_stc_capsense_gesture_context_t.
*
*******************************************************************************/
static void Cy_CapSense_Gesture_DecodeOneFgrClickDrag(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context)
{
    uint32_t duration;
    cy_stc_capsense_ofcd_context_t * cxt = &context->ofcdContext;
    
    if (context->numPosition >= 2U)
    {
        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
    }
    else
    {
        switch(cxt->state)
        {
            case CY_CAPSENSE_GESTURE_STATE_IDLE:
                if (context->numPosition == 1u)
                {
                    cxt->touchStartTime1 = context->timestamp;
                    cxt->touchStartPosition1 = context->position1;
                    cxt->state = CY_CAPSENSE_GESTURE_FIRST_TOUCH;
                }
                break;
                
            case CY_CAPSENSE_GESTURE_FIRST_TOUCH:
                if (context->numPosition == 0u)
                {
                    duration = context->timestamp - cxt->touchStartTime1;
                    if ((config->clickTimeoutMax >= duration) &&
                        (config->clickTimeoutMin <= duration) &&
                        (0u != Cy_CapSense_Gesture_CheckDisplacement(config->clickDistanceMax, 
                                                                    &context->positionLast1, 
                                                                    &cxt->touchStartPosition1)))
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_FIRST_LIFTOFF;
                        context->positionLast1 = cxt->touchStartPosition1;
                        cxt->touchStartTime1 = context->timestamp;
                    }
                    else
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                    }
                }
                break;
                                
            case CY_CAPSENSE_GESTURE_FIRST_LIFTOFF:
                if (context->numPosition == 0u)
                {
                    /* Check if timing is still valid */
                    duration = context->timestamp - cxt->touchStartTime1;
                    if (config->clickTimeoutMax < duration)
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                    }
                }
                else
                {
                    duration = context->timestamp - cxt->touchStartTime1;
                    if ((config->secondClickIntervalMax < duration) ||
                        (config->secondClickIntervalMin > duration) ||
                        (0u == Cy_CapSense_Gesture_CheckDisplacement(config->secondClickDistanceMax, 
                                                                    &context->position1, 
                                                                    &cxt->touchStartPosition1)))
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_FIRST_TOUCH;
                    }
                    else
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_SECOND_TOUCH;
                    }
                    cxt->touchStartTime1 = context->timestamp;
                    cxt->touchStartPosition1 = context->position1;
                }
                break;
                
            case CY_CAPSENSE_GESTURE_SECOND_TOUCH:
                if (context->numPosition == 0u)
                {
                    cxt->state = CY_CAPSENSE_GESTURE_FIRST_LIFTOFF;
                    context->positionLast1 = cxt->touchStartPosition1;
                    cxt->touchStartTime1 = context->timestamp;
                }
                else
                {
                    if (0u == Cy_CapSense_Gesture_CheckDisplacement(config->clickDistanceMax, 
                                                                    &context->position1, 
                                                                    &cxt->touchStartPosition1))
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_DETECTED;
                        context->detected |= CY_CAPSENSE_GESTURE_ONE_FNGR_CLICK_DRAG_MASK;
                    }
                }
                break;
                                
            case CY_CAPSENSE_GESTURE_DETECTED:
                if (context->numPosition == 0u)
                {
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                }
                else
                {
                    context->detected |= CY_CAPSENSE_GESTURE_ONE_FNGR_CLICK_DRAG_MASK;
                }
                break;
                
            case CY_CAPSENSE_GESTURE_INVALID:
                if (context->numPosition == 0u)
                {
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;   /* No touches: Reset state */
                }
                break;

            default:
                cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                break;
        }
    }
}


/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_DecodeTwoFgrSingleClick
****************************************************************************//**
*
* Performs decoding of two-finger click and drag gesture. This function is 
* called each scanning cycle.
*
* \param config
* The pointer to the gesture configuration structure 
* \ref cy_stc_capsense_gesture_config_t.
*
* \param context
* The pointer to the gesture context structure 
* \ref cy_stc_capsense_gesture_context_t.
*
*******************************************************************************/
static void Cy_CapSense_Gesture_DecodeTwoFgrSingleClick(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context)
{
    uint32_t duration;
    cy_stc_capsense_tfsc_context_t * cxt = &context->tfscContext;
    
    if (context->numPosition >= 3U)
    {
        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
    }
    else
    {
        switch(cxt->state)
        {
            case CY_CAPSENSE_GESTURE_STATE_IDLE:
                if (context->numPosition >= 1u)
                {
                    cxt->touchStartTime1 = context->timestamp;
                    cxt->touchStartPosition1 = context->position1;
                    cxt->state = CY_CAPSENSE_GESTURE_FIRST_TOUCH;
                } 
                if (context->numPosition >= 2u)
                {
                    cxt->touchStartTime2 = context->timestamp;
                    cxt->touchStartPosition2 = context->position2;
                    cxt->state = CY_CAPSENSE_GESTURE_SECOND_TOUCH;
                }
                break;
                
            case CY_CAPSENSE_GESTURE_FIRST_TOUCH:
                if (context->numPosition == 0u) 
                {
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                }
                else if (context->numPosition == 1u)
                {
                    /* Track timeouts and touch displacement to be sure that it meets conditions all the time */
                    duration = context->timestamp - cxt->touchStartTime1;
                    if (config->clickTimeoutMax < duration)
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
                    }
                }
                else if (context->numPosition == 2u)
                {
                    /* Track timeouts and touch displacement to be sure that it meets conditions all the time */
                    duration = context->timestamp - cxt->touchStartTime1;
                    if (config->clickTimeoutMax < duration)
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
                    } 
                    else
                    {
                        cxt->touchStartPosition1 = context->position1;
                        cxt->touchStartPosition2 = context->position2;
                        cxt->touchStartTime2 = context->timestamp;
                        cxt->state = CY_CAPSENSE_GESTURE_SECOND_TOUCH;
                    }
                }
                else /* (context->numPosition >= 3U) */
                {
                    cxt->state = CY_CAPSENSE_GESTURE_INVALID;
                }

                break;
            case CY_CAPSENSE_GESTURE_SECOND_TOUCH:
                if (context->numPosition == 0u)
                {
                    duration = context->timestamp - cxt->touchStartTime1;
                    if ((config->clickTimeoutMax < duration) ||
                        (config->clickTimeoutMin > duration) ||
                        (0u == Cy_CapSense_Gesture_CheckDisplacement(config->clickDistanceMax, 
                                                                    &context->positionLast1, 
                                                                    &cxt->touchStartPosition1)))
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                    }
                    duration = context->timestamp - cxt->touchStartTime2;
                    if ((config->clickTimeoutMax < duration) ||
                        (config->clickTimeoutMin > duration) ||
                        (0u == Cy_CapSense_Gesture_CheckDisplacement(config->clickDistanceMax, 
                                                                    &context->positionLast2, 
                                                                    &cxt->touchStartPosition2)))
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                    }
                    if (cxt->state == CY_CAPSENSE_GESTURE_SECOND_TOUCH)
                    {
                        context->detected |= CY_CAPSENSE_GESTURE_TWO_FNGR_SINGLE_CLICK_MASK;
                    }
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                }
                else if (context->numPosition == 1u)
                {
                    /* Track timeouts and touch displacement to be sure that it meets conditions all the time */
                    duration = context->timestamp - cxt->touchStartTime1;
                    if (config->clickTimeoutMax < duration)
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
                    }
                    else
                    {
                        cxt->touchStartPosition1 = context->position1;
                        cxt->state = CY_CAPSENSE_GESTURE_FIRST_LIFTOFF;
                    }
                }
                else if (context->numPosition == 2u)
                {
                    /* Track timeouts and touch displacement to be sure that it meets conditions all the time */
                    duration = context->timestamp - cxt->touchStartTime1;
                    if ((config->clickTimeoutMax < duration) ||
                        (0u == Cy_CapSense_Gesture_CheckDisplacement(config->clickDistanceMax, 
                                                                    &context->position1, 
                                                                    &cxt->touchStartPosition1)))
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
                    } 
                    duration = context->timestamp - cxt->touchStartTime2;
                    if ((config->clickTimeoutMax < duration) ||
                        (0u == Cy_CapSense_Gesture_CheckDisplacement(config->clickDistanceMax, 
                                                                    &context->position2, 
                                                                    &cxt->touchStartPosition2)))
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
                    } 
                }
                else /* (context->numPosition >= 3U) */
                {
                    cxt->state = CY_CAPSENSE_GESTURE_INVALID;
                }
                break;
                
            case CY_CAPSENSE_GESTURE_FIRST_LIFTOFF:
                if (context->numPosition == 0u)
                {
                    duration = context->timestamp - cxt->touchStartTime1;
                    if ((config->clickTimeoutMax >= duration) &&
                        (config->clickTimeoutMin <= duration) &&
                        (0u != Cy_CapSense_Gesture_CheckDisplacement(config->clickDistanceMax, 
                                                                    &context->positionLast1, 
                                                                    &cxt->touchStartPosition1)))
                    {
                        context->detected |= CY_CAPSENSE_GESTURE_TWO_FNGR_SINGLE_CLICK_MASK;
                    }
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                }
                else if (context->numPosition == 1u)
                {
                    /* Track timeouts and touch displacement to be sure that it meets conditions all the time */
                    duration = context->timestamp - cxt->touchStartTime1;
                    if (config->clickTimeoutMax < duration)
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
                    }
                }
                else if (context->numPosition == 2u)
                {
                    cxt->state = CY_CAPSENSE_GESTURE_INVALID;
                }
                else /* (context->numPosition >= 3U) */
                {
                    cxt->state = CY_CAPSENSE_GESTURE_INVALID;
                }
                break;
                
            case CY_CAPSENSE_GESTURE_INVALID:
                if (context->numPosition == 0u)
                {
                    /* No touches: Reset state */
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                }
                break;

            default:
                cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                break;
        }
    }
}


/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_DecodeOneFgrScroll
****************************************************************************//**
*
* Performs decoding of one-finger scroll gesture. This function is 
* called each scanning cycle.
*
* \param config
* The pointer to the gesture configuration structure 
* \ref cy_stc_capsense_gesture_config_t.
*
* \param context
* The pointer to the gesture context structure 
* \ref cy_stc_capsense_gesture_context_t.
*
*******************************************************************************/
static void Cy_CapSense_Gesture_DecodeOneFgrScroll(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context)
{
    uint32_t delta;
    uint32_t direction;
    cy_stc_capsense_ofsl_context_t * cxt = &context->ofslContext;

    switch(cxt->state)
    {
        case CY_CAPSENSE_GESTURE_STATE_IDLE:
            if (context->numPosition == 1u)
            {
                cxt->touchStartPosition1 = context->position1;
                cxt->debounce = config->scrollDebounce;
                cxt->direction = CY_CAPSENSE_GESTURE_DIRECTION_UNKNOWN;
                cxt->state = CY_CAPSENSE_GESTURE_FIRST_TOUCH;
            }
            break;
            
        case CY_CAPSENSE_GESTURE_FIRST_TOUCH:
            if (context->numPosition == 0u)
            {
                cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
            }
            else if (context->numPosition == 1u)
            {
                /* Determine direction */
                direction = Cy_CapSense_Gesture_DetermineQuadrant(context->position1.x, context->position1.y,
                                                           cxt->touchStartPosition1.x, cxt->touchStartPosition1.y);

                if ((CY_CAPSENSE_GESTURE_DIRECTION_UP == direction) || (CY_CAPSENSE_GESTURE_DIRECTION_DOWN == direction))
                {
                    delta = ABS_DIFF(context->position1.y, cxt->touchStartPosition1.y);
                }
                else
                {
                    delta = ABS_DIFF(context->position1.x, cxt->touchStartPosition1.x);
                }
                
                /* An activity is detected */
                if (delta >= config->scrollDistanceMin)
                {
                    /* If it is new direction then reset debounce counter */
                    if (cxt->direction != direction)
                    {
                        cxt->debounce = config->scrollDebounce;
                        cxt->direction = direction;
                    }
                    
                    if (cxt->debounce != 0u)
                    {
                        cxt->debounce--;
                    }

                    if (0u == cxt->debounce)
                    {
                        context->detected |= CY_CAPSENSE_GESTURE_ONE_FNGR_SCROLL_MASK;
                        context->direction |= direction << CY_CAPSENSE_GESTURE_DIRECTION_OFFSET_ONE_SCROLL;
                    }
                }
                cxt->touchStartPosition1 = context->position1;
            }
            else
            {
                cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
            }
            
            break;

        default:
            cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
            break;
    }
}

/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_DecodeTwoFgrScroll
****************************************************************************//**
*
* Performs decoding of two-finger scroll gesture. This function is 
* called each scanning cycle.
*
* \param config
* The pointer to the gesture configuration structure 
* \ref cy_stc_capsense_gesture_config_t.
*
* \param context
* The pointer to the gesture context structure 
* \ref cy_stc_capsense_gesture_context_t.
*
*******************************************************************************/
static void Cy_CapSense_Gesture_DecodeTwoFgrScroll(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context)
{
    uint32_t delta;
    uint32_t direction1;
    uint32_t direction2;
    uint32_t touchCenter;
    uint32_t touchCenterPrev;
    cy_stc_capsense_tfsl_context_t * cxt = &context->tfslContext;

    switch(cxt->state)
    {
        case CY_CAPSENSE_GESTURE_STATE_IDLE:
            if (context->numPosition == 2u)
            {
                cxt->touchStartPosition1 = context->position1;
                cxt->touchStartPosition2 = context->position2;
                cxt->debounce = config->scrollDebounce;
                cxt->direction = CY_CAPSENSE_GESTURE_DIRECTION_UNKNOWN;
                cxt->state = CY_CAPSENSE_GESTURE_SECOND_TOUCH;
            }

            break;
            
        case CY_CAPSENSE_GESTURE_SECOND_TOUCH:
            if (context->numPosition == 2u)
            {
                /* Determine direction */
                direction1 = Cy_CapSense_Gesture_DetermineQuadrant(context->position1.x, context->position1.y,
                                                           cxt->touchStartPosition1.x, cxt->touchStartPosition1.y);
                direction2 = Cy_CapSense_Gesture_DetermineQuadrant(context->position2.x, context->position2.y,
                                                           cxt->touchStartPosition2.x, cxt->touchStartPosition2.y);
                
                if (direction1 == direction2)
                {
                    if ((CY_CAPSENSE_GESTURE_DIRECTION_UP == direction1) || (CY_CAPSENSE_GESTURE_DIRECTION_DOWN == direction1))
                    {
                        touchCenter = (context->position1.y + context->position2.y) >> 1u;
                        touchCenterPrev = (cxt->touchStartPosition1.y + cxt->touchStartPosition2.y) >> 1u;
                    }
                    else
                    {
                        touchCenter = (context->position1.x + context->position2.x) >> 1u;
                        touchCenterPrev = (cxt->touchStartPosition1.x + cxt->touchStartPosition2.x) >> 1u;
                    }
                    delta = ABS_DIFF(touchCenter, touchCenterPrev);
                    
                    /* An activity is detected */
                    if (delta >= config->scrollDistanceMin)
                    {
                        /* If it is new direction then reset debounce counter */
                        if (cxt->direction != direction1)
                        {
                            cxt->debounce = config->scrollDebounce;
                            cxt->direction = direction1;
                        }
                        
                        if (cxt->debounce != 0u)
                        {
                            cxt->debounce--;
                        }

                        if (0u == cxt->debounce)
                        {
                            context->detected |= CY_CAPSENSE_GESTURE_TWO_FNGR_SCROLL_MASK;
                            context->direction |= direction1 << CY_CAPSENSE_GESTURE_DIRECTION_OFFSET_TWO_SCROLL;
                        }
                    }
                }
                cxt->touchStartPosition1 = context->position1;
                cxt->touchStartPosition2 = context->position2;
            }
            else
            {
                cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
            }
            
            break;

        default:
            cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
            break;
    }
}

/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_DecodeOneFgrFlick
****************************************************************************//**
*
* Performs decoding of one-finger flick gesture. This function is 
* called each scanning cycle.
*
* \param config
* The pointer to the gesture configuration structure 
* \ref cy_stc_capsense_gesture_config_t.
*
* \param context
* The pointer to the gesture context structure 
* \ref cy_stc_capsense_gesture_context_t.
*
*******************************************************************************/
static void Cy_CapSense_Gesture_DecodeOneFgrFlick(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context)
{
    uint32_t duration;
    uint32_t displacementX;
    uint32_t displacementY;
    uint32_t gestureDirection;
    cy_stc_capsense_offl_context_t * cxt = &context->offlContext;
    
    if (context->numPosition >= 2U)
    {
        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
    }
    else
    {
        switch(cxt->state)
        {
            case CY_CAPSENSE_GESTURE_STATE_IDLE:
                if (context->numPosition == 1u)
                {
                    cxt->touchStartTime1 = context->timestamp;
                    cxt->touchStartPosition1 = context->position1;
                    cxt->state = CY_CAPSENSE_GESTURE_FIRST_TOUCH;
                }
                break;
                
            case CY_CAPSENSE_GESTURE_FIRST_TOUCH:
                if (context->numPosition == 0u)
                {
                    /* Check conditions */
                    duration = context->timestamp - cxt->touchStartTime1;
                    displacementX = ABS_DIFF(context->positionLast1.x, cxt->touchStartPosition1.x);
                    displacementY = ABS_DIFF(context->positionLast1.y, cxt->touchStartPosition1.y);
                    if ((config->flickTimeoutMax >= duration) &&
                        ((config->flickDistanceMin <= displacementX) ||
                         (config->flickDistanceMin <= displacementY)))
                    {
                        displacementY <<= CY_CAPSENSE_TAN_BASE_SHIFT;
                        /* Find direction */
                        if (displacementY <= (displacementX * CY_CAPSENSE_TAN_22_5))
                        {
                            if (context->positionLast1.x > cxt->touchStartPosition1.x)
                            {
                                gestureDirection = CY_CAPSENSE_GESTURE_DIRECTION_RIGHT;
                            }
                            else
                            {
                                gestureDirection = CY_CAPSENSE_GESTURE_DIRECTION_LEFT;
                            }
                        }
                        else if (displacementY > (displacementX * CY_CAPSENSE_TAN_67_5))
                        {
                            if (context->positionLast1.y > cxt->touchStartPosition1.y)
                            {
                                gestureDirection = CY_CAPSENSE_GESTURE_DIRECTION_UP;
                            }
                            else
                            {
                                gestureDirection = CY_CAPSENSE_GESTURE_DIRECTION_DOWN;
                            }
                        }
                        else
                        {
                            if (context->positionLast1.x >= cxt->touchStartPosition1.x)
                            {
                                if (context->positionLast1.y >= cxt->touchStartPosition1.y)
                                {
                                    gestureDirection = CY_CAPSENSE_GESTURE_DIRECTION_UP_RIGHT;    
                                }
                                else
                                {
                                    gestureDirection = CY_CAPSENSE_GESTURE_DIRECTION_DOWN_RIGHT;    
                                }
                            }
                            else 
                            {
                                if (context->positionLast1.y >= cxt->touchStartPosition1.y)
                                {
                                    gestureDirection = CY_CAPSENSE_GESTURE_DIRECTION_UP_LEFT;    
                                }
                                else
                                {
                                    gestureDirection = CY_CAPSENSE_GESTURE_DIRECTION_DOWN_LEFT;    
                                }
                            }
                        }
                        context->detected |= CY_CAPSENSE_GESTURE_ONE_FNGR_FLICK_MASK;
                        context->direction |= gestureDirection << CY_CAPSENSE_GESTURE_DIRECTION_OFFSET_ONE_FLICK;
                    }
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                }
                else
                {
                    /* Track timeouts to be sure that gesture meets conditions all the time */
                    duration = context->timestamp - cxt->touchStartTime1;
                    if (config->flickTimeoutMax < duration)
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
                    }
                }
                break;
                                
            case CY_CAPSENSE_GESTURE_INVALID:
                if (context->numPosition == 0u)
                {
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;   /* No touches: Reset state */
                }
                break;

            default:
                cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                break;
        }
    }
}

/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_DecodeOneFgrEdgeSwipe
****************************************************************************//**
*
* Performs decoding of one-finger edge swipe gesture. This function is 
* called each scanning cycle.
*
* \param config
* The pointer to the gesture configuration structure 
* \ref cy_stc_capsense_gesture_config_t.
*
* \param context
* The pointer to the gesture context structure 
* \ref cy_stc_capsense_gesture_context_t.
*
*******************************************************************************/
static void Cy_CapSense_Gesture_DecodeOneFgrEdgeSwipe(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context)
{
    uint32_t duration;
    uint32_t angle;
    cy_stc_capsense_ofes_context_t * cxt = &context->ofesContext;
    
    if (context->numPosition >= 2u)
    {
        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
    }
    else
    {
        switch(cxt->state)
        {
            case CY_CAPSENSE_GESTURE_STATE_IDLE:
                if (context->numPosition == 1u)
                {
                    cxt->touchStartTime1 = context->timestamp;
                    cxt->touchStartPosition1 = context->position1;
                    
                    /* Get an edge */
                    cxt->edge = Cy_CapSense_Gesture_DetermineEdge(config, context);
                    if (CY_CAPSENSE_GESTURE_DIRECTION_UNKNOWN == cxt->edge)
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
                    }
                    else
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_FIRST_TOUCH;
                    }
                }
                break;
                
            case CY_CAPSENSE_GESTURE_FIRST_TOUCH:
                if (context->numPosition == 0u)
                {
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                }
                else
                {
                    /* Check timeout */
                    duration = context->timestamp - cxt->touchStartTime1;
                    if (duration > config->edgeTimeoutMax)
                    {
                        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
                    }
                    else
                    {
                        angle = Cy_CapSense_Gesture_CheckAngleWithinThresh(config, context);
                        /* Angle and displacement verification */
                        if (CY_CAPSENSE_GESTURE_DIRECTION_UNKNOWN == angle)
                        {
                            cxt->state = CY_CAPSENSE_GESTURE_INVALID;
                        }
                        else if (0u != angle)
                        {
                            cxt->state = CY_CAPSENSE_GESTURE_DETECTED;
                            context->detected |= CY_CAPSENSE_GESTURE_ONE_FNGR_EDGE_SWIPE_MASK;
                            context->direction |= cxt->edge << CY_CAPSENSE_GESTURE_DIRECTION_OFFSET_ONE_EDGE;
                        }
                    }
                }
                break;
                                
            case CY_CAPSENSE_GESTURE_DETECTED:
                if (context->numPosition == 0u)
                {
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                }
                break;
                
            case CY_CAPSENSE_GESTURE_INVALID:
                if (context->numPosition == 0u)
                {
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;   /* No touches: Reset state */
                }
                break;

            default:
                cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                break;
        }
    }
}

/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_DecodeTwoFgrZoom
****************************************************************************//**
*
* Performs decoding of two-finger zoom gesture. This function is 
* called each scanning cycle.
*
* \param config
* The pointer to the gesture configuration structure 
* \ref cy_stc_capsense_gesture_config_t.
*
* \param context
* The pointer to the gesture context structure 
* \ref cy_stc_capsense_gesture_context_t.
*
*******************************************************************************/
static void Cy_CapSense_Gesture_DecodeTwoFgrZoom(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context)
{
    int16_t displacementX;
    int16_t displacementY;
    int16_t deltaX;
    int16_t deltaY;
    uint32_t gestureDirection = CY_CAPSENSE_GESTURE_DIRECTION_UNKNOWN;
    cy_stc_capsense_tfzm_context_t * cxt = &context->tfzmContext;
    
    if (context->numPosition >= 3U)
    {
        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
    }
    else
    {
        switch(cxt->state)
        {
            case CY_CAPSENSE_GESTURE_STATE_IDLE:
                if (context->numPosition >= 1u)
                {
                    cxt->state = CY_CAPSENSE_GESTURE_FIRST_TOUCH;
                } 
                if (context->numPosition >= 2u)
                {
                    cxt->touchStartPosition1 = context->position1;
                    cxt->touchStartPosition2 = context->position2;
                    cxt->distanceX = ABS_DIFF(context->position1.x, context->position2.x);
                    cxt->distanceY = ABS_DIFF(context->position1.y, context->position2.y);
                    cxt->state = CY_CAPSENSE_GESTURE_SECOND_TOUCH;
                    cxt->debounce = config->zoomDebounce;
                }
                break;
                
            case CY_CAPSENSE_GESTURE_FIRST_TOUCH:
                if (context->numPosition == 0u)
                {
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                }
                else if (context->numPosition == 2u)
                {
                    cxt->touchStartPosition1 = context->position1;
                    cxt->touchStartPosition2 = context->position2;
                    cxt->distanceX = ABS_DIFF(context->position1.x, context->position2.x);
                    cxt->distanceY = ABS_DIFF(context->position1.y, context->position2.y);
                    cxt->state = CY_CAPSENSE_GESTURE_SECOND_TOUCH;
                    cxt->debounce = config->zoomDebounce;
                }
                else
                {
                    /* Do not change state when numPosition == 1u */
                }
                break;
            case CY_CAPSENSE_GESTURE_SECOND_TOUCH:
                if (context->numPosition == 0u)
                {
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                }
                else if (context->numPosition == 1u)
                {
                    cxt->state = CY_CAPSENSE_GESTURE_FIRST_TOUCH;
                }
                else if (context->numPosition == 2u)
                {
                    /* Calculate current distance between touches */
                    displacementX = ABS_DIFF(context->position1.x, context->position2.x);
                    displacementY = ABS_DIFF(context->position1.y, context->position2.y);
                    
                    /* Getting deltas */
                    deltaX = displacementX - (int16_t)cxt->distanceX;
                    deltaY = displacementY - (int16_t)cxt->distanceY;
                    
                    /* Check if deltas pass zoom active distances */
                    if ((deltaX > config->zoomDistanceMin) ||
                        (deltaY > config->zoomDistanceMin))
                    {
                        gestureDirection = CY_CAPSENSE_GESTURE_DIRECTION_IN;
                    } 
                    else if (deltaX < 0)
                    {
                        if (-deltaX > config->zoomDistanceMin)
                        {
                            gestureDirection = CY_CAPSENSE_GESTURE_DIRECTION_OUT;
                        }
                    }
                    else if (deltaY < 0)
                    {
                        if (-deltaY > config->zoomDistanceMin)
                        {
                            gestureDirection = CY_CAPSENSE_GESTURE_DIRECTION_OUT;
                        }
                    }
                    else
                    {
                        gestureDirection = CY_CAPSENSE_GESTURE_DIRECTION_UNKNOWN;
                    }
                    
                    if (CY_CAPSENSE_GESTURE_DIRECTION_UNKNOWN != gestureDirection)
                    {
                        cxt->distanceX = displacementX;
                        cxt->distanceY = displacementY;
                        /* Check timeout */
                        if (0u != config->zoomDebounce)
                        {
                            if (cxt->debounce > 0u)
                            {
                                cxt->debounce--;
                            }
                            if (cxt->debounce == 0u)
                            {
                                context->detected |= CY_CAPSENSE_GESTURE_TWO_FNGR_ZOOM_MASK;
                                context->direction |= (uint16_t)(gestureDirection << CY_CAPSENSE_GESTURE_DIRECTION_OFFSET_TWO_ZOOM);
                            }
                        }
                    }
                }
                else /* (context->numPosition >= 3U) */
                {
                    cxt->state = CY_CAPSENSE_GESTURE_INVALID;
                }
                
                break;
                
            case CY_CAPSENSE_GESTURE_INVALID:
                if (context->numPosition == 0u)
                {
                    /* No touches: Reset state */
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                }
                break;

            default:
                cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                break;
        }
    }
}

/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_DecodeOneFgrRotate
****************************************************************************//**
*
* Performs decoding of one-finger rotate gesture. This function is 
* called each scanning cycle.
*
* \param config
* The pointer to the gesture configuration structure 
* \ref cy_stc_capsense_gesture_config_t.
*
* \param context
* The pointer to the gesture context structure 
* \ref cy_stc_capsense_gesture_context_t.
*
*******************************************************************************/
static void Cy_CapSense_Gesture_DecodeOneFgrRotate(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context)
{
    uint32_t displacementX;
    uint32_t displacementY;
    uint32_t direction;
    uint32_t rotateHistory;
    cy_stc_capsense_ofrt_context_t * cxt = &context->ofrtContext;
    
    if (context->numPosition >= 2u)
    {
        cxt->state = CY_CAPSENSE_GESTURE_INVALID;
    }
    else
    {
        switch(cxt->state)
        {
            case CY_CAPSENSE_GESTURE_STATE_IDLE:
                if (context->numPosition == 1u)
                {
                    cxt->touchStartPosition1 = context->position1;
                    cxt->state = CY_CAPSENSE_GESTURE_FIRST_TOUCH;
                    cxt->debounce = config->rotateDebounce;
                    cxt->history = 0u;
                }
                break;
                
            case CY_CAPSENSE_GESTURE_FIRST_TOUCH:
                if (context->numPosition == 0u)
                {
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                }
                else
                {
                    displacementX = ABS_DIFF(context->position1.x, cxt->touchStartPosition1.x);
                    displacementY = ABS_DIFF(context->position1.y, cxt->touchStartPosition1.y);
                    
                    /* Check displacement */
                    if ((config->rotateDistanceMin <= displacementX) ||
                        (config->rotateDistanceMin <= displacementY))
                    {
                        /* Determine direction */
                        direction = Cy_CapSense_Gesture_DetermineQuadrant(context->position1.x, context->position1.y,
                                                                   cxt->touchStartPosition1.x, cxt->touchStartPosition1.y);
                        
                        /* Special case for empty history. Force a non-sequential set until 4 values are read */
                        rotateHistory = cxt->history;
                        if (0u == rotateHistory)
                        {
                            rotateHistory = direction + 2u;
                        }
                        /* New direction */
                        if (direction != (rotateHistory & CY_CAPSENSE_GESTURE_ROTATE_LAST_DIRECTION_MASK))
                        {
                            cxt->debounce = config->rotateDebounce;
                            /* Keep history updated */
                            rotateHistory = (uint8_t)((rotateHistory << 2u) | direction);
                            cxt->history = (uint8_t)rotateHistory;
                            /* Determine gesture code */
                            if ((rotateHistory == CY_CAPSENSE_GESTURE_ROTATE_ORDER_U_R_D_L) ||
                                (rotateHistory == CY_CAPSENSE_GESTURE_ROTATE_ORDER_L_U_R_D) ||
                                (rotateHistory == CY_CAPSENSE_GESTURE_ROTATE_ORDER_D_L_U_R) ||
                                (rotateHistory == CY_CAPSENSE_GESTURE_ROTATE_ORDER_R_D_L_U))
                            {
                                context->detected |= CY_CAPSENSE_GESTURE_ONE_FNGR_ROTATE_MASK;
                                context->direction |= (CY_CAPSENSE_GESTURE_DIRECTION_CW << CY_CAPSENSE_GESTURE_DIRECTION_OFFSET_ONE_ROTATE);
                            }
                            else if ((rotateHistory == CY_CAPSENSE_GESTURE_ROTATE_ORDER_U_L_D_R) ||
                                     (rotateHistory == CY_CAPSENSE_GESTURE_ROTATE_ORDER_R_U_L_D) ||
                                     (rotateHistory == CY_CAPSENSE_GESTURE_ROTATE_ORDER_D_R_U_L) ||
                                     (rotateHistory == CY_CAPSENSE_GESTURE_ROTATE_ORDER_L_D_R_U))
                            {
                                context->detected |= CY_CAPSENSE_GESTURE_ONE_FNGR_ROTATE_MASK;
                                context->direction |= (CY_CAPSENSE_GESTURE_DIRECTION_CCW << CY_CAPSENSE_GESTURE_DIRECTION_OFFSET_ONE_ROTATE);
                            }
                            else
                            {
                                /* Gesture is not detected. Detection status cleared commonly on upper level */
                            }
                        }
                        else
                        {
                            /* The same direction. Check debounce */
                            if (0u != config->rotateDebounce)
                            {
                                if (0u != cxt->debounce)
                                {
                                    cxt->debounce--;    
                                }
                                else
                                {
                                    /* Number of motion in the same direction exceeded */
                                    cxt->state = CY_CAPSENSE_GESTURE_INVALID;
                                }
                            }
                        }
                        /* Update position */
                        cxt->touchStartPosition1 = context->position1;
                    }
                }
                break;
                                
            case CY_CAPSENSE_GESTURE_INVALID:
                if (context->numPosition == 0u)
                {
                    cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;   /* No touches: Reset state */
                }
                break;

            default:
                cxt->state = CY_CAPSENSE_GESTURE_STATE_IDLE;
                break;
        }
    }
}


/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_DetermineQuadrant
****************************************************************************//**
*
* Determines directions.
*
* \param currX
* Current position X-axis.
*
* \param currY
* Current position Y-axis.
*
* \param prevX
* Previous position X-axis.
*
* \param prevY
* Previous position Y-axis.
*
* \return
* Returns direction either UP, DOWN, RIGHT or LEFT.
*
*******************************************************************************/
static uint32_t Cy_CapSense_Gesture_DetermineQuadrant(uint16_t currX, uint16_t currY, uint16_t prevX, uint16_t prevY)
{
    uint32_t quadrant;
    
    if ((ABS_DIFF(currX, prevX)) >= (ABS_DIFF(currY, prevY)))
    {
        if (currX >= prevX)
        {
            quadrant = CY_CAPSENSE_GESTURE_DIRECTION_RIGHT;
        }
        else
        {
            quadrant = CY_CAPSENSE_GESTURE_DIRECTION_LEFT;
        }
    }
    else
    {
        if (currY >= prevY)
        {
            quadrant = CY_CAPSENSE_GESTURE_DIRECTION_UP;
        }
        else
        {
            quadrant = CY_CAPSENSE_GESTURE_DIRECTION_DOWN;
        }
    }
    
    return (quadrant);
}


/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_DetermineEdge
****************************************************************************//**
*
* Determines direction of touch movement started on edge. If 
*
* \param config
* The pointer to the gesture configuration structure 
* \ref cy_stc_capsense_gesture_config_t.
*
* \param context
* The pointer to the gesture context structure 
* \ref cy_stc_capsense_gesture_context_t.
*
* \return
* Returns direction either UP, DOWN, RIGHT or LEFT. If movement started 
* not from an edge CY_CAPSENSE_GESTURE_DIRECTION_UNKNOWN is returned.
*
*******************************************************************************/
static uint32_t Cy_CapSense_Gesture_DetermineEdge(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context)
{
    uint32_t direction;
    uint32_t posX = context->position1.x;
    uint32_t posY = context->position1.y;
    uint32_t resX = config->resolutionX;
    uint32_t resY = config->resolutionY;
    uint32_t width = config->edgeEdgeSize;

    if ((posY >= width) && (posY < (resY - width)))
    {
        if (posX < width)
        {
            direction = CY_CAPSENSE_GESTURE_DIRECTION_RIGHT;
        }
        else if ((posX >= (resX - width)) && (posX <= resX))
        {
            direction = CY_CAPSENSE_GESTURE_DIRECTION_LEFT;
        }
        else
        {
            direction = CY_CAPSENSE_GESTURE_DIRECTION_UNKNOWN;
        }
    } 
    else if ((posX >= width) && (posX < (resX - width)))
    {
        if (posY < width)
        {
            direction = CY_CAPSENSE_GESTURE_DIRECTION_UP;
        }    
        else if ((posY >= (resY - width)) && (posY <= resY))
        {
            direction = CY_CAPSENSE_GESTURE_DIRECTION_DOWN;
        }
        else
        {
            direction = CY_CAPSENSE_GESTURE_DIRECTION_UNKNOWN;
        }
    }
    else
    {
        direction = CY_CAPSENSE_GESTURE_DIRECTION_UNKNOWN;
    }
    
    return direction;
}


/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_ArcTanOctant
****************************************************************************//**
*
* Calculates the angle associated with an X value and a Y value within the
* first octant of the coordinate plane (0 to 45 degrees). 
* Theory described in VASM#89.
* \note
*  1. Estimated error is +/-  90/127 degrees.
*
* \param x
* This value must be non-zero.  If it is zero, the result is undefined.
*
* \param y
* This value must be less than or equal to the value of x. If y
* is greater than x, the returned value is undefined.
*
* \return
* Angle in counts ranging from 0 to 64.  Each count represents an increment of
* 90/127 degrees.  However, 0 represents 0 degrees to 45/127 degrees, and 64
* represents exactly 45 degrees; i.e. This function returns 64 when X == Y.
*
*******************************************************************************/
static uint32_t Cy_CapSense_Gesture_ArcTanOctant(uint32_t x, uint32_t y)
{
    int32_t savedAngle = -CY_CAPSENSE_GESTURE_ARCTAN_COEFF_A;
    uint32_t scaledInputRatio;
    
    scaledInputRatio = (y << 8u) / x;
    
    savedAngle += CY_CAPSENSE_GESTURE_ARCTAN_COEFF_B * scaledInputRatio;
    savedAngle -= CY_CAPSENSE_GESTURE_ARCTAN_COEFF_C * scaledInputRatio * scaledInputRatio;
    
    return (((uint32_t)((savedAngle * 45) >> 23)) & 0x000001FFu);
}

/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_GetAngle
****************************************************************************//**
*  
* Calculates the angle associated with an X value and a Y value.
*
* \param x
* X value.
*
* \param y
* Y value.
*
* \return
* Returns calculated angle.
*
*******************************************************************************/
static int32_t Cy_CapSense_Gesture_GetAngle(uint32_t deltaX, uint32_t deltaY)
{
    int32_t angle;

    /* Determine the absolute value of the angle */
    if (deltaY <= deltaX)
    {
        /*
         * Angle is within 1 octant of the Y axis.  X is non-zero.  Therefore, Y
         * must be non-zero, so there will be no division by zero here.
         */
        angle = (int32_t)Cy_CapSense_Gesture_ArcTanOctant(deltaX, deltaY);
    }
    else /* (deltaY > deltaX) */
    {
        /*
         * Angle is more than 1 octant from the Y axis. absX is non-zero, so
         * there will be no division by zero here.
         */
        angle = 90 - (int32_t)Cy_CapSense_Gesture_ArcTanOctant(deltaY, deltaX);
    }
    return angle;
}

/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_CheckAngleWithinThresh
****************************************************************************//**
*
* Checks if an angle is within a threshold.
*
* \param config
* The pointer to the gesture configuration structure 
* \ref cy_stc_capsense_gesture_config_t.
*
* \param context
* The pointer to the gesture context structure 
* \ref cy_stc_capsense_gesture_context_t.
*
* \return
* Returns non-zero value if displacement is within the specified threshold.
*
*******************************************************************************/
static uint32_t Cy_CapSense_Gesture_CheckAngleWithinThresh(
                const cy_stc_capsense_gesture_config_t * config, 
                cy_stc_capsense_gesture_context_t * context)
{
    uint32_t deltaOpp; /* Opposite side */
    uint32_t deltaAdj; /* Adjacent side */
    uint32_t angle = 0u;
    uint32_t status = 0u;
    
    cy_stc_capsense_ofes_context_t * cxt = &context->ofesContext;
    uint32_t edgeType = cxt->edge;
    
    if ((edgeType == CY_CAPSENSE_GESTURE_DIRECTION_LEFT) || (edgeType == CY_CAPSENSE_GESTURE_DIRECTION_RIGHT))
    {
        deltaOpp = ABS_DIFF(context->position1.y, cxt->touchStartPosition1.y);
        deltaAdj = ABS_DIFF(context->position1.x, cxt->touchStartPosition1.x);
    }
    else /* ((edgeType == CY_CAPSENSE_GESTURE_DIRECTION_UP) || (edgeType == CY_CAPSENSE_GESTURE_DIRECTION_DOWN)) */
    {
        deltaOpp = ABS_DIFF(context->position1.x, cxt->touchStartPosition1.x);
        deltaAdj = ABS_DIFF(context->position1.y, cxt->touchStartPosition1.y);
    }

    if (deltaAdj >= config->edgeDistanceMin)
    {
        /* Prevent crash: 0 in denominator */
        if (deltaAdj != 0u)
        {
            if (deltaOpp != 0u)
            {
                angle = Cy_CapSense_Gesture_GetAngle(deltaAdj, deltaOpp);
            }
            if (angle <= config->edgeAngleMax)
            {
                status = 1u;
            }
            else
            {
                status = CY_CAPSENSE_GESTURE_DIRECTION_UNKNOWN;
            }
        }
        else
        {
            status = CY_CAPSENSE_GESTURE_DIRECTION_UNKNOWN;
        }
    }

    return status;
}


/*******************************************************************************
* Function Name: Cy_CapSense_Gesture_CheckDisplacement
****************************************************************************//**
*
* Checks if movement is within a thresholds.
*
* \param threshold
* Specifies the movement threshold.
*
* \param pos1
* Pointer to the current touch position structure 
* \ref cy_stc_capsense_gesture_position_t.
*
* \param pos2
* Pointer to the previous touch position structure 
* \ref cy_stc_capsense_gesture_position_t.
*
* \return
* Returns non-zero value if displacement is within the specified threshold.
*
*******************************************************************************/
static uint32_t Cy_CapSense_Gesture_CheckDisplacement(
                uint32_t threshold,
                cy_stc_capsense_gesture_position_t * pos1,
                cy_stc_capsense_gesture_position_t * pos2)
{
    uint32_t status = 0u;
    uint32_t displacementX = ABS_DIFF(pos1->x, pos2->x);
    uint32_t displacementY = ABS_DIFF(pos1->y, pos2->y);
    
    if ((threshold >= displacementX) && (threshold >= displacementY))
    {
        status = 1u;
    }

    return status;
}


/* [] END OF FILE */
