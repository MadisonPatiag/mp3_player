/**
 * @file    Audio.cpp
 *
 * Retrieves and plays music from DFPlayer. 
 *
 * @author  Madison Patiag
 *
 * @date    9/12/2026
 */
#include <Arduino.h>
#include "Audio.h"

#define DFR_RX D11
#define DFR_TX D12

/*
 * Helper function that sends a 10-byte command packet to the DFPlayer.
 */
static void Audio_SendCommand( uint8_t command, uint8_t parameterHigh, uint8_t parameterLow)
{
    uint8_t packet[10];

    packet[0] = 0x7E;   // start byte - tells DFPlayer a command packet is beginning
    packet[1] = 0xFF;   // version byte - tells DFPlayer which protocol we're using
    packet[2] = 0x06;   // length byte - indicates number of bytes in command's data
    packet[3] = command;    // command byte - executes specific action 
    packet[4] = 0x01;       // ACK byte - tells DFPlayer to send acknowledgement response 
    packet[5] = parameterHigh;  // parameter byte
    packet[6] = parameterLow;   // parameter byte

    // Calculate checksum for packets 1-6
    uint16_t sum = 0;
    for (int i = 1; i <= 6; i++)
    {
        sum += packet[i];
    }
    uint16_t checksum = 0 - sum;

    packet[7] = (checksum >> 8) & 0xFF; // upper byte of checksum
    packet[8] = checksum & 0xFF;        // lower byte of checksum
    packet[9] = 0xEF;                   // end byte - tells DFPlayer command packet ends here

    // Send entire 10 byte packet over UART
    Serial2.write(packet, sizeof(packet));  
    Serial2.flush(); // waits for transmission to finish
}

/**
 * Initializes the DFPlayer.
 */
bool Audio_Init(void)
{
    // Initialize and start RP2350's UART pins
    Serial2.setRX(DFR_RX);
    Serial2.setTX(DFR_TX);
    Serial2.begin(9600);

    // Give DFPlayer time to start up
    delay(3000);

    return true;
}

/**
 * Plays the inputted audio track from the respective folder. 
 */
void Audio_Play(uint8_t folderNumber, uint8_t trackNumber)
{
    Audio_SendCommand(
        0x0F,   // folder playback command
        folderNumber,
        trackNumber
    );
}

/**
 * Pauses the current audio track. 
 */
void Audio_Pause(void)
{
    Audio_SendCommand(
        0x0E,   // pause command
        0x00,   // 0x00 means no param
        0x00
    );
}

/**
 * Continues after a paused audio track. 
 */
void Audio_Resume(void)
{
    Audio_SendCommand(
        0x0D,   // resume command
        0x00,
        0x00
    );
}

/**
 * Plays the next track after the current track. 
 */
void Audio_Next(void)
{
    Audio_SendCommand(
        0x01,   // next command
        0x00,
        0x00
    );
}

/**
 * Plays the previous track before the current track. 
 */
void Audio_Previous(void)
{
    Audio_SendCommand(
        0x02,   // previous command
        0x00,
        0x00
    );
}

/**
 * Sets the appropriate volume 
 */
void Audio_SetVolume(uint8_t volume)
{
    Audio_SendCommand(
        0x06,   // volume command
        0x00,
        volume  // requested volume
    );
}

/**
 * Increase volume.
 */
void Audio_IncreaseVolume(void)
{
    Audio_SendCommand(
        0x04,   // increase volume command
        0x00,
        0x00  
    );
}

/**
 * Decrease volume.
 */
void Audio_DecreaseVolume(void)
{
    Audio_SendCommand(
        0x05,   // decrease volume command
        0x00,
        0x00  
    );
}
