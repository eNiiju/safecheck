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
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fs.h>

/* ------------------------------------------------------------------------- */
/*                             Global variables                              */
/* ------------------------------------------------------------------------- */

// Variables to store the device list (and length) for comparison when we add/remove a device
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

    // Retrieve current device list, for future comparison
    nb_devices = retrieve_usb_devices(devices);

    while (1) {
        // See if device event has occured
        struct udev_device* dev = udev_monitor_receive_device(mon);

        if (!dev) continue;

        const char* action = udev_device_get_action(dev);

        // Device connection event
        if (action && !strcmp(action, "add")) {
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
        // Device disconnection event
        else if (action && !strcmp(action, "remove")) {
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
        strncpy(device, devices[i], MAX_DEVICE_NAME_LENGTH);
        return true;
    }

    return false;
}

int retrieve_usb_devices(char devices[][MAX_DEVICE_NAME_LENGTH + 1])
{
    char device_name[MAX_DEVICE_NAME_LENGTH];
    int nb_devices = 0;
    int fd;

    for (int j = 0; j < MAX_DEVICES; j++) {
        snprintf(device_name, MAX_DEVICE_NAME_LENGTH, "/dev/sd%c", 'a' + j);
        fd = open(device_name, O_RDONLY | O_NONBLOCK);

        if (fd >= 0) {
            // Check if the device is a USB device by checking its block size
            int block_size;
            if (ioctl(fd, BLKSSZGET, &block_size) == 0 && block_size == 512) {
                // It's a USB device ! Add it to the list
                strncpy(devices[nb_devices], device_name, MAX_DEVICE_NAME_LENGTH);
                nb_devices++;
            }

            close(fd);
        }
    }

    return nb_devices;
}
