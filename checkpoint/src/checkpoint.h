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
/*                          Constants & Definitions                          */
/* ------------------------------------------------------------------------- */

#define SEND_DATA_PERIOD_S 60 // In seconds
#define EMERGENCY_BUTTON_PIN 5 // Wiring pi notation pin number

/* ------------------------------------------------------------------------- */
/*                    Thread routine function prototypes                     */
/* ------------------------------------------------------------------------- */

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
 * @brief waits for someone to click the emergency button
 *  puts a log of this event, print on the screen that emergency is indeed
 *  activated
 * @param arg unused
*/
void* button_routine(void* arg);

/* ------------------------------------------------------------------------- */
/*                            Function prototypes                            */
/* ------------------------------------------------------------------------- */

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

void retrieve_data_to_send(char* data_to_send, int max_size);
