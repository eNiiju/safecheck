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
#include <string.h>
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

    if (!init()) exit(EXIT_FAILURE);

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

    exit(EXIT_SUCCESS);
}

/* ------------------------------------------------------------------------- */
/*                         Thread routine functions                          */
/* ------------------------------------------------------------------------- */

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

            // Copy local log file to USB device
            copy_log_file_to_usb();

            // Copy configuration file from USB device locally
            copy_config_file_from_usb();

            // Unmount device
            if (umount(USB_MOUNT_PATH) != 0) {
                perror("umount failed");
                break;
            }
            printf("%s was unmounted successfully.\n", usb_event.device);
            break;
        case USB_EVENT_DISCONNECT:
            printf("USB device disconnected: %s\n", usb_event.device);
            break;
        }
    }

    pthread_exit(NULL);
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
    printf("USB mount path OK.\n");

    // Create log directory if it doesn't exist (and log file)
    opendir(LOG_DIRECTORY);
    if (errno == ENOENT && mkdir(LOG_DIRECTORY, 0777) != 0) {
        perror("Can't create log directory");
        return false;
    }
    printf("Log directory OK.\n");

    // Create log file if it doesn't exist
    FILE* log_file = fopen(LOG_FILE_PATH, "a");
    if (log_file == NULL) {
        perror("Can't create log file");
        return false;
    }
    fclose(log_file);
    printf("Log file OK.\n");

    // Retrieve configuration
    if (!read_configuration(&configuration, CONFIG_FILE_PATH)) {
        printf("Can't read configuration file\n");
        return false;
    }
    printf("Configuration OK.\n");

    return true;
}

void copy_log_file_to_usb()
{
    // Build path string
    char path_to_log[30];
    strcpy(path_to_log, USB_MOUNT_PATH);
    strcat(path_to_log, LOG_FILE_NAME);

    FILE* log_file = fopen(path_to_log, "w");
    if (log_file == NULL) {
        perror("Can't open log file\n");
        return;
    }

    FILE* local_log_file = fopen(LOG_FILE_PATH, "r");
    if (local_log_file == NULL) {
        perror("Can't open local log file\n");
        return;
    }

    // Read local log file line by line and write it to the USB device
    char line[MAX_LENGTH_LOG_LINE];
    while (fgets(line, sizeof(line), local_log_file) != NULL)
        fputs(line, log_file);
    printf("Log file copied to USB device.\n");

    fclose(local_log_file);
    fclose(log_file);
}

void copy_config_file_from_usb()
{
    conf_t conf;
    bool ok;

    // Build path string
    char path_to_config[30];
    strcpy(path_to_config, USB_MOUNT_PATH);
    strcat(path_to_config, CONFIG_FILE_NAME);

    if (access(path_to_config, F_OK) != 0) {
        printf("No configuration file found in USB device.\n");
        return;
    }
    printf("Configuration file found in USB device.\n");

    // Retrieve configuration from USB device
    ok = read_configuration(&conf, path_to_config);
    if (!ok) {
        printf("Error while reading configuration file from USB device.\n");
        return;
    }

    // Write configuration locally
    ok = write_configuration(&conf, CONFIG_FILE_PATH);
    if (!ok) {
        printf("Error while writing configuration file locally.\n");
        return;
    }

    printf("Configuration file copied locally successfully.\n");

    // Retrieve local configuration
    ok = read_configuration(&configuration, CONFIG_FILE_PATH);
    if (!ok) {
        printf("Error while reading configuration file locally.\n");
        return;
    }

    printf("Configuration updated.\n");
}