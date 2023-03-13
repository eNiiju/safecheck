/**
 * File containing the functions to handle the USB key.
 * @file usb_key.c
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#include "usb_key.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libudev.h>
#include <sys/mount.h>

/* ------------------------------------------------------------------------- */
/*                             Global variables                              */
/* ------------------------------------------------------------------------- */

char devices[MAX_DEVICES][MAX_DEVICE_NAME_LENGTH + 1];
int nb_devices;

/* ------------------------------------------------------------------------- */
/*                                 Functions                                 */
/* ------------------------------------------------------------------------- */

bool wait_usb_event(usb_event_t* usb_event)
{
    struct udev* udev = udev_new();

    if (!udev) {
        printf("Failed to create udev context.\n");
        return false;
    }

    struct udev_monitor* mon = udev_monitor_new_from_netlink(udev, "udev");

    if (!mon) {
        printf("Failed to create udev monitor.\n");
        udev_unref(udev);
        return false;
    }

    udev_monitor_filter_add_match_subsystem_devtype(mon, "usb", "usb_device");
    udev_monitor_enable_receiving(mon);

    while (1) {
        // Retrieve current device list, for future comparison
        nb_devices = retrieve_usb_devices(devices);

        // Wait for device event
        struct udev_device* dev = udev_monitor_receive_device(mon);

        if (dev) {
            const char* action = udev_device_get_action(dev);

            if (action && !strcmp(action, "add")) {
                // Device connection
                char device[MAX_DEVICE_NAME_LENGTH + 1];
                bool found = false;
                for (int i = 0; i < MAX_TRIES_FIND_DEVICE && !found; i++) {
                    sleep(1);
                    found = find_added_usb_device(device);
                }

                if (!found) continue;

                // Found the added usb device !
                usb_event->event = USB_EVENT_CONNECT;
                strncpy(usb_event->device, device, MAX_DEVICE_NAME_LENGTH);
                return true;
            }
            else if (action && !strcmp(action, "remove")) {
                // Device disconnection
                char device[MAX_DEVICE_NAME_LENGTH + 1];
                bool found = false;
                for (int i = 0; i < MAX_TRIES_FIND_DEVICE && !found; i++) {
                    sleep(1);
                    found = find_removed_usb_device(device);
                }

                if (!found) continue;

                // Found the removed usb device !
                usb_event->event = USB_EVENT_DISCONNECT;
                strncpy(usb_event->device, device, MAX_DEVICE_NAME_LENGTH);
                return true;
            }

            udev_device_unref(dev);
        }
    }

    udev_monitor_unref(mon);
    udev_unref(udev);
    return false;
}

bool find_added_usb_device(char* device)
{
    char new_devices[MAX_DEVICES][MAX_DEVICE_NAME_LENGTH + 1];
    int nb_new_devices = retrieve_usb_devices(new_devices);

    // Same number of devices (or less)
    if (nb_new_devices <= nb_devices) return false;

    // A device has been added to the list. Find it
    bool same_exists = true;
    for (int i = 0; i < nb_new_devices && same_exists; i++) {
        same_exists = false;

        for (int j = 0; j < nb_devices && !same_exists; j++) {
            if (strcmp(new_devices[i], devices[j]) != 0) continue;
            same_exists = true;
        }

        if (same_exists) continue;

        // Found it!
        strncpy(device, new_devices[i], MAX_DEVICE_NAME_LENGTH);
        return true;
    }

    return false;
}

bool find_removed_usb_device(char* device)
{
    char new_devices[MAX_DEVICES][MAX_DEVICE_NAME_LENGTH + 1];
    int nb_new_devices = retrieve_usb_devices(new_devices);

    // Same number of devices (or more)
    if (nb_new_devices >= nb_devices) return false;

    // A device has been removed from the list. Find it
    bool same_exists = true;
    for (int i = 0; i < nb_devices && same_exists; i++) {
        same_exists = false;

        for (int j = 0; j < nb_new_devices && !same_exists; j++) {
            if (strcmp(new_devices[i], devices[j]) != 0) continue;
            same_exists = true;
        }

        if (same_exists) continue;

        // Found it!
        strncpy(device, new_devices[i], MAX_DEVICE_NAME_LENGTH);
        return true;
    }

    return false;
}

int retrieve_usb_devices(char devices[][MAX_DEVICE_NAME_LENGTH + 1])
{
    char output[1000];

    // Run fdisk command, only show lines starting with /dev/sd
    // so that we get the list of usb devices, and capture output
    FILE* fp = popen("fdisk -l | grep ^/dev/sd", "r");
    if (fp == NULL) {
        printf("Failed to run fdisk command\n");
        return 0;
    }

    // Read the output of the command line by line
    int i = 0;
    while (fgets(output, sizeof(output), fp) != NULL) {
        char* device = strtok(output, " ");
        strncpy(devices[i], device, MAX_DEVICE_NAME_LENGTH);
        i++;
    }

    if (ferror(fp)) printf("Error reading output of fdisk command\n");

    pclose(fp); // Close the pipe and exit
    return i;
}
