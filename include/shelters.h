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

/**
 * Parses the shelter data downloaded onto the disk.
 *
 * Returns:
 *     allShelters shelters: Open, Closed, Full, and Alert shelters
*/
allShelters loadShelters();

/**
 * Loads the shelter data that only contains OPEN shelters
 *
 * Returns:
 *     json: A pointer to the cJSON object for the FEMA GeoJSON object.
*/
cJSON *loadOpenShelters();

/**
 * Loads the shelter data that only contains CLOSED shelters
 *
 * Returns:
 *     json: A pointer to the cJSON object for the FEMA GeoJSON object.
*/
cJSON *loadClosedShelters();

/**
 * Loads the shelter data that only contains FULL shelters
 *
 * Returns:
 *     json: A pointer to the cJSON object for the FEMA GeoJSON object.
*/
cJSON *loadFullShelters();

/**
 * Loads the shelter data that only contains ALERT shelters
 *
 * Returns:
 *     json: A pointer to the cJSON object for the FEMA GeoJSON object.
*/
cJSON *loadAlertShelters();

/**
 * Frees up the memory of all the cJSON objects inside the allShelters struct.
 *
 * Parameters:
 *      allShelters shelters: Open, Closed, Full, and Alert shelters
 */
void freeShelters(allShelters shelters);

/**
 * Prints out shelter names and their addresses.
 *
 * Parameters:
 *      json: A pointer to the cJSON object created for the GeoJSON data. Preferrably returned from loadShelters
 *      percent: double from 0 - 1 that states the percentage of the data to print out
*/
void printShelters(cJSON *json, double percent);

#endif
