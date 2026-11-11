/**
 * Parsing earthquake USGS GeoJSON Data
 */

// Checking if earthquake code has been defined yet
#ifndef EARTHQUAKE
#define EARTHQUAKE

#include <stdio.h>
#include <stdbool.h>
#include "cJSON.h"
#include "download.h"
#include <stdbool.h>

/**
 * Checks the earthquake GeoJSON for any errors with the formatting or data.
 *
 * Parameters:
 *      json: A pointer to the cJSON object for the USGS GeoJSON data
 *
 * Returns:
 *      bool: A boolean on whether the GeoJSON file has any errors or not
 */
bool checkEarthquakes(cJSON *json);

/**
 * Loads the earthquakes data from the disk and returns a pointer to the cJSON object.
 *
 * Parameters:
 *     time: Time period of the earthquakes, of datatype enum DataTime
 *          - ALL_HOUR = One hour
 *          - ALL_DAY = One day
 *          - ALL_WEEK = Seven days
 *          - ALL_MONTH = Thirty days
 *     magnitude: Minimum magnitude included in the data
 *          - SIGNIFICANT = Significant earthquakes or 7.5+
 *          - M4_5 = Magnitude 4.5+
 *          - M2_5 = Magnitude 2.5+
 *          - M1_0 = Magnitude 1.0+
 *          - ALL = All earthquakes
 *
 * Returns:
 *     json: A pointer to the cJSON object for the USGS GeoJSON data
*/
cJSON *loadEarthquakes(enum DataTime time, enum DataMagnitude magnitude);

/**
 * Sorts the earthquake cJSON object from least to greatest magnitude.
 *
 * Parameters:
 *      json: A pointer to the cJSON object created for the GeoJSON data. Preferrably returned from loadEarthquakes
 * Returns:
 *      json: A pointer to a duplicate cJSON object that is sorted from most to least magnitude
 */
cJSON *sortingMag(cJSON *json);

/**
 * Prints out all earthquake's magnitude and place.
 *
 * Parameters:
 *      json: A pointer to the cJSON object created for the GeoJSON data. Preferrably returned from loadEarthquakes
 *      percent: double from 0 - 1 that states the percentage of the data to print out
*/
void printEarthquakes(cJSON *json, double percent);

#endif
