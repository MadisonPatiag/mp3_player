#ifndef CONTROLS_H   
#define CONTROLS_H
/**
 * @file    Audio.h
 *
 * This library provides an interface to the ANO Rotary Encoder, specifically for the 
 *  scroll wheel and center button. 
 *
 * This library relies on continuously polling the pins connected to the rotary encoder. 
 * It then provides an event-based interface for use. 
 * The resultant output are either UpEvents or DownEvents corresponding to 
 *  whether the button has been pushed-down or released.
 *
 * Be aware that the Controls_Init() function exists for configuring the appropriate pins 
 *  on the RP2350 processor and must be done before Controls_CheckEvents() will work.
 *
 * @author  Madison Patiag
 *
 * @date    9/4/2026
 */

/**
 * This function initializes the proper pins such that the buttons 1-4 may be 
 * used.
 */
void Controls_Init(void);

/**
 * 
 */
uint8_t Controls_CheckEvents(void);


#endif // CONTROLS_H