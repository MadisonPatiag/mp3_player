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

#include "MaddieSongLibrary.h"
#include "bmo_face.h"

// **** Set any preprocessor directives here ****
#define LONG_PRESS      3
#define SONGS_PER_PAGE  5

// **** Set any local typedefs here *****
typedef enum 
{
    SETUP,          // 0
    CHOOSING_MUSIC, // 1
    PLAYING,        // 2
    PAUSED,         // 3
    LOW_POWER       // 4
} PlayerState;

typedef enum
{
    PAUSE_BUTTON,
    PLAY_BUTTON,
    NEXT_BUTTON,
    PREV_BUTTON
} SelectorState;

typedef struct
{
    PlayerState state;            // current state of mp3 player
    SelectorState selector_mode;  // current state of selector

    uint8_t selectedSong;   // the user's selected song 
    uint16_t elapsed_time;  // how long the song has been playing
} PlayerData;

// **** Define any module-level, global, or external variables here ****
uint8_t firstVisibleSong;
static volatile PlayerData player = 
{
    SETUP, 
    PLAY_BUTTON,
    0,
    0
};

volatile bool timerEvent = false;
volatile uint32_t freeRunningCounter = 0;
volatile ControlEvent controlEvent = CONTROL_NONE;

/* -------------------------------------------------------------------------- */
/*                              Player State Machine                          */
/* -------------------------------------------------------------------------- */
/* This function helps updates TFT to reflect the state. */
void updateDisplay(void)
{
    if (player.state == SETUP)
    {
        Display_DrawAlbumArt(bmo_face);
    }
    else if (player.state == CHOOSING_MUSIC) // show all available songs to play    
    {
        // count number of songs stored in SD card using for loop?

        // Display each song stored in SD card
    }
    else if (player.state == PLAYING)
    {
        Display_DrawAlbumArt(songs[player.selectedSong].art);
        Display_DrawText(songs[player.selectedSong].song_title, 120, 50);
        Display_DrawText(songs[player.selectedSong].album_title, 120, 70);
        // Display_DrawPlay();
        // Display_DrawForward();
        // Display_DrawReverse();
        // Display_DrawProgress();
    }
    else if (player.state == PAUSED)
    {
        Display_DrawAlbumArt(songs[player.selectedSong].art);
        Display_DrawText(songs[player.selectedSong].song_title, 120, 50);
        Display_DrawText(songs[player.selectedSong].album_title, 120, 70);
        // Display_DrawPause();
        // Display_DrawForward();
        // Display_DrawReverse();
        // Display_DrawProgress();
    }
    else if (player.state == LOW_POWER)
    {
        Display_Clear();
    }
}

/* MP3 Player State Machine */
void playerSM(void)
{
    switch (player.state)
    {
    case SETUP:
        // if (center button pressed)
            // change state to CHOOSING_MUSIC
        if (controlEvent == CONTROL_SELECT)
        {
            player.state = CHOOSING_MUSIC;
            updateDisplay();
        }
        break;

    case CHOOSING_MUSIC:
        // Encoder events to scroll through previous or next songs
        if (controlEvent == CONTROL_ROTATE_CW)
        {
            // selector selects next song
            player.selectedSong++;

            // if at end of song selection list, loop back to top
            if (player.selectedSong >= num_songs)
            {
                player.selectedSong = 0;
            }
            updateDisplay();
        }
        else if (controlEvent == CONTROL_ROTATE_CCW)
        {
            // selector selects previous song
            if (player.selectedSong == 0)
            {
                player.selectedSong = num_songs - 1;
            }
            // if at beginning of song selection list, loop to bottom of list
            else
            {
                player.selectedSong--;
            }
            updateDisplay();
        }

        // Center button event to select chosen song
        if (controlEvent == CONTROL_SELECT)
        {
            player.state = PLAYING;
            Audio_Play(songs[player.selectedSong].folder, songs[player.selectedSong].track);
            updateDisplay();
        }

        break;

    case PLAYING:
        // want display bar for this to be (prev, play/pause, next) 
        //  with the default selector being the play/pause
        if (controlEvent == CONTROL_ROTATE_CW)
        {
            // selector state is now next button (FROM DEFAULT)
            // else goes to next selector state from the current selector state???

            // updateDisplay();
        }

        if (controlEvent == CONTROL_ROTATE_CCW)
        {
            // same thing as CW but vice versa

            // updateDisplay();
        }

        /* Center button events */ 
        if (controlEvent == CONTROL_SELECT && player.selector_mode == PLAY_BUTTON)
        {
            Audio_Pause();
            player.state = PAUSED;
            // change play button into pause button to reflect the now paused state
            // updateDisplay();
        } 
        else if (controlEvent == CONTROL_SELECT && player.selector_mode == NEXT_BUTTON)
        {
            Audio_Next();
            updateDisplay();
        }
        else if (controlEvent == CONTROL_SELECT && player.selector_mode == PREV_BUTTON)
        {
            Audio_Previous();
            updateDisplay();
        }
        break;

    case PAUSED:
        // essentially same functionality as PLAYING state, 
        //  but vice versa for player.selector_mode == PAUSE_BUTTON

    case LOW_POWER:
        // if (long press on off button)
            // enter low power mode
        // if (already off and long press off button)
            // enter SETUP state
    }
}

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