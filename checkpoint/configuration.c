/**
 * File containing the functions to interact with the configuration file.
 * @file configuration.c
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "configuration.h"

/* ------------------------------------------------------------------------- */
/*                                 Functions                                 */
/* ------------------------------------------------------------------------- */

conf_t read_configuration(char* path_to_configuration_file)
{
    conf_t conf = {
        .nb_checkpoints = 0,
        .nb_participants = 0
    };
    FILE* fp;
    int read_state = 0; // 0, READ_STATE_ORDER or READ_STATE_PARTICIPANTS
    char* line;
    size_t n = 0;
    ssize_t read_size;

    fp = fopen(path_to_configuration_file, "r");
    if (fp == NULL) exit(EXIT_FAILURE);

    while ((read_size = getline(&line, &n, fp)) != -1) {
        // After this line, we are reading the order
        if (strncmp(line, ORDER_SECTION_START, strlen(ORDER_SECTION_START)) == 0) {
            read_state = READ_STATE_ORDER;
            continue;
        }

        // After this line, we are reading the participants
        if (strncmp(line, PARTICIPANTS_SECTION_START, strlen(PARTICIPANTS_SECTION_START)) == 0) {
            read_state = READ_STATE_PARTICIPANTS;
            continue;
        }

        // Empty line (only '\n' or nothing : useless)
        if (read_size <= 1)
            continue;

        // Read the checkpoint
        if (read_state == READ_STATE_ORDER) {
            conf.order[conf.nb_checkpoints] = atoi(line);
            conf.nb_checkpoints++;
        }
        // Read the participant
        else if (read_state == READ_STATE_PARTICIPANTS) {
            char* token = strtok(line, " ");
            conf.participants[conf.nb_participants].id = atoi(token);
            token = strtok(NULL, "\n");
            strcpy(conf.participants[conf.nb_participants].name, token);
            conf.nb_participants++;
        }
    }

    fclose(fp);
    if (line) free(line);

    return conf;
}
