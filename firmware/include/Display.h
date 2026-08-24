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
// Standard libraries.
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

/** Display_DrawText(*text, x, y)
 * 
 * This function puts text onto the ILI9341.
 * 
 * @param text  The actual text to be displayed
 * @param x     The x-coordinate of where the text should be displayed
 * @param y     The y-coordinate of where the text should be displayed
 * @return Returns 0 if unable to draw text, else returns 1 for success
 */
int Display_DrawText(const char* text, int x, int y);

/** Display_DrawAlbumArt()
 * 
 * This function displays the album art onto the ILI9341.
 * 
 * @param art   The actual art to be displayed all in binary
 * @param x     The x-coordinate of where the text should be displayed
 * @param y     The y-coordinate of where the text should be displayed
 * @return  Returns 0 if unable to draw text, else returns 1 for success
 */
int Display_DrawAlbumArt(unsigned char art, int x, int y);


 #endif // DISPLAY_H