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
#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/*                          Constants & Definitions                          */
/* ------------------------------------------------------------------------- */

#define PATH_LOG "./resources/safecheck.log"
#define MAX_LENGTH_DESCRIPTION 256
#define MAX_LENGTH_LINE 10 /* timestamp */ + 11 /* id */ + 2 /* quotes */ + MAX_LENGTH_DESCRIPTION + 3 /* spaces & newline */
#define EMERGENCY_ID -1
#define EMERGENCY_SOLVED_ID -2
#define EMERGENCY_DESCRIPTION "Emergency button pressed"
#define EMERGENCY_SOLVED_DESCRIPTION "Emergency solved"

/* ------------------------------------------------------------------------- */
/*                             Type definitions                              */
/* ------------------------------------------------------------------------- */

/**
 * Structure representing a log entry.
*/
struct log {
    time_t timestamp;
    int id; // Runner ID or ID_EMERGENCY for emergency, ID_EMERGENCY_SOLVED for emergency solved
    char description[MAX_LENGTH_DESCRIPTION]; // The participant's name, or emergency description
};

typedef struct log log_t;

/* ------------------------------------------------------------------------- */
/*                            Function prototypes                            */
/* ------------------------------------------------------------------------- */

/**
 * Creates a log entry in the log file.
 * @param id The ID of the runner, or a special ID.
 * @param description The description of the log entry. (e.g. the runner's name)
 * @return True if the log entry was created successfully, false otherwise.
*/
bool create_log(int id, char* description);

/**
 * Creates an emergency log entry in the log file.
 * @return True if the log entry was created successfully, false otherwise.
*/
bool create_emergency_log(void);

/**
 * Creates an emergency solved log entry in the log file.
 * @return True if the log entry was created successfully, false otherwise.
*/
bool create_emergency_solved_log(void);

/**
 * Reads a log entry from the log file.
 * @param log Pointer to the log struct to fill.
 * @param index The index of the log entry to read. (0 = first line)
 * @return True if the log entry was read successfully, false otherwise.
*/
bool read_log(log_t* log, int index);

/**
 * Counts the number of log entries in the log file.
 * @return The number of log entries.
*/
int count_logs(void);

/**
 * Returns the last emergency state.
 * @return True if an emergency is active, false otherwise.
*/
bool is_emergency_active(void);

#endif
