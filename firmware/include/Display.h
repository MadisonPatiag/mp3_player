#ifndef DISPLAY_H   
#define DISPLAY_H
/**
 * @file    Display.h
 *
 * Functional Display module that displays text and album art onto the TFT ILI9341. 
 *
 * @author  Madison Patiag
 *
 * @date    8/22/2026
 */
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

/** Display_Init()
 * 
 * Initializes the display and clears the screen.
 */
void Display_Init(void);

/** Display_Clear()
 * 
 * Clears the display.
 */
void Display_Clear(void);

/** Display_DrawText()
 * 
 * Displays text at the specified position.
 * 
 * @param text  The null-terminated text to display
 * @param x     The x-coordinate of the text
 * @param y     The y-coordinate of the text
 * @param sizeX The X size of the text
 * @param sizeY The Y size of the text
 * @param fgColor The color of the text
 * @param bgColor The color of the text background
 */
void Display_DrawText(
    const char* text, 
    int x, 
    int y, 
    uint8_t sizeX, 
    uint8_t sizeY,
    uint16_t fgColor = ILI9341_WHITE, 
    uint16_t bgColor = ILI9341_BLACK
);

/** Display_DrawTextTruncated()
 * 
 * Truncates text with "..." if it exceeds maxPixelWidth
 * 
 * @param text  The null-terminated text to display
 * @param x     The x-coordinate of the text
 * @param y     The y-coordinate of the text
 * @param sizeX The X size of the text
 * @param sizeY The Y size of the text
 * @param maxPixelWidth The max pixel width of the area before truncation
 * @param fgColor The color of the text
 * @param bgColor The color of the text background
 */
void Display_DrawTextTruncated(
    const char* text, 
    int x, 
    int y, 
    int sizeX, 
    int sizeY, 
    uint16_t maxPixelWidth, 
    uint16_t fgColor = ILI9341_WHITE, 
    uint16_t bgColor = ILI9341_BLACK
);
                      
/** Display_DrawAlbumArt()
 * 
 * Displays album artwork at the specified position.
 * 
 * @param art   The bitmap data representing the album artwork.
 */
void Display_DrawAlbumArt(const uint16_t *art);

/** Display_FillRect()
 * 
 * Fill rectangle of given size with a specific color. Makes it easier to clear unwanted text. 
 * 
 * @param x      The x-coordinate of the rect
 * @param y      The y-coordinate of the rect
 * @param width  The width of the rect
 * @param height The height of the rect
 * @param color  The color to fill the rect
 */
void Display_FillRect(
    int16_t x, 
    int16_t y, 
    int16_t width, 
    int16_t height, 
    uint16_t color
);

/** Display_DrawPlay()
 * 
 * Draws a filled play triangle at the default controls position.
 * 
 * @param color Color of the icon (default: ILI9341_WHITE)
 */
void Display_DrawPlay(uint16_t color = ILI9341_WHITE);

/** Display_DrawPause()
 * 
 * Draws filled pause rectangles at the default controls position.
 * 
 * @param color Color of the icon (default: ILI9341_WHITE)
 */
void Display_DrawPause(uint16_t color = ILI9341_WHITE);

/** Display_DrawNext()
 * 
 * Draws double right-facing arrows for skipping forward.
 * 
 * @param color Color of the icon (default: ILI9341_WHITE)
 */
void Display_DrawNext(uint16_t color = ILI9341_WHITE);

/** Display_DrawPrev()
 * 
 * Draws double left-facing arrows for skipping backward.
 * 
 * @param color Color of the icon (default: ILI9341_WHITE)
 */
void Display_DrawPrev(uint16_t color = ILI9341_WHITE);

/** Display_DrawProgress()
 * 
 * Draws a track timeline progress bar based on the current player state.
 * 
 * @param elapsedTime   How many seconds the song has been playing for
 * @param totalDuration The total duration of the song
 * @param barColor      The color of the progress bar
 */
void Display_DrawProgress(
    uint16_t elapsedTime, 
    uint16_t totalDuration, 
    uint16_t barColor = ILI9341_CYAN
);


 #endif // DISPLAY_H