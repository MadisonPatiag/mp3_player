#ifndef AUDIO_H   
#define AUDIO_H
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


#endif // AUDIO_H