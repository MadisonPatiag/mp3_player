#ifndef AUDIO_H   
#define AUDIO_H
/**
 * @file    Audio.h
 *
 * Functional Audio module that retrieves and plays music from the DFPlayer.
 *
 * @author  Madison Patiag
 *
 * @date    9/4/2026
 */

/**
 * Initializes the DFPlayer.
 */
void Audio_Init(void);

/**
 * Plays the inputted audio track. 
 */
void Audio_Play(uint16_t track);

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