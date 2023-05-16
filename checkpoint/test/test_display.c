/**
 * @author Noe Maillet & Mathéo Mercier
 * @file test_display.c
 * This file is made to test the functions of display.c
*/

#include "../src/display.h"

#include <stdbool.h>

ssd1306_i2c_t* p_display;
ssd1306_framebuffer_t* p_framebuffer;

bool init_display();

int main(void)
{
    struct display_message message = {
        .length = 13,
        .lines = { "jean", "jacques", "goldman", "anyway", "jsplus", "aled", "j'ai", "pas", "de", "phrase", "drole", "a", "ecrir" }
    };

    if (!init_display()) return -1;

    display_string(p_display, p_framebuffer, "Hello World!\nAHAHaAHHAAAahahaaahahanoe\nmat BG", 4, 16, 4);
    sleep(5);

    display_message_array(p_display, p_framebuffer, message, message.length);
    sleep(5);

    display_ok(p_display, p_framebuffer, false);
    sleep(5);

    display_error(p_display, p_framebuffer, false);
    sleep(5);

    display_clear(p_display, p_framebuffer);

    return 0;
}

bool init_display()
{
    p_display = ssd1306_i2c_open(DISPLAY_I2C_BUS_ADDRESS, DISPLAY_I2C_DEVICE_ADDRESS, DISPLAY_WIDTH, DISPLAY_HEIGHT, NULL);
    if (!p_display) {
        printf("Error: Could not open display\n");
        return false;
    }
    if (ssd1306_i2c_display_initialize(p_display) < 0) {
        printf("Error: Could not initialize display\n");
        ssd1306_i2c_close(p_display);
        return false;
    }
    sleep(3);
    p_framebuffer = ssd1306_framebuffer_create(p_display->width, p_display->height, p_display->err);
}
