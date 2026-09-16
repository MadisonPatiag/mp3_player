/**
 * @file    MaddieSongLibrary.h
 *
 * Holds all relevant information for each song within microSD card. 
 *
 * @author  Madison Patiag
 *
 * @date    9/16/2026
 */
#include <stdint.h>

typedef struct
{
    const char *album_title; // album title of song
    const char *song_title;  // current title of song
    const char *artist;      // current artist of song
    const uint16_t *art;     // current album art of song

    uint8_t folder;          // folder number within MaddieSongLibrary
    uint8_t track;           // track number within MaddieSongLibrary

    uint16_t duration;       // the length of the song in seconds
} SongData;

/**
 * Contains all the song data for each song stored in the microSD card
 */
extern const SongData songs[];

/**
 * Variable for total number of songs inside microSD card
 */
extern const uint8_t num_songs;