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
#include <errno.h>
#include <dirent.h>
#include <sys/mount.h>
#include <sys/stat.h>

/* ------------------------------------------------------------------------- */
/*                               Main function                               */
/* ------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
    pthread_t th_display, th_button, th_rfid, th_send_data, th_usb_key;

    if (!init()) return -1;

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

bool init(void)
{
    // Create usb mount path directory if it doesn't exist
    opendir(USB_MOUNT_PATH);
    if (errno == ENOENT && mkdir(USB_MOUNT_PATH, 0777) != 0) {
        perror("Can't create usb mount path directory");
        return false;
    }

    return true;
}

void* display_routine(void* arg)
{
    pthread_exit(NULL);
}

void* button_routine(void* arg)
{
    pthread_exit(NULL);
}

void* rfid_routine(void* arg)
{
    pthread_exit(NULL);
}

void* send_data_routine(void* arg)
{
    pthread_exit(NULL);
}

void* usb_key_routine(void* arg)
{
    usb_event_t usb_event;

    while (1) {
        if (!wait_usb_event(&usb_event)) continue;

        // A usb device event has occured
        switch (usb_event.event) {
        case USB_EVENT_CONNECT:
            printf("Device connected: %s\n", usb_event.device);

            // Mount device to defined path
            if (mount(usb_event.device, USB_MOUNT_PATH, "vfat", 0, NULL) != 0) {
                perror("mount failed");
                break;
            }

            printf("%s was mounted successfully.\n", usb_event.device);
            break;
        case USB_EVENT_DISCONNECT:
            printf("Device disconnected: %s\n", usb_event.device);
            break;
        }
    }

    pthread_exit(NULL);
}
