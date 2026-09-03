/**
 * @file    Display.h
 *
 * Displays text and album art onto the TFT ILI9341. 
 *
 * @author  Madison Patiag
 *
 * @date    8/22/2026
 */
// Standard libraries.
#include <Adafruit_ILI9341.h>
#include <SPI.h>

// Personal libraries.
#include "Display.h"

// Preprocessor directives.
#define TFT_CS    D3
#define TFT_DC    D6
#define TFT_RESET D7

#define ALBUM_ART_X       100
#define ALBUM_ART_Y       40
#define ALBUM_ART_WIDTH   160
#define ALBUM_ART_HEIGHT  160

// Objects.
Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RESET);

/**
 * Initializes the display and clears the screen by rotating it horizontally and 
 *  filling it with black. 
 */
void Display_Init(void)
{
    tft.begin();
    tft.setRotation(1); // makes the display horizontal
    tft.fillScreen(ILI9341_BLACK);
}

/**
 * Clears the display.
 */
void Display_Clear(void)
{
    tft.fillScreen(ILI9341_BLACK);
}

/**
 * This function puts text onto the ILI9341.
 */
void Display_DrawText(const char* text, int x, int y)
{
    tft.setCursor(x, y);                // set text cursor location
    tft.setTextColor(ILI9341_WHITE);    // set text font color w/ transparent bg
    tft.setTextSize(2);                 // set text size
    tft.print(text);                    // print text
}

/**
 * This function displays the album art onto the ILI9341.
 */
void Display_DrawAlbumArt(const uint16_t *art)
{
    // start the write and set the appropriate window/section to take up on the display
    tft.startWrite(); // this is for writing huge sections onto the display
    tft.setAddrWindow(ALBUM_ART_X, ALBUM_ART_Y, ALBUM_ART_WIDTH, ALBUM_ART_HEIGHT);

    // iterate over *art and send the color of each pixel into the sectioned area
    for (int y = 0; y < ALBUM_ART_HEIGHT; y++)
    {
        for (int x = 0; x < ALBUM_ART_WIDTH; x++)
        {
            tft.pushColor(art[y * ALBUM_ART_WIDTH + x]);
        }
    }

    // end the write operation
    tft.endWrite();
}