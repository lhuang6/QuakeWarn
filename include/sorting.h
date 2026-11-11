/**
 * All of the json sorting logic.
 */

#ifndef SORTING
#define SORTING

#include <stdio.h>
#include <stdbool.h>
#include "cJSON.h"
#include "mongoose.h"
#include <math.h>

/**
 * Computes the Haversine Formula for finding the distance between two points with their latitude and
 * longitude.
 *
 * Parameters:
 *      lat_1: -90 to 90 Degrees latitude for the first point
 *      lon_1: -180 to 180 Degrees latitude for the first point
 *      lat_2: -90 to 90 Degrees latitude for the second point
 *      lon_2: -180 to 180 Degrees latitude for the second point
 *
 * Returns:
 *      double: Distance between the two points in kilometers
 */
double haversineFormula(double lat_1, double lon_1, double lat_2, double lon_2);

/**
 * Calculates the distance between each shelter and a point and puts it in geometry -> distance
 *
 * Parameters:
 *      json: A pointer to the cJSON object created for the GeoJSON data. Preferrably returned from loadEarthquakes
 *      lat: Latitude (-90 to 90) of the point
 *      lon: Longitude (-180 to 180) of the point
 */
void addDistances(cJSON *json, double lat, double lon);

/**
 * Sorts shelters by closest to a point in latitude and longitude
 *
 * Parameters:
 *      json: A pointer to the cJSON object created for the GeoJSON data. Preferrably returned from loadEarthquakes
 *      lat: Latitude (-90 to 90) of the point
 *      lon: Longitude (-180 to 180) of the point
 *
 * Returns:
 *      duplicate: A new pointer to a cJSON object that has all of the earthquakes sorted
 */
cJSON *sortClosestShelters(cJSON *json, double lat, double lon);

/**
 * Sorts earthquakes based on how much the earthquake will impact a point. The formula for calculating "impact" is magnitude divided
 * by the distance from that earthquake.
 *
 * Parameters:
 *      json: A pointer to the cJSON object created for the GeoJSON data. Preferrably returned from loadEarthquakes
 *
 * Returns:
 *      duplicate: A new pointer to a cJSON object that has all of the earthquakes sorted
 */
cJSON *sortEarthquakeImpact(cJSON *json, double latitude, double longitude);

/**
 * Sorts earthquakes based on how far the earthquake is from a point.
 *
 * Parameters:
 *      json: A pointer to the cJSON object created for the GeoJSON data. Preferrably returned from loadEarthquakes
 *
 * Returns:
 *      duplicate: A new pointer to a cJSON object that has all of the earthquakes sorted
 */
cJSON *sortEarthquakeDistance(cJSON *json, double latitude, double longitude);

/**
 * Returns the type of sorting based on user input.
 *
 * Returns:
 *      char type: I or D for sorting between imapct or distance
 */
char choice();

/**
 * Prints the distance of shelter or earthquake data.
 *
 * Parameters:
 *      json: A valid cJSON object for shelter or earthquake data.
 */
void PrintDistance(cJSON *json);

#endif
