/**
 * Header for the file containing the functions to handle the RFID.
 * @file rfid.h
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#ifndef RFID_H
#define RFID_H

#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/*                          Constants & Definitions                          */
/* ------------------------------------------------------------------------- */

#define RFID_CODE_PARTICIPANT_RANGE_START 0
#define RFID_CODE_PARTICIPANT_RANGE_END 999
#define RFID_CODE_ADMIN 10000
#define RFID_BLOCK_ADRESS_CODE 1
#define RFID_BLOCK_ADRESS_NAME 2
#define READ_BLOCK_BUFFER_SIZE 32
#define MAX_LENGTH_FIRST_NAME 8
#define MAX_LENGTH_LAST_NAME MAX_LENGTH_FIRST_NAME

/* ------------------------------------------------------------------------- */
/*                             Type definitions                              */
/* ------------------------------------------------------------------------- */

struct rfid_read {
    unsigned int code;
    char first_name[MAX_LENGTH_FIRST_NAME + 1];
    char last_name[MAX_LENGTH_LAST_NAME + 1];
};

typedef struct rfid_read rfid_read_t;

/* ------------------------------------------------------------------------- */
/*                            Function prototypes                            */
/* ------------------------------------------------------------------------- */

bool wait_rfid_read(rfid_read_t* rfid_read);

#endif
