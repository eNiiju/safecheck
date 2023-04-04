/**
 * File containing the functions to interact with the OLED display.
 * @file display.c
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */


#include <stdio.h>
#include "display.h"

/* ------------------------------------------------------------------------- */
/*                                 Functions                                 */
/* ------------------------------------------------------------------------- */

int write_string_to_display(ssd1306_i2c_t* p_display, ssd1306_framebuffer_t* p_framebuffer, char* string, int x, int y, int size){
    ssd1306_framebuffer_box_t bbox;
    char s[(MAX_DISPLAY_CHARS_LINE * 3) + 3];
    char* token;
    int i = 0;

    strncpy(s, string, (MAX_DISPLAY_CHARS_LINE * 3) + 3);
    ssd1306_framebuffer_clear(p_framebuffer);
    token = strtok(s, "\n");

    /*We can only display on 3 lines by default*/
    while(token != NULL && i < 3){
        ssd1306_framebuffer_draw_text(p_framebuffer, token, sizeof(char)*strlen(token), x, y+(i*21), SSD1306_FONT_DEFAULT, size, &bbox);
        token = strtok(NULL, "\n");
        i++;
    }
    ssd1306_i2c_display_update(p_display, p_framebuffer);
    sleep(1);
    return 0;
}

int write_tab_to_display_scroll(ssd1306_i2c_t* p_display, ssd1306_framebuffer_t* p_framebuffer, display_tab_t tab, int nb_lines){
    int cpt=0;
    char s[(MAX_DISPLAY_CHARS_LINE * 3) + 3];

    ssd1306_framebuffer_clear(p_framebuffer);

    while (s[0] != '\0' && cpt < nb_lines)
    {
        strcpy(s, "\0");
        strcat(s, tab[cpt]);
        strcat(s, "\n");
        strcat(s, tab[cpt+1]);
        strcat(s, "\n");
        strcat(s, tab[cpt+2]);
        write_string_to_display(p_display, p_framebuffer, s, DISPLAY_X_OFFSET, DISPLAY_Y_OFFSET, DEFAULT_FONTSIZE);
        sleep(5);
        cpt=cpt+3;
    }
    
    ssd1306_framebuffer_clear(p_framebuffer);
    return 0;
}

int write_error_to_display(ssd1306_i2c_t* p_display, ssd1306_framebuffer_t* p_framebuffer){
    ssd1306_framebuffer_box_t bbox;

    ssd1306_framebuffer_clear(p_framebuffer);
    ssd1306_framebuffer_draw_text(p_framebuffer, "X", sizeof(char)*strlen("X"), 46, 46, SSD1306_FONT_DEFAULT, 12, &bbox);
    ssd1306_i2c_display_update(p_display, p_framebuffer);
    ssd1306_framebuffer_clear(p_framebuffer);

    return 0;
}

int write_valid_to_display(ssd1306_i2c_t* p_display, ssd1306_framebuffer_t* p_framebuffer){
    ssd1306_framebuffer_box_t bbox;

    ssd1306_framebuffer_clear(p_framebuffer);
    ssd1306_framebuffer_draw_text(p_framebuffer, "OK", sizeof(char)*strlen("OK"), 32, 46, SSD1306_FONT_DEFAULT, 10, &bbox);
    ssd1306_i2c_display_update(p_display, p_framebuffer);
    ssd1306_framebuffer_clear(p_framebuffer);
    return 0;
}