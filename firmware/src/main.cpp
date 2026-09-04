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
#include "Display.h"
#include "Audio.h"
#include "Controls.h"
#include "clairo_charm.h" // remove after testing

// **** Set any preprocessor directives here ****


// **** Set any local typedefs here *****
typedef enum 
{
    SETUP,          // 0
    CHOOSING_MUSIC, // 1
    PLAYING,        // 2
    PAUSED,         // 3
    LOW_POWER       // 4
} PlayerState;

typedef struct 
{
    PlayerState state;        // current state of mp3 player
    const char *title;              // current title of song
    const char *artist;             // current artist of song
    const uint16_t *art;            // current album art of song
    uint8_t playing = 0;      // whether or not song is playing
    uint16_t elapsed_time;    // how long the song has been playing
    uint16_t song_duration;   // the length of the song
} SongData;

// **** Define any module-level, global, or external variables here ****
static SongData song = // this should be empty, just filled right now for testing display
{
    SETUP,
    "Second Nature",
    "Clairo",
    clairo_charm,
    0,
    0,
    100
}

void playerSM(void)
{
    switch (song.state)
    {
    case SETUP:
        // Display_Clear();
        // Display_DrawAlbumArt(song.art);
        // song.state = CHOOSING_MUSIC;
        // break;
    case CHOOSING_MUSIC:
    case PLAYING:
    case PAUSED:
    case LOW_POWER:
    }
}

void setup()
{
    Serial.begin(115200);

    // Wait for the serial monitor to connect
    while (!Serial)
    {
        delay(10);
    }

    // Serial.println("================================");
    // Serial.println("XIAO RP2350 Boot Successful!");
    // Serial.println("================================");
    // printf(
    //     "Welcome to Maddie's MP3 Player."
    //     "Compiled on %s %s. \n\r",
    //     __TIME__,
    //     __DATE__);

    // Initialize components
    Display_Init();
    // DFPlayer
    // Buttons


    /* TESTING */
    // Display_DrawText("HELLO!", 50, 50);
    Display_DrawAlbumArt(clairo_charm);
}

void loop()
{
    playerSM();
}

