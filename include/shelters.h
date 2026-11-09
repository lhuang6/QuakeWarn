// Header for the shelter.c source code

#ifndef SHELTERS
#define SHELTERS

#include <stdio.h>
#include "cJSON.h"
#include <stdbool.h>

/**
 * Pointers for all cJSON shelter data based on their status
 */
typedef struct {
    cJSON *openShelters;    // All open shelters
    cJSON *closedShelters;  // Closed shelters
    cJSON *fullShelters;    // Shelters at full capacity
    cJSON *alertShelters;   // Ones that are alert
} allShelters;

bool checkShelters(cJSON *json);

allShelters loadShelters();

cJSON *loadOpenShelters();

cJSON *loadClosedShelters();

cJSON *loadFullShelters();

cJSON *loadAlertShelters();

void freeShelters(allShelters shelters);

void printShelters(cJSON *json, double percent);

#endif
