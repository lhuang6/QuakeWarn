// HTTP server for connecting C code with HTML

#include "http.h"
#include <stdio.h>
#include <stdbool.h>
#include "cJSON.h"
#include <math.h>

/**
 * Computes the Haversine Formula for finding the distance between two points with their latitude and longitude.
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
double HaversineFormula(double lat_1, double lon_1, double lat_2, double lon_2){
    double radius = 6378.0;

    // To convert the degree to radians for trignomatry
    //M_PI is the Pi to
    lat_1 = lat_1 * M_PI/180;
    lon_1 = lon_1 * M_PI/180;
    lat_2 = lat_2 * M_PI/180;
    lon_2 = lon_2 * M_PI/180;

    // To convert the degree to radians for trignomatry
    double dlat = lat_2 - lat_1;
    double dlon = lon_2 - lon_1;

    // One of the steps to find the a temporary value that measures how separated the two locations are on the Earth's sphere.
    double a = sin(dlat / 2) * sin(dlat / 2) + cos(lat_1) * cos(lat_2) * sin(dlon / 2) * sin(dlon / 2);

    // To find the Angular distance between the two locations
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    // To convert distance into KM
    return radius * c;
}
