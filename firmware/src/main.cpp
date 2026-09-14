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

static SongData song = // this should be empty, just filled right now for testing display
{
    SETUP,
    "Second Nature",
    "Clairo",
    clairo_charm,
    0,
    0,
    100
};

// **** Function prototypes ****
void Timer_Init(void);
void Timer_End(void);
bool Timer_ISR_Callback(struct repeating_timer *t);

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
/*                              Setup, Loop, & Timer_ISR                                  */
/* -------------------------------------------------------------------------- */
void setup()
{
    Serial.begin(115200); // Initialize serial monitor for debugging purposes

    // Wait for serial monitor to connect
    while (!Serial)
    {
        delay(10);
    }

    printf(
        "Welcome to Maddie's MP3 Player.\n"
        "Compiled on %s %s. \n\r",
        __TIME__,
        __DATE__);

    // Initialize components
    Display_Init();
    bool audio_init_success = Audio_Init();
    // Controls - Controls_Init();

    if (!audio_init_success)
    {
        Serial.println("DFPlayer initialization failed!");
    }
    else
    {
        Serial.println("DFPlayer initialized successfully!");
    }


    /* TESTING */
    // Display Testing
    Display_Clear();
    // Display_DrawText("HELLO!", 50, 50);
    Display_DrawAlbumArt(clairo_charm);
    
    // Timer Testing
    // Serial.println("Starting timer test...");
    // Timer_Init();

    // Audio Testing
    
    Audio_Play(1,1);
}

void loop()
{
    /*
     * The timer ISR sets timerEvent whenever the timer expires.
     *
     * The main loop polls this event and handles it here.
     */
    // Poll timer event flag 
    if (timerEvent == true)
    {
        timerEvent = false; // clear timer event flag (asap so it's only processed once)
        
        // playerSM(); // run player state machine ONLY when timer event triggered

        /*
         * For testing:
         * Print the free-running counter every timer event.
         */
        // Serial.print("Timer event! Counter = ");
        // Serial.println(freeRunningCounter);
    }
}

/**
 * Timer_ISR_Callback()
 *
 * This function is called automatically every time the repeating timer expires.
 */
bool Timer_ISR_Callback(struct repeating_timer *t)
{
    timerEvent = true;     // signal that a timer event occurred
    freeRunningCounter++;  // increment free running counter (counts in 10 ms clicks)

    // return true so timer continues repeating
    return true; 
}