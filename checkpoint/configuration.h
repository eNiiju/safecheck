/**
 * Header for the file containing the functions to interact with the configuration file.
 * @file configuration.h
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/* ------------------------------------------------------------------------- */
/*                          Constants & Definitions                          */
/* ------------------------------------------------------------------------- */

#define PATH_CONFIG "./resources/safecheck.conf"
#define MAX_CHECKPOINTS 64
#define MAX_NAME_LENGTH 256
#define MAX_PARTICIPANTS 351

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
    struct participant participants[MAX_PARTICIPANTS];
};

typedef struct configuration conf_t;

#endif
