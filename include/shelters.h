// Header for the shelter.c source code

#ifndef SHELTERS
#define SHELTERS

#include <stdio.h>
#include "cJSON.h"

/**
 * Contains pointers to two pointers to cJSON objects containing all open shelters and closed shelters
 */
typedef struct {
    cJSON *openShelters;    // All open shelters
    cJSON *closedShelters;  // All closed shelters
} SplitShelters;

bool checkShelters(cJSON *json);

cJSON *loadShelters();

void printShelters(cJSON *json, double percent);

SplitShelters splitSheltersStatus(cJSON *json);

#endif
