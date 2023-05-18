/**
 * Header for the file containing the functions to handle the USB key.
 * @file usb_key.h
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#ifndef USB_KEY_H
#define USB_KEY_H

#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/*                          Constants & Definitions                          */
/* ------------------------------------------------------------------------- */

#define MAX_DEVICES 10
#define MAX_DEVICE_NAME_LENGTH 10
#define MAX_TRIES_FIND_DEVICE 10
#define USB_MOUNT_PATH "/mnt/safecheck/"
#define USB_EVENT_CONNECT 1
#define USB_EVENT_DISCONNECT 2

/* ------------------------------------------------------------------------- */
/*                             Type definitions                              */
/* ------------------------------------------------------------------------- */

/**
 * Structure representing a USB event, including the type of event (connection
 * or disconnection of the USB drive) and the name of the device (example: /dev/sda1)
*/
struct usb_event {
    int event; // USB_EVENT_CONNECT || USB_EVENT_CONNECT
    char device[MAX_DEVICE_NAME_LENGTH + 1];
};

typedef struct usb_event usb_event_t;

/* ------------------------------------------------------------------------- */
/*                            Function prototypes                            */
/* ------------------------------------------------------------------------- */

/**
 * Waits for a USB event (connection or disconnection of the USB drive), find the device
 * that was added or removed, and fill the usb_event structure with the event type and
 * the device name, then returns.
 * @param usb_event Pointer to the usb_event_t structure to fill.
 * @return True if everything went well (usb_event structure is readable), false otherwise.
*/
bool wait_usb_event(usb_event_t* usb_event);

/**
 * Find which USB device was added.
 * @param device Pointer to the char array to fill with the device name.
 * @return True if a device was found, false otherwise.
*/
bool find_added_usb_device(char* device);

/**
 * Find which USB device was removed.
 * @param device Pointer to the char array to fill with the device name.
 * @return True if a device was found, false otherwise.
*/
bool find_removed_usb_device(char* device);

/**
 * Retrieve the list of USB devices currently connected (calling fdisk -l).
 * @param devices Pointer to the array to fill with the device names.
 * @return The number of devices found.
*/
int retrieve_usb_devices(char devices[][MAX_DEVICE_NAME_LENGTH + 1]);

#endif
