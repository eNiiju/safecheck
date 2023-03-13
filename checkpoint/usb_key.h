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
#define MOUNT_PATH "/mnt/safecheck"
#define USB_EVENT_CONNECT 1
#define USB_EVENT_DISCONNECT 2

/* ------------------------------------------------------------------------- */
/*                             Type definitions                              */
/* ------------------------------------------------------------------------- */

struct usb_event {
    int event; // USB_EVENT_CONNECT || USB_EVENT_CONNECT
    char device[MAX_DEVICE_NAME_LENGTH + 1];
};

typedef struct usb_event usb_event_t;

/* ------------------------------------------------------------------------- */
/*                            Function prototypes                            */
/* ------------------------------------------------------------------------- */

bool wait_usb_event(usb_event_t* usb_event);

bool find_added_usb_device(char* device);

bool find_removed_usb_device(char* device);

int retrieve_usb_devices(char devices[][MAX_DEVICE_NAME_LENGTH + 1]);

#endif
