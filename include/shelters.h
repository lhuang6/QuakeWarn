// Header for the shelter.c source code

#ifndef SHELTERS
#define SHELTERS

#include <stdio.h>
#include "cJSON.h"
#include <stdbool.h>

/**
 * 
 */
typedef struct {
    cJSON *openShelters;
    cJSON *closedShelters;
    cJSON *fullShelters;
    cJSON *alertShelters;
} allShelters;

bool checkShelters(cJSON *json);

cJSON *loadShelters();

void printShelters(cJSON *json, double percent);

#endif
