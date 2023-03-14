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
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <dirent.h>
#include <sys/mount.h>
#include <sys/stat.h>

/* ------------------------------------------------------------------------- */
/*                             Global variables                              */
/* ------------------------------------------------------------------------- */

conf_t configuration;

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

    // Retrieve configuration


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
            printf("USB device connected: %s\n", usb_event.device);

            // Mount device to defined path
            if (mount(usb_event.device, USB_MOUNT_PATH, "vfat", 0, NULL) != 0) {
                perror("mount failed");
                break;
            }

            printf("%s was mounted successfully.\n", usb_event.device);

            // Copy log file to USB device
            char path_to_log[30];
            strcpy(path_to_log, USB_MOUNT_PATH);
            strcat(path_to_log, "/");
            strcat(path_to_log, LOG_FILE_NAME);

            FILE* log_file = fopen(path_to_log, "w");
            if (log_file != NULL) {
                FILE* local_log_file = fopen(PATH_LOG, "r");
                if (local_log_file != NULL) {
                    char c;
                    while ((c = fgetc(local_log_file)) != EOF)
                        fputc(c, log_file);
                    fclose(local_log_file);
                }
                else perror("Can't open local log file\n");
                fclose(log_file);
            }
            else perror("Can't open log file\n");
            fclose(log_file);

            // If there is a configuration file, copy it locally
            char path_to_config[30];
            strcpy(path_to_config, USB_MOUNT_PATH);
            strcat(path_to_config, "/");
            strcat(path_to_config, CONFIG_FILE_NAME);

            if (access(path_to_config, F_OK) == 0) {
                // Retrieve configuration from USB device
                printf("Configuration file found in USB device.\n");
                bool ok;
                conf_t conf;

                ok = read_configuration(&conf, path_to_config);
                if (!ok) {
                    printf("Error while reading configuration file from USB device.\n");
                    break;
                }

                // Write configuration locally
                ok = write_configuration(&conf, PATH_CONFIG);
                if (!ok) {
                    printf("Error while writing configuration file locally.\n");
                    break;
                }

                printf("Configuration file copied locally successfully.\n");

                // Retrieve local configuration
                ok = read_configuration(&configuration, PATH_CONFIG);

                if (!ok) {
                    printf("Error while reading configuration file locally.\n");
                    break;
                }

                printf("Configuration updated.\n");
            }
            break;
        case USB_EVENT_DISCONNECT:
            printf("USB device disconnected: %s\n", usb_event.device);
            break;
        }
    }

    pthread_exit(NULL);
}
