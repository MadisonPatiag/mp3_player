#ifndef AUDIO_H   
#define AUDIO_H
/**
 * @file    Timer.h
 *
 * Serves as the hardware timer for the RP2350.  
 *
 * @author  Madison Patiag
 *
 * @date    9/11/2026
 */
#include <stdint.h>
#include <pico/time.h>

/**
 * Timer_ISR_Callback()
 *
 * This function is called automatically every time the repeating timer expires.
 */
bool Timer_ISR_Callback(struct repeating_timer *t);

/**
 * Timer_Init()
 *
 * Configures and starts the repeating timer.
 *
 * The timer period is 10 ms.
 */
void Timer_Init(void);

/**
 * Timer_End()
 *
 * Stops the given repeating timer.
 */
void Timer_End(void);


#endif // AUDIO_H