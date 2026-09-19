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
#include <string>

// Personal libraries
#include "Timer.h"
#include "Display.h"
#include "Audio.h"
#include "Controls.h"

#include "MaddieSongLibrary.h"
#include "images/maxresdefault.h"

// **** Set any preprocessor directives here ****
#define LONG_PRESS      500 // 5 seconds (500 ticks * 10ms/tick)
#define SONGS_PER_PAGE  5

// **** Set any local typedefs here *****
typedef enum 
{
    SETUP,                  // 0
    CHOOSING_MUSIC,         // 1
    PLAYING,                // 2
    PAUSED,                 // 3
    POWER_BUTTON_PENDING,   // 4  
    LOW_POWER               // 5
} PlayerState;

typedef enum
{
    PREV_BUTTON,    // 0
    PLAY_BUTTON,    // 1
    NEXT_BUTTON     // 2
} SelectorState;

typedef struct
{
    PlayerState state;            // current state of mp3 player
    PlayerState previous_state;   // previous state of mp3 player
    SelectorState selector_mode;  // current state of selector

    uint16_t button_press_time;   // how long a button has been pressed
    uint8_t selectedSong;   // the user's selected song 
    uint16_t elapsed_time;  // how long the song has been playing
} PlayerData;

// **** Define any module-level, global, or external variables here ****
uint8_t firstVisibleSong;
uint8_t previousFirstVisibleSong;
uint8_t previousSelectedSong;
uint8_t oldSelectorY;

static volatile PlayerData player = 
{
    SETUP, 
    SETUP,
    PLAY_BUTTON,
    0,
    0,
    0
};

volatile ControlEvent controlEvent = CONTROL_NONE;
volatile bool timerEvent = false;
static volatile uint32_t freeRunningCounter = 0;
static uint16_t startTime = 0;

/* -------------------------------------------------------------------------- */
/*                              Player State Machine                          */
/* -------------------------------------------------------------------------- */
/* This function helps updates TFT to reflect the state. */
void updateDisplay(void)
{
    uint16_t lineHeight = 24;

    if (player.state == SETUP)
    {
        // CHANGE FILE NAME TO BMO FOR CLARITY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! (later)
        Display_DrawAlbumArt(maxresdefault);
    }
    else if (player.state == CHOOSING_MUSIC) // show all available songs to play
    {
        // Clear old song list (MAKE THIS BETTERRRRRRRRRRRRRRRRRRRR!!!!!!!!!!!!!)
        // Display_FillRect(0, 30, 320, 210, 0);

        // Calculate number of visible songs per page
        uint8_t visibleSongs = num_songs - firstVisibleSong;
        if (visibleSongs > SONGS_PER_PAGE)
        {
            visibleSongs = SONGS_PER_PAGE;
        }

        // display 5 visible songs at a time
        // previousFirstVisibleSong = firstVisibleSong;

        /* Draw main menu for selecting songs */ 
        for (uint8_t i = 0; i < visibleSongs; i++)
        {
            uint8_t songIndex = firstVisibleSong + i;

            // Draw selector icon
            Display_DrawText(
                (songIndex == player.selectedSong) ? ">" : " ",
                5,              // x position
                40 + (i * 35),  // changes y position equally for each song
                2, 2
            );
            
            // Format the string ("song name - artist") directly into the buffer
            char label[128]; // Temporary buffer on the stack
            snprintf(label, sizeof(label), 
                "%s - %s", 
                songs[songIndex].song_title, songs[songIndex].artist);

            // Draw available songs as a list
            Display_DrawTextTruncated(
                label,
                20,             // x position
                40 + (i * 35),  // changes y position equally for each song
                2, 2, 295
            );
        }
    }
    else if (player.state == PLAYING || player.state == PAUSED)
    {
        // Clear previous screen
        // Display_FillRect(0, 30, 320, 210, 0);

        // Playing/Paused screen position
        uint16_t xPos = 150;
        uint16_t currentY = 40;
        uint16_t maxTextWidth = 320 - xPos; // 170px space available next to album art

        // Draw Album Art
        Display_DrawAlbumArt(songs[player.selectedSong].art);

        // Draw song title
        Display_DrawTextTruncated(songs[player.selectedSong].song_title, xPos, currentY, 2, 2, maxTextWidth);
        currentY += 22;

        // Draw artist text
        Display_DrawTextTruncated(songs[player.selectedSong].artist, xPos, currentY, 1, 1, maxTextWidth);
        currentY += 25;

        // Draw album title
        Display_DrawTextTruncated(songs[player.selectedSong].album_title, xPos, currentY, 1, 1, maxTextWidth);

        /* Draw Prev, Play/Pause, Next Bar */
        // Play Button
        if (player.state == PLAYING)
        {
            Display_FillRect(150, 170, 18, 18, ILI9341_BLACK);
            if (player.selector_mode == PLAY_BUTTON)
            {
                Display_DrawPlay(ILI9341_LIGHTGREY);
            }
            else
            {
                Display_DrawPlay(ILI9341_WHITE);
            }
        }
        // Pause Button
        else if (player.state == PAUSED)
        {
            Display_FillRect(150, 170, 18, 18, ILI9341_BLACK);
            if (player.selector_mode == PLAY_BUTTON)
            {
                Display_DrawPause(ILI9341_LIGHTGREY);
            }
            else
            {
                Display_DrawPause(ILI9341_WHITE);
            }
        }
        
        // Next Button
        if (player.selector_mode == NEXT_BUTTON)
        {
            Display_DrawNext(ILI9341_LIGHTGREY);
        }
        else
        {
            Display_DrawNext(ILI9341_WHITE);
        }

        // Prev Button
        if (player.selector_mode == PREV_BUTTON)
        {
            Display_DrawPrev(ILI9341_LIGHTGREY);
        }
        else
        {
            Display_DrawPrev(ILI9341_WHITE);
        }

        // Progress bar
        Display_DrawProgress(player.elapsed_time, songs[player.selectedSong].duration);
    }
    else if (player.state == LOW_POWER)
    {
        // CHANGE THIS LATER!!!!!!!!!!!!!!!!!!!!!
        Display_Clear();
    }
}

/* Helper function for updating Selector states based on encoder events */
SelectorState Selector_Move(SelectorState currentSelector, ControlEvent event)
{
    /* CW event */
    if (event == CONTROL_ROTATE_CW)
    {
        if (currentSelector == PREV_BUTTON)
        {
            return PLAY_BUTTON;
        }
        else if (currentSelector == PLAY_BUTTON)
        {
            return NEXT_BUTTON;
        }
        else
        {
            return PREV_BUTTON;
        }
    }
    /* CCW event */
    else if (event == CONTROL_ROTATE_CCW)
    {
        if (currentSelector == PREV_BUTTON)
        {
            return NEXT_BUTTON;
        }
        else if (currentSelector == PLAY_BUTTON)
        {
            return PREV_BUTTON;
        }
        else
        {
            return PLAY_BUTTON;
        }
    }

    return currentSelector;
}

/* MP3 Player State Machine */
void playerSM(void)
{
    // Check if power button clicked at any state
    if (controlEvent == CONTROL_POWER_DOWN)
    {
        player.previous_state = player.state;
        startTime = freeRunningCounter;
        player.state = POWER_BUTTON_PENDING;
    }

    switch (player.state)
    {
    case SETUP:
        // if center button pressed then change state to CHOOSING_MUSIC
        if (controlEvent == CONTROL_SELECT)
        {
            player.state = CHOOSING_MUSIC;
            updateDisplay();
        }
        break;

    case CHOOSING_MUSIC:
        /* Encoder events to scroll through previous or next songs */
        if (controlEvent == CONTROL_ROTATE_CW)
        {
            // Select next song
            player.selectedSong++;

            // Update number of songs to show and what song is displayed first
            if (player.selectedSong >= (firstVisibleSong + SONGS_PER_PAGE))
            {
                firstVisibleSong = player.selectedSong - SONGS_PER_PAGE + 1;
            }

            // If at end of song selection list, loop back to top
            if (player.selectedSong >= num_songs)
            {
                player.selectedSong = 0;
            }
            
            updateDisplay();
        }
        else if (controlEvent == CONTROL_ROTATE_CCW)
        {
            // Selects previous song
            if (player.selectedSong == 0)
            {
                player.selectedSong = num_songs - 1;

                // Update number of songs to show and what song is displayed first
                if (num_songs > SONGS_PER_PAGE)
                {
                    firstVisibleSong = num_songs - SONGS_PER_PAGE;
                }
                else
                {
                    firstVisibleSong = 0;
                }
            }
            // If at beginning of song selection list, loop to bottom of list
            else
            {
                player.selectedSong--;

                // update number of songs to show
                if (player.selectedSong < firstVisibleSong)
                {
                    firstVisibleSong = player.selectedSong;
                }
            }

            updateDisplay();
        }

        /* Center button event to select chosen song */
        // Updates player's current song information
        if (controlEvent == CONTROL_SELECT)
        {
            Audio_Play(songs[player.selectedSong].folder, songs[player.selectedSong].track);
            player.state = PLAYING;
            player.selector_mode = PLAY_BUTTON;
            player.elapsed_time = 0;

            Display_FillRect(0, 30, 320, 210, 0);
            updateDisplay();
        }

        break;

    case PLAYING:
        // Update elapsed_time every 1 sec for progress bar
        if (timerEvent)
        {
            // 100 ticks * 10ms = 1000ms = 1 sec
            if ((freeRunningCounter % 100) == 0)
            {
                if (player.elapsed_time < songs[player.selectedSong].duration)
                {
                    player.elapsed_time++;
                    
                    // Redraw only the progress bar to prevent full screen flicker
                    Display_DrawProgress(player.elapsed_time, songs[player.selectedSong].duration);
                }
            }
        }

        // Update Selector state based on scroll event
        if (controlEvent == CONTROL_ROTATE_CW || controlEvent == CONTROL_ROTATE_CCW)
        {
            player.selector_mode = Selector_Move(player.selector_mode, controlEvent);
            updateDisplay();
        }

        /* Center button event */ 
        if (controlEvent == CONTROL_SELECT)
        {
            // Pause song
            if (player.selector_mode == PLAY_BUTTON)
            {
                Audio_Pause();
                player.state = PAUSED;
            } 
            // Go to next song and play it
            else if (player.selector_mode == NEXT_BUTTON)
            {
                Audio_Next();
                player.elapsed_time = 0; // Reset time for next track

                // update player's selected song
                player.selectedSong++;
                if (player.selectedSong >= num_songs)
                {
                    player.selectedSong = 0;
                }

                Display_FillRect(150, 40, 170, 100, ILI9341_BLACK);
            }
            // Go to previous song and play it
            else if (player.selector_mode == PREV_BUTTON)
            {
                Audio_Previous();
                player.elapsed_time = 0; // Reset time for previous track

                // update player's selected song
                if (player.selectedSong == 0)
                {
                    player.selectedSong = num_songs - 1;
                }
                else
                {
                    player.selectedSong--;
                }

                Display_FillRect(150, 40, 170, 100, ILI9341_BLACK);
            }

            updateDisplay();
        }
    
        break;

    case PAUSED:
        // Update Selector state based on scroll event
        if (controlEvent == CONTROL_ROTATE_CW || controlEvent == CONTROL_ROTATE_CCW)
        {
            player.selector_mode = Selector_Move(player.selector_mode, controlEvent);
            updateDisplay();
        }

        /* Center button event */ 
        if (controlEvent == CONTROL_SELECT)
        {
            // Play song
            if (player.selector_mode == PLAY_BUTTON)
            {
                Audio_Resume();
                player.state = PLAYING;
            } 
            // Go to next song and play it
            else if (player.selector_mode == NEXT_BUTTON)
            {
                Audio_Next();
                player.elapsed_time = 0; // Reset time for next track

                // update player's selected song
                player.selectedSong++;
                if (player.selectedSong >= num_songs)
                {
                    player.selectedSong = 0;
                }

                player.state = PLAYING;
                Display_FillRect(150, 40, 170, 100, ILI9341_BLACK);

            }
            // Go to previous song and play it
            else if (player.selector_mode == PREV_BUTTON)
            {
                Audio_Previous();
                player.elapsed_time = 0; // Reset time for previous track

                // update player's selected song
                if (player.selectedSong == 0)
                {
                    player.selectedSong = num_songs - 1;
                }
                else
                {
                    player.selectedSong--;
                }

                player.state = PLAYING;
                Display_FillRect(150, 40, 170, 100, ILI9341_BLACK); 
            }

            updateDisplay();
        }
    
        break;

    case POWER_BUTTON_PENDING:
        // Go back to CHOOSING_MUSIC state if press button for less than LONG_PRESS
        // (don't stop music from playing doe)
        if (controlEvent == CONTROL_POWER_UP)
        {
            player.button_press_time = freeRunningCounter - startTime;

            // enter LOW_POWER state if button has been pressed for LONG_PRESS
            if (player.button_press_time >= LONG_PRESS)
            {
                // EnterLowPower();
                player.state = LOW_POWER;
            }
            // else go to previous state or to CHOOSING_MUSIC state
            else
            {
                if (player.previous_state == PLAYING || player.previous_state == PAUSED)
                {
                    player.state = CHOOSING_MUSIC;
                }
                else
                {
                    player.state = player.previous_state;
                }
            }

            updateDisplay();
        }
        break;

    case LOW_POWER:
        // if (power button generates WAKE UP event)
            // wake up and change state to SETUP
        // else
            // stay in LOW_POWER (normal program execution is suspended)

        break;
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