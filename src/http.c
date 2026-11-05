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
double HaversineFormula(double Userlat, double Userlon, double Shelterlat, double Shelterlon){
    double radius = 6378.0;

    // To convert the degree to radians for trignomatry
    //M_PI is the Pi to
    Userlat = Userlat * M_PI/180;
    Userlon = Userlon * M_PI/180;
    Shelterlat = Shelterlat * M_PI/180;
    Shelterlon = Shelterlon * M_PI/180;

    // To convert the degree to radians for trignomatry
    double dlat = Shelterlat - Userlat;
    double dlon = Shelterlon - Userlon;

    // One of the steps to find the a temporary value that measures how separated the two locations are on the Earth's sphere.
    double a = sin(dlat / 2) * sin(dlat / 2) + cos(Userlat) * cos(Shelterlat) * sin(dlon / 2) * sin(dlon / 2);

    // To find the Angular distance between the two locations
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    // To convert distance into KM
    return radius * c;
}

void PrintDistance(cJSON *json) {
    double Userlat;
    double Userlon;
    do {
        Userlat = 0;
        Userlon = 0;
        printf("Enter latitude(between -90 to 90): ");
        scanf("%lf", &Userlat);

        printf("Enter longitude(between -180 to 180): ");
        scanf("%lf", &Userlon);
        if (Userlat < -90 || Userlat > 90){
            printf("Invalid latitude.\n");
            return;
        }
        if (Userlon < -180 || Userlon > 180){
            printf("Invalid longitude.\n");
            return;
        }
    }while (Userlat < -90 || Userlat > 90 || Userlon < -180 || Userlon > 180);
    
        cJSON *features = cJSON_GetObjectItem(json, "features");

        for (int i = 0; i < cJSON_GetArraySize(features); i++){
            cJSON *feature = cJSON_GetArrayItem(features, i);

            cJSON *geometry = cJSON_GetObjectItemCaseSensitive(feature, "geometry");

            cJSON *coordinates = cJSON_GetObjectItemCaseSensitive(geometry, "coordinates");

            double Shelterlon = cJSON_GetArrayItem(coordinates, 0)->valuedouble;
            double Shelterlat = cJSON_GetArrayItem(coordinates, 1)->valuedouble;

            double distance = HaversineFormula(Userlat, Userlon, Shelterlat, Shelterlon);

            cJSON *properties = cJSON_GetObjectItem(feature, "properties");
            cJSON *mag = cJSON_GetObjectItemCaseSensitive (properties, "mag");

            double magnitude = mag->valuedouble;
            //If the impact is more then 500 then you have to be consured
            double impact = magnitude*1000 / (distance+1);

            printf("Earthquake %d Distance: %.2f km\n The impact : %.2f\n", i + 1, distance, impact);

            magnitude = 0.00;
    }
}
