/**
 * @file    Timer.cpp
 *
 * Serves as the hardware timer for the RP2350.  
 *
 * @author  Madison Patiag
 *
 * @date    9/11/2026
 */
// Personal libraries.
#include "Timer.h"

// Module-level variable
struct repeating_timer timer;

/**
 * Timer_Init()
 *
 * Configures and starts the repeating timer.
 *
 * The timer period is 10 ms.
 */
void Timer_Init(void)
{
    // choose for ISR to run every 10 ms
    const int TIMER_PERIOD_MS = 10;

    // create/start the timer (provided in pico/time.h)
    //  (similar to using the HAL for the STM32)
    bool success = add_repeating_timer_ms(
        TIMER_PERIOD_MS,      // how often timer runs
        Timer_ISR_Callback,   // what function should be called (callback fxn)
        NULL,                 // what custom data we want to pass to callback fxn
        &timer                // where to store info about this timer
    );
}

/**
 * Timer_End()
 *
 * Stops the given repeating timer.
 */
void Timer_End(void)
{
    // cancels repeating timer from Timer_Init()
    cancel_repeating_timer(&timer);
}