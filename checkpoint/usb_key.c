/**
 * File containing the functions to handle the USB key.
 * @file usb_key.c
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#include "usb_key.h"

#include <stdio.h>
#include <libudev.h>

/* ------------------------------------------------------------------------- */
/*                                 Functions                                 */
/* ------------------------------------------------------------------------- */

void wait_usb_connection(void)
{
    struct udev* udev = udev_new();
    if (!udev) {
        printf("Failed to create udev\n");
        return;
    }

    struct udev_monitor* mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(mon, "usb", NULL);
    udev_monitor_enable_receiving(mon);

    printf("Waiting for USB device...\n");

    while (1) {
        struct udev_device* dev = udev_monitor_receive_device(mon);
        if (dev) {
            const char* devnode = udev_device_get_devnode(dev);
            if (devnode) {
                printf("USB device connected at %s\n", devnode);
                break;
            }
            udev_device_unref(dev);
        }
    }

    udev_monitor_unref(mon);
    udev_unref(udev);
}
