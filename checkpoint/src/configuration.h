/**
 * Header for the file containing the functions to interact with the configuration file.
 * @file configuration.h
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/*                          Constants & Definitions                          */
/* ------------------------------------------------------------------------- */

#define CONFIG_FILE_NAME "safecheck.conf"
#define CONFIG_FILE_PATH "/etc/safecheck.conf"
#define CONFIG_DIRECTORY "/etc/"
#define MAX_CHECKPOINTS 64
#define MAX_NAME_LENGTH 256
#define MAX_PARTICIPANTS 351
#define ORDER_SECTION_START "[order]"
#define PARTICIPANTS_SECTION_START "[participants]"
#define READ_STATE_ORDER 1
#define READ_STATE_PARTICIPANTS 2

/* ------------------------------------------------------------------------- */
/*                             Type definitions                              */
/* ------------------------------------------------------------------------- */

/**
 * Structure representing a participant.
*/
struct participant {
    int id;
    char name[MAX_NAME_LENGTH];
};

/**
 * Structure representing a Safecheck configuration.
*/
struct configuration {
    int order[MAX_CHECKPOINTS];
    int nb_checkpoints;
    struct participant participants[MAX_PARTICIPANTS];
    int nb_participants;
};

typedef struct configuration conf_t;

/* ------------------------------------------------------------------------- */
/*                            Function prototypes                            */
/* ------------------------------------------------------------------------- */

/**
 * Read the configuration file and store the data in a configuration structure.
 * @param configuration Pointer to the configuration structure.
 * @param path_to_configuration_file Path to the configuration file.
 * @return True if the configuration file was read successfully, false otherwise.
*/
bool read_configuration(conf_t* configuration, char* path_to_configuration_file);

/**
 * Write the data of a configuration structure in a file.
 * @param configuration Pointer to the configuration structure.
 * @param path_to_configuration_file Path to the configuration file.
 * @return True if the configuration file was written successfully, false otherwise.
*/
bool write_configuration(conf_t* configuration, char* path_to_configuration_file);

#endif