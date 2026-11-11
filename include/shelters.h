/**
 * Parsing shelter GeoJSON data from FEMA
 */

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
    cJSON *alertShelters;   // Alert Shelters
} allShelters;

/**
 * Checks the shelter GeoJSON for any errors with the formatting or data.
 *
 * Parameters:
 *      json: A pointer to the cJSON object for the FEMA GeoJSON Data.
 *
 * Returns:
 *      bool: A boolean on whether the GeoJSON file has any errors or not
 */
bool checkShelters(cJSON *json);

allShelters loadShelters();

cJSON *loadOpenShelters();

cJSON *loadClosedShelters();

cJSON *loadFullShelters();

cJSON *loadAlertShelters();

void freeShelters(allShelters shelters);

void printShelters(cJSON *json, double percent);

#endif
