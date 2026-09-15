#pragma once // had to use pragma instead of header guard since it wasn't working how I intended
/**
 * @file    Audio.h
 *
 * Functional Audio module that retrieves and plays music from the DFPlayer.
 *
 * @author  Madison Patiag
 *
 * @date    9/12/2026
 */
#include <stdint.h>

/**
 * Initializes the DFPlayer.
 */
bool Audio_Init(void);

/**
 * Plays the inputted audio track from the respective folder. 
 */
void Audio_Play(uint8_t folderNumber, uint8_t trackNumber);

/**
 * Pauses the current audio track. 
 */
void Audio_Pause(void);


/**
 * Continues after a paused audio track. 
 */
void Audio_Resume(void);

/**
 * Plays the next track after the current track. 
 */
void Audio_Next(void);

/**
 * Plays the previous track before the current track. 
 */
void Audio_Previous(void);

/**
 * Sets the appropriate volume.
 */
void Audio_SetVolume(uint8_t volume);

/**
 * Increase volume.
 */
void Audio_IncreaseVolume(void);

/**
 * Decrease volume.
 */
void Audio_DecreaseVolume(void);
