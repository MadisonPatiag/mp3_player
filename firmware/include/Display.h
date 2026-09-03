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
 * Displays text at the specified position.
 * 
 * @param text  The null-terminated text to display
 * @param x     The x-coordinate of the text
 * @param y     The y-coordinate of the text
 */
void Display_DrawText(const char* text, int x, int y);

/** Display_DrawAlbumArt()
 * 
 * Displays album artwork at the specified position.
 * 
 * @param art   The bitmap data representing the album artwork.
 */
void Display_DrawAlbumArt(const uint16_t *art);


 #endif // DISPLAY_H