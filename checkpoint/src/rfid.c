/**
 * File containing the functions to handle the RFID.
 * @file rfid.c
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#include <stdio.h>

#include "../lib/rc522/rc522_main.h"

#include "rfid.h"

/* ------------------------------------------------------------------------- */
/*                                 Functions                                 */
/* ------------------------------------------------------------------------- */

bool wait_rfid_read(rfid_read_t* rfid_read)
{
    int code;
    unsigned char buffer[READ_BLOCK_BUFFER_SIZE];
    unsigned int converted_decimal;

    // Configure and initialize rc522 lib
    if (get_config_file()) exit(1);
    if (HW_init(RFID_SPI_SPEED, RFID_GPIO)) close_out(1);
    InitRc522();

    // Read runner ID
    code = read_block(RFID_BLOCK_ADRESS_CODE, buffer, 0);
    if (code == TAG_ERR) return false;
    sscanf((const char*)buffer, "%x", &converted_decimal); // Convert to decimal
    rfid_read->code = converted_decimal;

    // Close / disconnect the RFID card
    PcdHalt();

    // If admin, don't read name
    rfid_read->admin = rfid_read->code == RFID_CODE_ADMIN;
    if (rfid_read->admin) 
        return true;

    // If illegal code
    if (rfid_read->code < RFID_CODE_PARTICIPANT_RANGE_START || rfid_read->code > RFID_CODE_PARTICIPANT_RANGE_END)
        return false;

    // Read runner name
    code = read_block(RFID_BLOCK_ADRESS_NAME, buffer, 0);
    if (code == TAG_ERR) return false;

    // Convert array of characters representing a byte by groups of two to real byte array
    char byte_array[MAX_LENGTH_FIRST_NAME + MAX_LENGTH_LAST_NAME];
    char tmp_string[3];
    for (int i = 0; i < READ_BLOCK_BUFFER_SIZE; i += 2) {
        tmp_string[0] = buffer[i];
        tmp_string[1] = buffer[i + 1];
        tmp_string[2] = '\0';
        byte_array[i / 2] = strtol(tmp_string, NULL, 16);
    }

    // Copy first and last name in struct
    strncpy(rfid_read->first_name, byte_array, MAX_LENGTH_FIRST_NAME);
    strncpy(rfid_read->last_name, byte_array + MAX_LENGTH_FIRST_NAME, MAX_LENGTH_LAST_NAME);

    // Close / disconnect the RFID card
    PcdHalt();

    return true;
}
