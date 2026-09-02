/**
 * @file    main.cpp
 * 
 * MP3 Player firmware
 *
 * @author  Madison Patiag
 *
 * @date    8/3/2026
 */
// **** Include libraries here **** 
// Standard libraries
#include <Arduino.h>

// Personal libraries
#include <Display.h>

void setup()
{
    Serial.begin(115200);

    // Wait for the serial monitor to connect
    while (!Serial)
    {
        delay(10);
    }

    Serial.println("================================");
    Serial.println("Portable MP3 Player");
    Serial.println("XIAO RP2350 Boot Successful!");
    Serial.println("================================");

    // DISPLAY TESTING
    Display_Init();
    Display_DrawText("HELLO!", 20, 20);z
}

void loop()
{
    // Nothing here yet.
}