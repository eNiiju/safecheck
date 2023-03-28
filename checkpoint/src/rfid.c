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

void my_read()
{
    uint8_t gpio = 22;
    uint32_t spi_speed = 1000L;
    if (get_config_file()) exit(1);
    if (HW_init(spi_speed, gpio)) close_out(1);
    InitRc522();

    int max_blocks = 64;
    int addr = -1, tmp;
    unsigned char buff[35];

    // get valid block (reading block 0 is allowed)
    printf("Please provide the block (Max %d) to read : ", max_blocks - 1);
    addr = get_block_number(max_blocks - 1, 0);

    // read block and display content
    tmp = read_block(addr, buff, 1);

    // close / disconnect the card
    PcdHalt();
}
