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
#include <pico/time.h>

// Personal libraries
#include "Timer.h"
#include "Display.h"
#include "Audio.h"
#include "Controls.h"

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
    const char *title;        // current title of song
    const char *artist;       // current artist of song
    const uint16_t *art;      // current album art of song
    uint8_t playing = 0;      // whether or not song is playing
    uint16_t elapsed_time;    // how long the song has been playing
    uint16_t song_duration;   // the length of the song
} SongData;

// **** Define any module-level, global, or external variables here ****
volatile bool timerEvent = false;
volatile uint32_t freeRunningCounter = 0;
volatile ControlEvent controlEvent = CONTROL_NONE;

// **** Function prototypes ****
// void Timer_Init(void);
// void Timer_End(void);
// bool Timer_ISR_Callback(struct repeating_timer *t);

/* -------------------------------------------------------------------------- */
/*                              Player State Machine                          */
/* -------------------------------------------------------------------------- */
// void playerSM(void)
// {
//     switch (song.state)
//     {
//     case SETUP:
//         // Display_Clear();
//         // Display_DrawAlbumArt(song.art);
//         // song.state = CHOOSING_MUSIC;
//         // break;
//     case CHOOSING_MUSIC:
//     case PLAYING:
//     case PAUSED:
//     case LOW_POWER:
//     }
// }

/* -------------------------------------------------------------------------- */
/*                              Setup, Loop, & Timer_ISR                      */
/* -------------------------------------------------------------------------- */
void setup(void)
{
    Serial.begin(115200); // Initialize serial monitor for debugging purposes

    // Initialize components
    Display_Init();
    Audio_Init();
    Timer_Init();
    Controls_Init();

    // DELETE LATER
    Serial.println("System initialized!");
}

void loop()
{
    // Poll Control event flag
    if (controlEvent != CONTROL_NONE)
    {
        playerSM();

        // clear control event
        controlEvent = CONTROL_NONE;
    }

    // Poll Timer event flag
    if (timerEvent == true)
    {
        playerSM();

        // clear Timer event flag
        timerEvent = false;
    }
}

/**
 * Timer_ISR_Callback()
 *
 * This function is called automatically every time the repeating timer expires.
 *  It sets timerEvent whenever the timer expires and the main loop polls the event
 *  and handles it in loop().
 */
bool Timer_ISR_Callback(struct repeating_timer *t)
{
    timerEvent = true;     // signal that a timer event occurred
    freeRunningCounter++;  // increment free running counter (counts in 10 ms clicks)
    ControlEvent event = Controls_CheckEvents();

    if (event != CONTROL_NONE)
    {
        controlEvent = event;
    }

    // return true so timer continues repeating
    return true; 
}