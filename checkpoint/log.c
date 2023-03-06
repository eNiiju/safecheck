/**
 * File containing the functions to interact with the log file.
 * @file log.c
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#include "log.h"

#include <stdio.h>

/* ------------------------------------------------------------------------- */
/*                                 Functions                                 */
/* ------------------------------------------------------------------------- */

bool create_log(int id, char* description)
{
    FILE* log_file = fopen(PATH_LOG, "a");

    if (log_file == NULL) return false;

    time_t timestamp = time(NULL);

    // Write log data to file
    fprintf(log_file, "%ld %d \"%s\"\n", timestamp, id, description);

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
