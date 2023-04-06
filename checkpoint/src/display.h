/**
 * Header for the file containing the functions to interact with the OLED display.
 * @file display.h
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "../lib/libssd1306/include/ssd1306_i2c.h"

/* ------------------------------------------------------------------------- */
/*                          Constants & Definitions                          */
/* ------------------------------------------------------------------------- */

#define DISPLAY_I2C_BUS_ADDRESS "/dev/i2c-1"
#define DISPLAY_I2C_DEVICE_ADDRESS 0x3c
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_X_OFFSET 4
#define DISPLAY_Y_OFFSET 16
#define DEFAULT_FONTSIZE 4
#define DISPLAY_MAX_LINE_CHAR 32
#define DISPLAY_MAX_LINES 30
#define DISPLAY_MAX_LINES_VISIBLE 3
#define DISPLAY_MAX_STRING_LENGTH (DISPLAY_MAX_LINE_CHAR * DISPLAY_MAX_LINES_VISIBLE) + DISPLAY_MAX_LINES_VISIBLE // 3 lines + '\n'

/* ------------------------------------------------------------------------- */
/*                             Type definitions                              */
/* ------------------------------------------------------------------------- */

/**
 * Struct representing a multiple lines message to display.
*/
struct display_message {
    int length;
    char lines[DISPLAY_MAX_LINES][DISPLAY_MAX_LINE_CHAR];
};

typedef struct display_message display_message_t;

/* ------------------------------------------------------------------------- */
/*                            Function prototypes                            */
/* ------------------------------------------------------------------------- */

/**
 * Display a string. Please use default fontsizes and use
 * the OFFSETS to position the text. To write on multiple lines,
 * use the newline ('\n') character.
 * @param p_display Pointer to the display
 * @param p_framebuffer Pointer to the framebuffer
 * @param string String to write
 * @param x X starting position of the string
 * @param y Y starting position of the string
 * @param font_size Font size
*/
void display_string(ssd1306_i2c_t* p_display, ssd1306_framebuffer_t* p_framebuffer, char string[DISPLAY_MAX_STRING_LENGTH], int x, int y, int font_size);

/**
 * Display a multi-line message, 3 lines at a time.
 * @param p_display Pointer to the display
 * @param p_framebuffer Pointer to the framebuffer
 * @param msg The message to display
 * @param nb_lines Number of lines to display
*/
void display_message_array(ssd1306_i2c_t* p_display, ssd1306_framebuffer_t* p_framebuffer, display_message_t msg, int nb_lines);

/**
 * Display a cross to indicate an error.
 * @param p_display Pointer to the display
 * @param p_framebuffer Pointer to the framebuffer
*/
void display_error(ssd1306_i2c_t* p_display, ssd1306_framebuffer_t* p_framebuffer);

/**
 * Display "OK" to indicate a success.
 * @param p_display Pointer to the display
 * @param p_framebuffer Pointer to the framebuffer
*/
void display_ok(ssd1306_i2c_t* p_display, ssd1306_framebuffer_t* p_framebuffer);

#endif
