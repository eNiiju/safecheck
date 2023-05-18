/**
 * @author Noe Maillet & Mathéo Mercier
 * @file test_rfid.c
 * This file is made to test the RFID functions.
*/

#include "../lib/rc522/rc522_main.h"
#include "../src/rfid.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
    const int block = 1;
    int code;
    unsigned char buffer[READ_BLOCK_BUFFER_SIZE];
    unsigned int converted_decimal;

    // Configure and initialize rc522 lib
    if (get_config_file()) exit(1);
    if (HW_init(RFID_SPI_SPEED, RFID_GPIO)) close_out(1);
    InitRc522();

    // Read runner ID
    code = read_block(block, buffer, 1);
    if (code == TAG_ERR) return -1;
    sscanf((const char*)buffer, "%x", &converted_decimal); // Convert to decimal
    printf("Decimal read of block %d: %d", block, converted_decimal);

    // Close / disconnect the RFID card
    PcdHalt();

    return 0;
}
