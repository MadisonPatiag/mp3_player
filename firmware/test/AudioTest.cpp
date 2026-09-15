#include <Arduino.h>
#include "Audio.h"

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("===== AUDIO MODULE TEST =====");

    if (Audio_Init())
    {
        Serial.println("Audio initialized!");
    }
    else
    {
        Serial.println("Audio initialization failed!");
    }

    Serial.println();
    Serial.println("Setting volume to 20...");
    Audio_SetVolume(20);

    delay(1000);

    Serial.println("Playing folder 01, track 001...");
    Audio_Play(1, 1);

    delay(5000);

    Serial.println("Pausing...");
    Audio_Pause();

    delay(3000);

    Serial.println("Resuming...");
    Audio_Resume();

    delay(5000);

    Serial.println("Next track...");
    Audio_Next();

    delay(5000);

    Serial.println("Previous track...");
    Audio_Previous();

    Serial.println();
    Serial.println("===== AUDIO TEST COMPLETE =====");
}

void loop()
{
}