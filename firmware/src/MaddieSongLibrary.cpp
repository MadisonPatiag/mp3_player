/**
 * @file    MaddieSongLibrary.cpp
 *
 * Holds all relevant information for each song within microSD card. 
 *
 * @author  Madison Patiag
 *
 * @date    9/16/2026
 */
#include "MaddieSongLibrary.h"

#include "clairo_charm.h"

const SongData songs[] =
{
    {
        "Charm",
        "Nomad",
        "Clairo",
        clairo_charm,
        1,
        1,
        226
    },

    {
        "Charm",
        "Sexy to Someone",
        "Clairo",
        clairo_charm,
        1,
        2,
        208
    },

    {
        "Charm",
        "Second Nature",
        "Clairo",
        clairo_charm,
        1,
        3,
        227
    },

    {
        "Charm",
        "Slow Dance",
        "Clairo",
        clairo_charm,
        1,
        4,
        234
    },

    {
        "Charm",
        "Thank You",
        "Clairo",
        clairo_charm,
        1,
        5,
        205
    },
};

const uint8_t num_songs = sizeof(songs) / sizeof(songs[0]);