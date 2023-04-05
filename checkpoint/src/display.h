/**
 * Header for the file containing the functions to interact with the OLED display.
 * @file display.h
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#ifndef DISPLAY_H
#define DISPLAY_H

/****************************************** includes ******************************************/
#include "../lib/libssd1306/include/ssd1306_i2c.h"
#include <string.h>


/****************************************** defines ******************************************/
#define BUS_ADDRESS "/dev/i2c-1" // I2C bus address
#define DEVICE_ADDRESS 0x3c // Device address on the I2C bus
#define DISPLAY_WIDTH 128 // Display width in pixels
#define DISPLAY_HEIGHT 64 // Display height in pixels
#define DISPLAY_X_OFFSET 4 // Display X offset in pixels
#define DISPLAY_Y_OFFSET 16 // Display Y offset in pixels
#define DEFAULT_FONTSIZE 4 // Default font size
#define MAX_DISPLAY_CHARS_LINE 32 // Max number of characters that can be displayed on the display
#define MAX_DISPLAY_LINES 3 // Max number of lines that can be displayed on the display
#define MAX_DISPLAY_TAB_LINES 30 // Max number of lines that can be displayed on the display

/****************************************** typedefs ******************************************/
typedef char display_tab_t[MAX_DISPLAY_TAB_LINES][MAX_DISPLAY_CHARS_LINE];

/****************************************** prototypes ******************************************/

/**
 * @brief Write a string to the display. please use default fontsizes and use
 * the OFFSETS to position the text. To write on multiple lines, use
 * the \n character.
 * @param p_display Pointer to the display.
 * @param p_framebuffer Pointer to the framebuffer.
 * @param string String to write.
 * @param x X starting position of the string.
 * @param y Y starting position of the string.
 * @param size fontsize
 * @param mode 0 for normal, 1 for 2 lines, 3 for 3 lines, 4 for marquee text
*/
int write_string_to_display(ssd1306_i2c_t* p_display, ssd1306_framebuffer_t* p_framebuffer, char* string, int x, int y, int size);

/**
 * @brief Write a tab to the display. it will display 3 lines of text. then scroll to the next 3
 * @param p_display Pointer to the display.
 * @param p_framebuffer Pointer to the framebuffer.
 * @param tab the tab to write to the display (3 lines of text at a time and a total of 30 lines max)
*/
int write_tab_to_display_scroll(ssd1306_i2c_t* p_display, ssd1306_framebuffer_t* p_framebuffer, display_tab_t tab, int nb_lines);

int write_error_to_display(ssd1306_i2c_t* p_display, ssd1306_framebuffer_t* p_framebuffer);

int write_valid_to_display(ssd1306_i2c_t* p_display, ssd1306_framebuffer_t* p_framebuffer);

#endif
