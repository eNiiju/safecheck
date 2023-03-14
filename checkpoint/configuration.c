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
/*                              Macro functions                              */
/* ------------------------------------------------------------------------- */

#define IS_NUMBER(n, s) !(n == 0 && strncmp(s, "0", 1) != 0)

/* ------------------------------------------------------------------------- */
/*                                 Functions                                 */
/* ------------------------------------------------------------------------- */

bool read_configuration(conf_t* configuration, char* path_to_configuration_file)
{
    // Reset the configuration struct
    *configuration = (conf_t) {
        .nb_checkpoints = 0,
        .nb_participants = 0,
        .order = {},
        .participants = {}
    };

    FILE* fp;
    int read_state = 0; // 0, READ_STATE_ORDER or READ_STATE_PARTICIPANTS
    char* line;
    size_t n = 0;
    ssize_t read_size;

    fp = fopen(path_to_configuration_file, "r");
    if (fp == NULL) return false;

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
            int parsed = atoi(line);
            if (!IS_NUMBER(parsed, line)) continue;
            configuration->order[configuration->nb_checkpoints] = parsed;
            configuration->nb_checkpoints++;
        }
        // Read the participant
        else if (read_state == READ_STATE_PARTICIPANTS) {
            int id;
            char name[MAX_NAME_LENGTH];

            // Get the id
            char* token = strtok(line, " ");
            id = atoi(token);
            if (token == NULL || !IS_NUMBER(id, token)) continue;

            // Get the name
            token = strtok(NULL, "\"");
            if (token == NULL) continue;
            strncpy(name, token, MAX_NAME_LENGTH);

            // Add the participant to the array
            configuration->participants[configuration->nb_participants].id = id;
            strcpy(configuration->participants[configuration->nb_participants].name, name);
            configuration->nb_participants++;
        }
    }

    fclose(fp);
    if (line) free(line);

    return true;
}

bool write_configuration(conf_t* configuration, char* path_to_configuration_file)
{
    FILE* fp = fopen(path_to_configuration_file, "w");
    if (fp == NULL) return false;

    // Write the order
    fprintf(fp, "%s\n", ORDER_SECTION_START);
    for (int i = 0; i < configuration->nb_checkpoints; i++)
        fprintf(fp, "%d\n", configuration->order[i]);

    // Write the participants
    fprintf(fp, "%s\n", PARTICIPANTS_SECTION_START);
    for (int i = 0; i < configuration->nb_participants; i++)
        fprintf(fp, "%d \"%s\"\n", configuration->participants[i].id, configuration->participants[i].name);

    fclose(fp);
    return true;
}
