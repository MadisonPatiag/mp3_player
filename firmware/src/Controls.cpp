/**
 * @file    Controls.cpp
 *
 * Functional use of the ANO rotary encoder's scroll wheel/middle button and a single button. 
 *
 * @author  Madison Patiag
 *
 * @date    9/14/2026
 */
// **** Standard libraries. 
#include <Arduino.h>

// **** Personal libraries.
#include "Controls.h"

// **** Preprocessor directives.
#define ANO_ENCA    D16
#define ANO_ENCB    D17
#define ANO_SW1     D18

#define BUTTONS_DEBOUNCE_PERIOD 4 // 4 samples * 10 ms timer = 40 ms

static uint8_t previousEncoderState;
static int8_t encoderPosition; // tracks how many encoder transition's we've counted

/**
 * Initializes the proper pins such that the encoder buttons and center button may be used.
 */
void Controls_Init(void)
{
    // Initialize RP2350's GPIO pins for the ANO
    pinMode(ANO_ENCA, INPUT_PULLUP);
    pinMode(ANO_ENCB, INPUT_PULLUP);
    pinMode(ANO_SW1, INPUT_PULLUP);

    // Read the initial encoder state (0b00, 0b01, 0b10, 0b11)
    previousEncoderState = (digitalRead(ANO_ENCA) << 1) | digitalRead(ANO_ENCB);
    encoderPosition = 0;
}

/**
 * Checks the current button states and returns any events that have occured since its last call.  *  This function should be called repeatedly in a Timer ISR.
 */
ControlEvent Controls_CheckEvents(void)
{
    // State history for encoder
    static uint8_t state_history[BUTTONS_DEBOUNCE_PERIOD] = {HIGH, HIGH, HIGH, HIGH};
    static uint8_t state_history_oldest_index = 0;

    // Previous center button state
    static bool previous_button_state = HIGH;

    // Initial/default state of encoder and center button
    ControlEvent return_event = CONTROL_NONE;

    /*
     * ---------------------------------------------------------
     * CENTER BUTTON DEBOUNCING
     * ---------------------------------------------------------
     */

    // Record newest center button reading
    state_history[state_history_oldest_index] = digitalRead(ANO_SW1);

    // Move to next history location
    state_history_oldest_index = (state_history_oldest_index + 1) % BUTTONS_DEBOUNCE_PERIOD;

    // Determine whether all recent readings agree
    uint8_t sum = 0;

    for (int history = 0; history < BUTTONS_DEBOUNCE_PERIOD; history++)
    {
        sum += state_history[history];
    }

    // Button has been released
    if ((sum == BUTTONS_DEBOUNCE_PERIOD) && (previous_button_state == LOW))
    {
        previous_button_state = HIGH;
    }
    // Button has been pressed
    else if ((sum == 0) && (previous_button_state == HIGH))
    {
        previous_button_state = LOW;
        return_event = CONTROL_SELECT;
    }

    /*
     * ---------------------------------------------------------
     * ROTARY ENCODER
     * ---------------------------------------------------------
     */

    // Read encoder state
    uint8_t currentEncoderState = (digitalRead(ANO_ENCA) << 1) | digitalRead(ANO_ENCB);

    // Check if encoder changed
    if (currentEncoderState != previousEncoderState)
    {
        // save old and new encoder states
        uint8_t previous = previousEncoderState;
        uint8_t current = currentEncoderState;

        previousEncoderState = currentEncoderState;

        // CCW transition
        if ((previous == 0b00 && current == 0b01) ||
            (previous == 0b01 && current == 0b11) ||
            (previous == 0b11 && current == 0b10) ||
            (previous == 0b10 && current == 0b00))
        {
            encoderPosition++;

            // generate an event if received 4 quadrature transitions
            if (encoderPosition >= 4)
            {
                encoderPosition = 0;
                return_event = CONTROL_ROTATE_CCW;
            }
        }
        // CW transition
        else if ((previous == 0b00 && current == 0b10) ||
                 (previous == 0b10 && current == 0b11) ||
                 (previous == 0b11 && current == 0b01) ||
                 (previous == 0b01 && current == 0b00))
        {
            encoderPosition--;

            // generate an event if received 4 quadrature transitions
            if (encoderPosition <= -4)
            {
                encoderPosition = 0;
                return_event = CONTROL_ROTATE_CW;
            }
        }
    }

    // return event that occured
    return return_event;
}


