#include "../src/display.h"

int main(void){
    ssd1306_framebuffer_box_t bbox;
    ssd1306_i2c_t* p_display;
    ssd1306_framebuffer_t* p_framebuffer;
    display_tab_t tab={"jean", "jacques", "goldman", "anyway", "jsplus", "aled", "j'ai", "pas", "de", "phrase", "drole", "a", "ecrir"};

    //************************init**************************//
    //I previously made an init func but the library doesn't like me using 
    //passing references by address so I just put the code here
    p_display = ssd1306_i2c_open(BUS_ADDRESS, DEVICE_ADDRESS, DISPLAY_WIDTH, DISPLAY_HEIGHT, NULL);
    if(!p_display){
        printf("Error: Could not open display\n");
        return -1;
    }
    if(ssd1306_i2c_display_initialize(p_display) < 0){
        printf("Error: Could not initialize display\n");
        ssd1306_i2c_close(p_display);
        return -1;
    }
    sleep(3);
    p_framebuffer=ssd1306_framebuffer_create(p_display->width, p_display->height, p_display->err);
    //**********************end init**************************//

    //write_string_to_display(p_display, p_framebuffer, "Hello World!\npd AHAHaAHHAAAahahaaahahanoe\nmat BG", 4, 16, 4);
    //write_tab_to_display_scroll(p_display, p_framebuffer, tab, 14);
    write_valid_to_display(p_display, p_framebuffer);
}