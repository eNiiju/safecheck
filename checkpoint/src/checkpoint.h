/**
 * Header of the main file for the checkpoint's program.
 * @file checkpoint.h
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#include "display.h"
#include "kineis.h"
#include "log.h"
#include "rfid.h"
#include "usb_key.h"

#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/*                            Function prototypes                            */
/* ------------------------------------------------------------------------- */

/**
 * Function for the display thread.
 * It is responsible for displaying informations on the screen.
 * @param arg Unused.
*/
void* display_routine(void* arg);

/**
 * Function for the button thread.
 * It is responsible for handling the button interrupts.
 * @param arg Unused.
*/
void* button_routine(void* arg);

/**
 * Function for the RFID thread.
 * It is responsible for handling the RFID interrupts.
 * @param arg Unused.
*/
void* rfid_routine(void* arg);

/**
 * Function for the send data thread.
 * It is responsible for sending data to the Kinéis satellites.
 * @param arg Unused.
*/
void* send_data_routine(void* arg);

/**
 * Function for the USB key thread.
 * It is responsible for handling the USB key interactions.
 * @param arg Unused.
*/
void* usb_key_routine(void* arg);

/**
 * Initialization function. Prints error if they happen.
 * @return True if initialization was successful, false otherwise.
*/
bool init(void);

/**
 * Copy the log file to the USB key.
 * Prints error if they happen.
 * @return True if copy was successful, false otherwise.
*/
bool copy_log_file_to_usb();
