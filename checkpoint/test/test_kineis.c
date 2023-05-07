/**
 @author Noe Maillet & Mathéo Mercier
 @file test_kineis.c
 * This file is made to test the functions of kineis.c
*/

#include "../src/kineis.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    int fd; //this will be our COM port descriptor
    char data[MAX_KINEIS_DATA_SIZE];
    char response[MAX_KINEIS_DATA_SIZE];
    int res;

    fd = kineis_init();

    if (fd < 0) {
        printf("Error opening serial port\n");
        return -1;
    }

    kineis_set_band(fd, BAND);
    kineis_read_data(fd, response);
    kineis_set_power(fd, POWER);
    kineis_read_data(fd, response);

    if (fd < 0) {
        printf("Error opening serial port\n");
        return -1;
    }

    strcpy(data, "F00100000000000000000000000000000000000");
    for (int i = 0; i < 10; i++) {
        kineis_send_data(fd, data, strlen(data));
        kineis_read_data(fd, response);
    }

    kineis_close(fd);
    return 0;
}
