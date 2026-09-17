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
#define OFF_BACK_BTN D13
#define INC_VOL_BTN  D14
#define DEC_VOL_BTN  D15
#define ANO_ENCA     D16
#define ANO_ENCB     D17
#define ANO_SW1      D18

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

    // Initialize RP2350's GPIO pins for switch buttons
    pinMode(OFF_BACK_BTN, INPUT_PULLUP);
    pinMode(INC_VOL_BTN, INPUT_PULLUP);
    pinMode(DEC_VOL_BTN, INPUT_PULLUP);

    // Read the initial encoder state (0b00, 0b01, 0b10, 0b11) and power button
    previousEncoderState = (digitalRead(ANO_ENCA) << 1) | digitalRead(ANO_ENCB);
    encoderPosition = 0;
}

/**
 * Checks the current button states and returns any events that have occured since its last call.  *  This function should be called repeatedly in a Timer ISR.
 */
ControlEvent Controls_CheckEvents(void)
{
    // State history for center button and power button
    static uint8_t center_state_history[BUTTONS_DEBOUNCE_PERIOD] = {HIGH, HIGH, HIGH, HIGH};
    static uint8_t center_state_history_oldest_index = 0;
    static uint8_t power_state_history[BUTTONS_DEBOUNCE_PERIOD] = {HIGH, HIGH, HIGH, HIGH};
    static uint8_t power_state_history_oldest_index = 0;
    static uint8_t inc_vol_state_history[BUTTONS_DEBOUNCE_PERIOD] = {HIGH, HIGH, HIGH, HIGH};
    static uint8_t inc_vol_state_history_oldest_index = 0;
    static uint8_t dec_vol_state_history[BUTTONS_DEBOUNCE_PERIOD] = {HIGH, HIGH, HIGH, HIGH};
    static uint8_t dec_vol_state_history_oldest_index = 0;

    // Previous center, power, and volume button states
    static bool previous_center_button_state = HIGH;
    static bool previous_power_button_state = HIGH;
    static bool previous_inc_volume_state = HIGH;
    static bool previous_dec_volume_state = HIGH;

    // Initial/default state of all buttons
    ControlEvent return_event = CONTROL_NONE;

    /*
    * ---------------------------------------------------------
    * POWER / BACK BUTTON DEBOUNCING
    * ---------------------------------------------------------
    */
    // Record newest power button reading
    power_state_history[power_state_history_oldest_index] = digitalRead(OFF_BACK_BTN);

    // Move to next history location
    power_state_history_oldest_index =
        (power_state_history_oldest_index + 1) % BUTTONS_DEBOUNCE_PERIOD;

    // Determine whether all recent readings agree
    uint8_t power_sum = 0;

    for (int history = 0; history < BUTTONS_DEBOUNCE_PERIOD; history++)
    {
        power_sum += power_state_history[history];
    }

    // Button has been pressed
    if ((power_sum == 0) && (previous_power_button_state == HIGH))
    {
        previous_power_button_state = LOW;
        return_event = CONTROL_POWER_DOWN;
    }
    // Button has been released
    else if ((power_sum == BUTTONS_DEBOUNCE_PERIOD) && (previous_power_button_state == LOW))
    {
        previous_power_button_state = HIGH;
        return_event = CONTROL_POWER_UP;
    }

    /*
     * ---------------------------------------------------------
     * CENTER AND INCREASE/DECREASE BUTTON DEBOUNCING
     * ---------------------------------------------------------
     */
    // Record newest button reading
    center_state_history[center_state_history_oldest_index] = digitalRead(ANO_SW1);
    inc_vol_state_history[inc_vol_state_history_oldest_index] = digitalRead(INC_VOL_BTN);
    dec_vol_state_history[dec_vol_state_history_oldest_index] = digitalRead(DEC_VOL_BTN);

    // Move to next history location
    center_state_history_oldest_index = 
        (center_state_history_oldest_index + 1) % BUTTONS_DEBOUNCE_PERIOD;
    inc_vol_state_history_oldest_index = 
        (inc_vol_state_history_oldest_index + 1) % BUTTONS_DEBOUNCE_PERIOD;
    dec_vol_state_history_oldest_index = 
        (dec_vol_state_history_oldest_index + 1) % BUTTONS_DEBOUNCE_PERIOD;

    // Determine whether all recent readings agree
    uint8_t center_sum = 0;
    uint8_t inc_vol_sum = 0;
    uint8_t dec_vol_sum = 0;

    for (int history = 0; history < BUTTONS_DEBOUNCE_PERIOD; history++)
    {
        center_sum += center_state_history[history];
        inc_vol_sum += inc_vol_state_history[history];
        dec_vol_sum += dec_vol_state_history[history];
    }

    // CENTER has been released
    if ((center_sum == BUTTONS_DEBOUNCE_PERIOD) && (previous_center_button_state == LOW))
    {
        previous_center_button_state = HIGH;
    }
    // CENTER has been pressed
    else if ((center_sum == 0) && (previous_center_button_state == HIGH))
    {
        previous_center_button_state = LOW;
        return_event = CONTROL_SELECT;
    }

    // INCREASE VOLUME has been released
    if ((inc_vol_sum == BUTTONS_DEBOUNCE_PERIOD) && (previous_inc_volume_state == LOW))
    {
        previous_inc_volume_state = HIGH;
    }
    // INCREASE VOLUME has been pressed
    else if ((inc_vol_sum == 0) && (previous_inc_volume_state == HIGH))
    {
        previous_inc_volume_state = LOW;
        return_event = CONTROL_VOLUME_INCREASE;
    }

    // DECREASE VOLUME has been released
    if ((dec_vol_sum == BUTTONS_DEBOUNCE_PERIOD) && (previous_dec_volume_state == LOW))
    {
        previous_dec_volume_state = HIGH;
    }
    // DECREASE VOLUME has been pressed
    else if ((dec_vol_sum == 0) && (previous_dec_volume_state == HIGH))
    {
        previous_dec_volume_state = LOW;
        return_event = CONTROL_VOLUME_DECREASE;
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


