/**
 * File containing the functions to interact with the log file.
 * @file log.c
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#include "log.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ------------------------------------------------------------------------- */
/*                                 Functions                                 */
/* ------------------------------------------------------------------------- */

bool create_log(int id, char* description)
{
    log_t log;
    time_t timestamp = time(NULL);
    FILE* log_file = fopen(PATH_LOG, "a"); // Open in append mode

    if (log_file == NULL) return false;

    // Create log struct, copy description until MAX_LENGTH_DESCRIPTION
    log.timestamp = timestamp;
    log.id = id;
    strncpy(log.description, description, MAX_LENGTH_DESCRIPTION);

    // Write log data to file
    fprintf(log_file, "%ld %d \"%s\"\n", log.timestamp, log.id, log.description);

    fclose(log_file);
    return true;
}

bool create_emergency_log(void)
{
    return create_log(EMERGENCY_ID, EMERGENCY_DESCRIPTION);
}

bool create_emergency_solved_log(void)
{
    return create_log(EMERGENCY_SOLVED_ID, EMERGENCY_SOLVED_DESCRIPTION);
}

bool read_log(log_t* log, int index)
{
    FILE* log_file = fopen(PATH_LOG, "r");
    char line[MAX_LENGTH_LINE];

    if (log_file == NULL) return false;

    for (int i = 0; fgets(line, sizeof(line), log_file) != NULL; i++) {
        if (i != index) continue;

        // Split line into timestamp, id and description
        char* timestamp = strtok(line, " ");
        char* id = strtok(NULL, " \"");
        char* description = strtok(NULL, "\"");

        // Copy data to log struct
        log->timestamp = atol(timestamp);
        log->id = atoi(id);
        strcpy(log->description, description);
    }

    fclose(log_file);
    return true;
}

int count_logs(void)
{
    FILE* log_file = fopen(PATH_LOG, "r");
    char line[MAX_LENGTH_LINE];
    int count = 0;

    if (log_file == NULL) return 0;

    while (fgets(line, sizeof(line), log_file) != NULL)
        count++;

    fclose(log_file);
    return count;
}

bool is_emergency_active(void)
{
    FILE* log_file = fopen(PATH_LOG, "r");
    char line[MAX_LENGTH_LINE];
    bool emergency_active = false;

    if (log_file == NULL) return false;

    while (fgets(line, sizeof(line), log_file) != NULL) {
        // Split line to get the id
        strtok(line, " ");
        char* id = strtok(NULL, " \"");

        // Update emergency_active if id is EMERGENCY_ID or EMERGENCY_SOLVED_ID
        if (atoi(id) == EMERGENCY_ID)
            emergency_active = true;
        else if (atoi(id) == EMERGENCY_SOLVED_ID)
            emergency_active = false;
    }

    fclose(log_file);
    return emergency_active;
}
