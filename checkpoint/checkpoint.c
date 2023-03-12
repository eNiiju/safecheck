/**
 * Main file for the checkpoint's program.
 * @file checkpoint.c
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#include "checkpoint.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* ------------------------------------------------------------------------- */
/*                               Main function                               */
/* ------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
    pthread_t th_display, th_button, th_rfid, th_send_data, th_usb_key;

    // Create threads
    pthread_create(&th_display, NULL, display_routine, NULL);
    pthread_create(&th_button, NULL, button_routine, NULL);
    pthread_create(&th_rfid, NULL, rfid_routine, NULL);
    pthread_create(&th_send_data, NULL, send_data_routine, NULL);
    pthread_create(&th_usb_key, NULL, usb_key_routine, NULL);

    // Wait for threads to finish
    pthread_join(th_display, NULL);
    pthread_join(th_button, NULL);
    pthread_join(th_rfid, NULL);
    pthread_join(th_send_data, NULL);
    pthread_join(th_usb_key, NULL);

    return 0;
}

/* ------------------------------------------------------------------------- */
/*                                 Functions                                 */
/* ------------------------------------------------------------------------- */

void* display_routine(void* arg)
{
    printf("Display routine\n");

    pthread_exit(NULL);
}

void* button_routine(void* arg)
{
    printf("Button routine\n");

    pthread_exit(NULL);
}

void* rfid_routine(void* arg)
{
    printf("RFID routine\n");

    pthread_exit(NULL);
}

void* send_data_routine(void* arg)
{
    printf("Send data routine\n");

    pthread_exit(NULL);
}

void* usb_key_routine(void* arg)
{
    printf("USB key routine\n");

    pthread_exit(NULL);
}
