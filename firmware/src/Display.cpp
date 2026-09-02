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

#define TFT_CS    D3
#define TFT_DC    D6
#define TFT_RESET D7

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RESET);

/**
 * Initializes the display and clears the screen.
 */
void Display_Init(void)
{
    tft.begin();

    Serial.println("TFT initialization complete.");

    tft.fillScreen(ILI9341_RED);

    Serial.println("TFT fill complete.");
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
    tft.setCursor(x, y);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print(text);
}

/**
 * This function displays the album art onto the ILI9341.
 */
int Display_DrawAlbumArt(unsigned char art, int x, int y);