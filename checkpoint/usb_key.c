/**
 * File containing the functions to handle the USB key.
 * @file usb_key.c
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#include "usb_key.h"

#include <stdio.h>
#include <string.h>
#include <libudev.h>

/* ------------------------------------------------------------------------- */
/*                                 Functions                                 */
/* ------------------------------------------------------------------------- */

void wait_usb_connection(void)
{
struct udev *udev;
    struct udev_monitor *mon;
    int fd;
    fd_set fds;
    struct udev_device *dev;

    // create a udev object and monitor for USB device events
    udev = udev_new();
    if (!udev) {
        fprintf(stderr, "Failed to create udev object\n");
        return;
    }
    mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(mon, "block", "disk");
    udev_monitor_enable_receiving(mon);
    fd = udev_monitor_get_fd(mon);

    while (1) {
        // wait for a USB device event
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        if (select(fd+1, &fds, NULL, NULL, NULL) <= 0) {
            fprintf(stderr, "Failed to select() on udev monitor fd\n");
            return;
        }

        // get the device that triggered the event
        dev = udev_monitor_receive_device(mon);
        if (dev) {
            // check if the device is a USB key that has just been mounted
            if (udev_device_get_property_value(dev, "ID_BUS") &&
                strcmp(udev_device_get_property_value(dev, "ID_BUS"), "usb") == 0 &&
                udev_device_get_property_value(dev, "ID_FS_TYPE") &&
                strcmp(udev_device_get_property_value(dev, "ID_FS_TYPE"), "vfat") == 0 &&
                udev_device_get_property_value(dev, "UDISKS_PARTITION_MOUNT_FLAGS") &&
                strcmp(udev_device_get_property_value(dev, "UDISKS_PARTITION_MOUNT_FLAGS"), "auto") == 0) {
                printf("USB key mounted at %s\n", udev_device_get_property_value(dev, "UDISKS_PARTITION_MOUNT_POINT"));
            }

            udev_device_unref(dev);
        }
    }

    // clean up
    udev_monitor_unref(mon);
    udev_unref(udev);
}
