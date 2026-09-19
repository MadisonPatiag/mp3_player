/**
 * @file    Display.cpp
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

#define ALBUM_ART_X       20
#define ALBUM_ART_Y       20
#define ALBUM_ART_WIDTH   120
#define ALBUM_ART_HEIGHT  120

// Center controls below the album art/text area
#define CONTROLS_Y       170
#define PLAY_ICON_X      150
#define PLAY_ICON_Y      CONTROLS_Y
#define PLAY_ICON_SIZE   18  // Height/width of triangle

// UI Control Positioning
#define PREV_ICON_X      (PLAY_ICON_X - 45)
#define NEXT_ICON_X      (PLAY_ICON_X + 45)

#define PROGRESS_BAR_X   20
#define PROGRESS_BAR_Y   150
#define PROGRESS_BAR_W   280
#define PROGRESS_BAR_H   6

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
 * Displays text at the specified position.
 */
void Display_DrawText(
    const char* text, 
    int x, 
    int y, 
    uint8_t sizeX, 
    uint8_t sizeY, 
    uint16_t fgColor, 
    uint16_t bgColor)
{
    tft.setFont(NULL);      // Use default font
    tft.setTextWrap(false); // Prevents wrapping to X=0
    tft.setCursor(x, y);    // Where to place text box
    tft.setTextColor(fgColor, bgColor); // Text color and text background color
    tft.setTextSize(sizeX, sizeY);      // Asymmetric scale to alter text size
    tft.print(text);    // Print desired string
}

/**
 * Truncates text with "..." if it exceeds maxPixelWidth
 */
void Display_DrawTextTruncated(
    const char* text, 
    int x, 
    int y, 
    int sizeX, 
    int sizeY, 
    uint16_t maxPixelWidth, 
    uint16_t fgColor, 
    uint16_t bgColor)
{
    // Calculate font and screen capacity
    uint16_t charWidth = 6 * sizeX; // Base font width is 6px per size multiplier
    uint16_t maxChars = maxPixelWidth / charWidth;

    // If length of text exceeds screen capacity then truncate and draw the text,
    //  else just draw the text as it is
    if (strlen(text) > maxChars)
    {
        // truncate and draw ellipsis
        char truncated[64];
        if (maxChars > 3)
        {
            snprintf(truncated, sizeof(truncated), "%.*s...", maxChars - 3, text);
        }
        else
        {
            snprintf(truncated, sizeof(truncated), "%.*s", maxChars, text);
        }
        Display_DrawText(truncated, x, y, sizeX, sizeY, fgColor, bgColor);
    }
    else
    {
        Display_DrawText(text, x, y, sizeX, sizeY, fgColor, bgColor);
    }
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

/** 
 * Fill rectangle of given size with a specific color.
 *  Makes it easier to clear unwanted text. 
 */
void Display_FillRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color)
{
    tft.fillRect(x, y, width, height, color);
}

/**
 * Draws a filled play triangle at the default controls position.
 */
void Display_DrawPlay(uint16_t color)
{
    // Points defining a right-pointing triangle:
    // Point 0 (Top-left):    x, y
    // Point 1 (Bottom-left): x, y + size
    // Point 2 (Right-tip):   x + size, y + (size / 2)
    
    int16_t x0 = PLAY_ICON_X;
    int16_t y0 = PLAY_ICON_Y;
    int16_t x1 = PLAY_ICON_X;
    int16_t y1 = PLAY_ICON_Y + PLAY_ICON_SIZE;
    int16_t x2 = PLAY_ICON_X + PLAY_ICON_SIZE;
    int16_t y2 = PLAY_ICON_Y + (PLAY_ICON_SIZE / 2);

    tft.fillTriangle(x0, y0, x1, y1, x2, y2, color);
}

/**
 * Draws filled pause rectangles at the default controls position.
 */
void Display_DrawPause(uint16_t color)
{
    uint8_t barWidth = 5;
    uint8_t barGap = 6;

    // Left bar
    tft.fillRect(PLAY_ICON_X, PLAY_ICON_Y, barWidth, PLAY_ICON_SIZE, color);

    // Right bar
    tft.fillRect(PLAY_ICON_X + barWidth + barGap, PLAY_ICON_Y, barWidth, PLAY_ICON_SIZE, color);
}

/**
 * Draws double right-facing arrows for skipping forward.
 */
void Display_DrawNext(uint16_t color)
{
    int16_t size = PLAY_ICON_SIZE;
    int16_t half = size / 2;
    int16_t offset = size / 2;

    // Triangle 1 (facing right)
    tft.fillTriangle(NEXT_ICON_X, PLAY_ICON_Y, 
                     NEXT_ICON_X, PLAY_ICON_Y + size, 
                     NEXT_ICON_X + half, PLAY_ICON_Y + half, color);

    // Triangle 2 (facing right)
    tft.fillTriangle(NEXT_ICON_X + offset, PLAY_ICON_Y, 
                     NEXT_ICON_X + offset, PLAY_ICON_Y + size, 
                     NEXT_ICON_X + offset + half, PLAY_ICON_Y + half, color);
}

/**
 * Draws double left-facing arrows for skipping backward.
 */
void Display_DrawPrev(uint16_t color)
{
    int16_t size = PLAY_ICON_SIZE;
    int16_t half = size / 2;
    int16_t offset = size / 2;

    // Triangle 1 (facing Left)
    tft.fillTriangle(PREV_ICON_X + half, PLAY_ICON_Y, 
                     PREV_ICON_X + half, PLAY_ICON_Y + size, 
                     PREV_ICON_X, PLAY_ICON_Y + half, color);

    // Triangle 2 (facing Left)
    tft.fillTriangle(PREV_ICON_X + half + offset, PLAY_ICON_Y, 
                     PREV_ICON_X + half + offset, PLAY_ICON_Y + size, 
                     PREV_ICON_X + offset, PLAY_ICON_Y + half, color);
}

/**
 * Draws a track timeline progress bar based on the current player state.
 */
void Display_DrawProgress(uint16_t elapsedTime, uint16_t totalDuration, uint16_t barColor)
{
    // Prevent division by zero
    if (totalDuration == 0) 
    {
        totalDuration = 1;
    }

    // Prevents bar overflow
    if (elapsedTime > totalDuration) 
    {
        elapsedTime = totalDuration;
    }

    // Draw outer board of bar
    tft.drawRect(PROGRESS_BAR_X, PROGRESS_BAR_Y, PROGRESS_BAR_W, PROGRESS_BAR_H, ILI9341_DARKGREY);

    // Calculate filled pixel width
    uint16_t fillWidth = (elapsedTime * (PROGRESS_BAR_W - 2)) / totalDuration;

    // Draw filled/played region
    if (fillWidth > 0)
    {
        tft.fillRect(PROGRESS_BAR_X + 1, PROGRESS_BAR_Y + 1, fillWidth, PROGRESS_BAR_H - 2, barColor);
    }

    // Clear unplayed region
    uint16_t remainingWidth = (PROGRESS_BAR_W - 2) - fillWidth;
    if (remainingWidth > 0)
    {
        tft.fillRect(PROGRESS_BAR_X + 1 + fillWidth, PROGRESS_BAR_Y + 1, remainingWidth, PROGRESS_BAR_H - 2, ILI9341_BLACK);
    }
}