/**
 * File containing the functions to interact with the OLED display.
 * @file display.c
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#include <string.h>

#include "display.h"

/* ------------------------------------------------------------------------- */
/*                                 Functions                                 */
/* ------------------------------------------------------------------------- */

void display_string(ssd1306_i2c_t* p_display, ssd1306_framebuffer_t* p_framebuffer, char* string, int x, int y, int font_size)
{
    ssd1306_framebuffer_box_t bbox;
    char* token;
    char _string[(DISPLAY_MAX_LINE_CHAR * DISPLAY_MAX_LINES_VISIBLE) + DISPLAY_MAX_LINES_VISIBLE];

    // Copy input string
    strncpy(_string, string, (DISPLAY_MAX_LINE_CHAR * 3) + 3);

    ssd1306_framebuffer_clear(p_framebuffer);

    // Display each line
    token = strtok(_string, "\n");
    for (int i = 0; token != NULL && i < DISPLAY_MAX_LINES_VISIBLE; i++) {
        ssd1306_framebuffer_draw_text(p_framebuffer, token, sizeof(char) * strlen(token), x, y + (i * (DISPLAY_HEIGHT / 3)), SSD1306_FONT_DEFAULT, font_size, &bbox);
        token = strtok(NULL, "\n");
    }
    ssd1306_i2c_display_update(p_display, p_framebuffer);

    sleep(1);
}

void display_message_array(ssd1306_i2c_t* p_display, ssd1306_framebuffer_t* p_framebuffer, display_message_t msg, int nb_lines)
{
    int cpt = 0;
    char s[(DISPLAY_MAX_LINE_CHAR * 3) + 3];

    ssd1306_framebuffer_clear(p_framebuffer);

    while (s[0] != '\0' && cpt < nb_lines) {
        strcpy(s, "\0");
        strcat(s, msg.lines[cpt]);
        strcat(s, "\n");
        strcat(s, msg.lines[cpt + 1]);
        strcat(s, "\n");
        strcat(s, msg.lines[cpt + 2]);
        display_string(p_display, p_framebuffer, s, DISPLAY_X_OFFSET, DISPLAY_Y_OFFSET, DEFAULT_FONTSIZE);
        sleep(5);
        cpt = cpt + 3;
    }

    ssd1306_framebuffer_clear(p_framebuffer);
}

void display_error(ssd1306_i2c_t* p_display, ssd1306_framebuffer_t* p_framebuffer)
{
    ssd1306_framebuffer_box_t bbox;

    ssd1306_framebuffer_clear(p_framebuffer);
    ssd1306_framebuffer_draw_text(p_framebuffer, "X", sizeof(char) * strlen("X"), 46, 46, SSD1306_FONT_DEFAULT, 12, &bbox);
    ssd1306_i2c_display_update(p_display, p_framebuffer);
    ssd1306_framebuffer_clear(p_framebuffer);
}

void display_ok(ssd1306_i2c_t* p_display, ssd1306_framebuffer_t* p_framebuffer)
{
    ssd1306_framebuffer_box_t bbox;

    ssd1306_framebuffer_clear(p_framebuffer);
    ssd1306_framebuffer_draw_text(p_framebuffer, "OK", sizeof(char) * strlen("OK"), 32, 46, SSD1306_FONT_DEFAULT, 10, &bbox);
    ssd1306_i2c_display_update(p_display, p_framebuffer);
    ssd1306_framebuffer_clear(p_framebuffer);
}
