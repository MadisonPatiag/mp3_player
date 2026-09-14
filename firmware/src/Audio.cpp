/**
 * @file    Audio.cpp
 *
 * Retrieves and plays music from DFPlayer. 
 *
 * @author  Madison Patiag
 *
 * @date    9/12/2026
 */
// Standard libraries.
#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

// Personal libraries.
#include "Audio.h"

// Preprocessor directives.
#define DFR_TX  D12
#define DFR_RX  D11

// Objects.
static DFRobotDFPlayerMini dfPlayer;

/** Audio_Init().
 * 
 * Initializes the DFPlayer
 */
bool Audio_Init(void)
{
    Serial2.setRX(DFR_RX);  
    Serial2.setTX(DFR_TX);
    Serial2.begin(9600);

    // Initialize DFPlayer and return if it was successful or not
    return dfPlayer.begin(Serial2, true, true);
}

/**
 * Plays the inputted audio track from the respective folder. 
 */
void Audio_Play(uint8_t folderNumber, uint8_t trackNumber)
{
    dfPlayer.playFolder(folderNumber, trackNumber);
}

/**
 * Pauses the current audio track. 
 */
void Audio_Pause(void)
{
    dfPlayer.pause();
}

/**
 * Continues after a paused audio track. 
 */
void Audio_Resume(void)
{
    dfPlayer.start();
}

/**
 * Plays the next track after the current track. 
 */
void Audio_Next(void)
{
    dfPlayer.next();
}

/**
 * Plays the previous track before the current track. 
 */
void Audio_Previous(void)
{
    dfPlayer.previous();
}

