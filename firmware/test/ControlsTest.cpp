/**
 * @file    ControlsTest.cpp
 *
 * Functinoal use of the ANO rotary encoder's scroll wheel/middle button and a single button. 
 *
 * @author  Madison Patiag
 *
 * @date    9/14/2026
 */
// **** Standard libraries.
#include <Arduino.h>

// **** Personal libraries.
#include "Controls.h"

void setup(void)
{
    Serial.begin(115200);

    delay(2000);

    Serial.println();
    Serial.println("===== CONTROLS MODULE TEST =====");

    Controls_Init();

    Serial.println("Controls initialized!");
    Serial.println("Rotate the encoder or press the center button.");
    Serial.println();
}

void loop(void)
{
    ControlEvent event = Controls_CheckEvents();

    switch (event)
    {
        case CONTROL_ROTATE_CW:
            Serial.println("ROTATE CLOCKWISE");
            break;

        case CONTROL_ROTATE_CCW:
            Serial.println("ROTATE COUNTER-CLOCKWISE");
            break;

        case CONTROL_SELECT:
            Serial.println("SELECT");
            break;

        case CONTROL_POWER:
            Serial.println("POWER");
            break;

        case CONTROL_NONE:
            break;
    }

    delay(1);
}