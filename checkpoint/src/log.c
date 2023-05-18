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

bool create_log(int code, char* description)
{
    log_t log;
    time_t timestamp = time(NULL);
    FILE* log_file = fopen(LOG_FILE_PATH, "a"); // Open in append mode

    if (log_file == NULL) return false;

    // Create log struct, copy description until MAX_LENGTH_LOG_DESCRIPTION
    log.timestamp = timestamp;
    log.code = code;
    strncpy(log.description, description, MAX_LENGTH_LOG_DESCRIPTION);

    // Write log data to file
    fprintf(log_file, "%ld %d \"%s\"\n", log.timestamp, log.code, log.description);

    fclose(log_file);
    return true;
}

bool create_emergency_log(void)
{
    return create_log(EMERGENCY_CODE, EMERGENCY_DESCRIPTION);
}

bool create_emergency_solved_log(void)
{
    return create_log(EMERGENCY_SOLVED_CODE, EMERGENCY_SOLVED_DESCRIPTION);
}

bool read_log(log_t* log, int index)
{
    FILE* log_file = fopen(LOG_FILE_PATH, "r");
    char line[MAX_LENGTH_LOG_LINE];

    if (log_file == NULL) return false;

    for (int i = 0; fgets(line, sizeof(line), log_file) != NULL; i++) {
        if (i != index) continue;

        // Split line into timestamp, code and description
        char* timestamp = strtok(line, " ");
        char* code = strtok(NULL, " \"");
        char* description = strtok(NULL, "\"");

        // Copy data to log struct
        log->timestamp = atol(timestamp);
        log->code = atoi(code);
        strcpy(log->description, description);
    }

    fclose(log_file);
    return true;
}

int count_logs(void)
{
    FILE* log_file = fopen(LOG_FILE_PATH, "r");
    char line[MAX_LENGTH_LOG_LINE];
    int count = 0;

    if (log_file == NULL) return 0;

    while (fgets(line, sizeof(line), log_file) != NULL)
        count++;

    fclose(log_file);
    return count;
}

bool is_emergency_active(void)
{
    FILE* log_file = fopen(LOG_FILE_PATH, "r");
    char line[MAX_LENGTH_LOG_LINE];
    bool emergency_active = false;

    if (log_file == NULL) return false;

    while (fgets(line, sizeof(line), log_file) != NULL) {
        // Split line to get the code
        strtok(line, " ");
        char* code = strtok(NULL, " \"");

        // Update emergency_active if code is EMERGENCY_CODE or EMERGENCY_SOLVED_CODE
        if (atoi(code) == EMERGENCY_CODE)
            emergency_active = true;
        else if (atoi(code) == EMERGENCY_SOLVED_CODE)
            emergency_active = false;
    }

    fclose(log_file);
    return emergency_active;
}

bool get_last_log_by_code(log_t* log, int code)
{
    FILE* log_file = fopen(LOG_FILE_PATH, "r");
    char line[MAX_LENGTH_LOG_LINE];
    bool found = false;

    if (log_file == NULL) return false;

    while (fgets(line, sizeof(line), log_file) != NULL) {
        // Split line to get the code
        strtok(line, " ");
        char* log_code = strtok(NULL, " \"");

        // Update log if code matches
        if (atoi(log_code) == code) {
            char* timestamp = strtok(NULL, " \"");
            char* description = strtok(NULL, "\"");

            log->timestamp = atol(timestamp);
            log->code = atoi(log_code);
            strcpy(log->description, description);
            found = true;
        }
    }

    fclose(log_file);
    return found;
}
