
#include <Arduino.h>

#include "Display.h"
#include "clairo_charm.h" // remove after testing   

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

