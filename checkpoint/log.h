/**
 * Header for the file containing the functions to interact with the log file.
 * @file log.h
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#ifndef LOG_H
#define LOG_H

#include <time.h>

/* ------------------------------------------------------------------------- */
/*                          Constants & Definitions                          */
/* ------------------------------------------------------------------------- */

#define PATH_LOG "./resources/safecheck.log"
#define MAX_LENGTH_DESCRIPTION 256
#define ID_EMERGENCY -1
#define ID_EMERGENCY_SOLVED -2

/* ------------------------------------------------------------------------- */
/*                             Type definitions                              */
/* ------------------------------------------------------------------------- */

/**
 * Structure representing a log entry.
*/
struct log {
    time_t timestamp;
    int id; // ID_EMERGENCY for emergency, ID_EMERGENCY_SOLVED for emergency solved
    char description[MAX_LENGTH_DESCRIPTION]; // The participant's name, or emergency description
};

typedef struct log log_t;

#endif
