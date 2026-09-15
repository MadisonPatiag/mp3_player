#ifndef CONTROLS_H   
#define CONTROLS_H
/**
 * @file    Controls.h
 *
 * This library provides an interface to the ANO Rotary Encoder, specifically for the 
 *  scroll wheel and center button. 
 *
 * This library relies on continuously polling the pins connected to the rotary encoder. 
 * It then provides an event-based interface for use. 
 *
 * Be aware that the Controls_Init() function exists for configuring the appropriate pins 
 *  on the RP2350 processor and must be done before Controls_CheckEvents() will work.
 *
 * @author  Madison Patiag
 *
 * @date    9/14/2026
 */
#include <stdint.h>

typedef enum
{
    CONTROL_NONE,
    CONTROL_ROTATE_CW,
    CONTROL_ROTATE_CCW,
    CONTROL_SELECT,
    CONTROL_POWER
} ControlEvent;

/**
 * Initializes the proper pins such that the encoder buttons and center button may be used.
 */
void Controls_Init(void);

/**
 * Checks the current button states and returns any events that have occured since its last call.  *  This function should be called repeatedly in a Timer ISR.
 */
ControlEvent Controls_CheckEvents(void);


#endif // CONTROLS_H