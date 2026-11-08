// HTTP server for connecting C code with HTML

#include "http.h"
#include <stdio.h>
#include <stdbool.h>
#include "cJSON.h"
#include <math.h>
#include <ctype.h>

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


void sortingImpact(cJSON *json, double latitude, double longitude){
    // Array of earthquakes
    cJSON *features = cJSON_GetObjectItem(json, "features");

    // How many earthquakes there are
    int count = cJSON_GetArraySize(features);

    // Performing an Insertion Sort:
    // 1. Start with the second element as the first element is assumed to be sorted.
    // 2. Compare the second element with the first if the second is smaller then swap them.
    // 3. Move to the third element, compare it with the first two, and put it in its correct position
    // 4. Repeat until the entire array is sorted.
    for (int i = 1; i < count; i++){

        // Fetching coordinates of each earthquake
        cJSON *feature = cJSON_GetArrayItem(features, i);
        cJSON *geometry = cJSON_GetObjectItemCaseSensitive(feature, "geometry");
        cJSON *coordinates = cJSON_GetObjectItemCaseSensitive(geometry, "coordinates");

        //Decleared the longitudes and latitudes for the distance formula
        double shelterLon = cJSON_GetArrayItem(coordinates, 0)->valuedouble;
        double shelterLat = cJSON_GetArrayItem(coordinates, 1)->valuedouble;

        // Distance between the point and the shelter's coordinates
        double distance = HaversineFormula(latitude, longitude, shelterLat, shelterLon);

        // Finding the magnitude of the earthquakes
        cJSON *properties = cJSON_GetObjectItem(feature, "properties");
        cJSON *mag = cJSON_GetObjectItemCaseSensitive (properties, "mag");

        //Declearing mag as double or it will print 'mag'
        double magnitude = mag->valuedouble;

        // Impact formula to find the impact of each of the earthquake
        // If the impact is more then 500 then you have to be consured
        double impact = magnitude * 1000 / (distance + 1);

        // Index for checking the list on the left
        int j = i -1;

        // Comparing the sorted list on the left
        while(j >= 0){

            //Opens feature from geoJSON from the first line
            cJSON *Lastfeature = cJSON_GetArrayItem(features, j);

            //Goes inside geometry
            cJSON *Lastgeometry = cJSON_GetObjectItemCaseSensitive(Lastfeature, "geometry");

            //Goes to the coordinates for latitudes and longitudes
            cJSON *LastCoordinates = cJSON_GetObjectItemCaseSensitive(Lastgeometry, "coordinates");

            double lastShelterLon = cJSON_GetArrayItem(LastCoordinates, 0) ->valuedouble;
            double lastShelterLon = cJSON_GetArrayItem(LastCoordinates, 1) ->valuedouble;
            //Declearing the last distance of the feature
            double Lastdistance = HaversineFormula(latitude, longitude, lastShelterLon, lastShelterLon);

            cJSON *Lastproperties = cJSON_GetObjectItem(Lastfeature, "properties");
            cJSON *LastMag = cJSON_GetObjectItemCaseSensitive(Lastproperties, "mag");

            double LastMagnitude = LastMag->valuedouble;
            //Declearing the last impact with use of the formula
            double LastImpact = LastMagnitude*1000 / (Lastdistance + 1);

            if (LastImpact <= impact) {
                break;
            }

            // Going through each element towards the left
            j--;
        }

            // Move the current element into its sorted position
            if (j != i - 1) {
                // Detatching the element and inserting it back into the swapped position
                cJSON *item = cJSON_DetachItemFromArray(features, i);

                cJSON_InsertItemInArray(features, j + 1, item);
            }
        }
    }

void sortingDistance(cJSON *json, double latitude, double longitude){
    cJSON *features = cJSON_GetObjectItem(json, "features");

    // Counts how many strings 'features' there are
    int count = cJSON_GetArraySize(features);

    // Performing an Insertion Sort:
    // 1. Start with the second element as the first element is assumed to be sorted.
    // 2. Compare the second element with the first if the second is smaller then swap them.
    // 3. Move to the third element, compare it with the first two, and put it in its correct position
    // 4. Repeat until the entire array is sorted.
    for (int i = 1; i < count; i++) {


        // it goes line by line BUT it skips the first one, for J to sort
        cJSON *feature = cJSON_GetArrayItem(features, i);
        cJSON *geometry = cJSON_GetObjectItemCaseSensitive(feature, "geometry");
        cJSON *coordinates = cJSON_GetObjectItemCaseSensitive(geometry, "coordinates");

        double Shelterlon = cJSON_GetArrayItem(coordinates, 0)->valuedouble;
        double Shelterlat = cJSON_GetArrayItem(coordinates, 1)->valuedouble;

        double distance = HaversineFormula(latitude, longitude, Shelterlat, Shelterlon);

        // Compare with elements to the left
        int j = i - 1;
        while (j >= 0) {
            cJSON *LastFeature = cJSON_GetArrayItem(features, j);
            cJSON *LastGeometry = cJSON_GetObjectItemCaseSensitive(LastFeature, "geometry");
            cJSON *LastCoordinates = cJSON_GetObjectItemCaseSensitive(LastGeometry, "coordinates");
            double lastShelterLon = cJSON_GetArrayItem(LastCoordinates, 0)->valuedouble;
            double LastShelterlat = cJSON_GetArrayItem(LastCoordinates, 1)->valuedouble;

            double Lastdistance = HaversineFormula(latitude, longitude, LastShelterlat, lastShelterLon);
            if (Lastdistance <= distance) {
                break;
            }

            // Going through each element towards the left
            j--;
        }

        // Move the current element into its sorted position
        if (j != i - 1) {
            // Detatching the element and inserting it back into the swapped position
            cJSON *item = cJSON_DetachItemFromArray(features, i);
            cJSON_InsertItemInArray(features, j + 1, item);
        }
    }
}

char choice() {
    char type;

    do {
        //Seeing if the user picks Impact ordering or Distance ordering
        printf("Enter I for Impact (highest to lowest)\n");
        printf("Enter D for Distance (closest to farthest)\n");
        printf("Choice: ");

        scanf(" %c", &type);
        //This makes the whatever user typed uppercase
        type = toupper(type);

        if (type != 'I' && type != 'D') {//Error if it is not I or D
            fprintf(stderr, "Error: Enter I or D\n");
        }

    } while (type != 'I' && type != 'D');

    return type;
}

void PrintDistance(cJSON *json){
    char type = choice();
    double Userlat;
    double Userlon;
    do {
        Userlat = 0;
        Userlon = 0;
        //This is where you keep there you are in longitude and latitude
        printf("Enter latitude(between -90 to 90): ");
        scanf("%lf", &Userlat);

        if (Userlat < -90 || Userlat > 90){//Error if it going out of the range because earth only has 180 latitude
            fprintf(stderr, "Error: latitude must be between -90 to 90\n");
            continue;
        }

        printf("Enter longitude(between -180 to 180): ");
        scanf("%lf", &Userlon);

        if (Userlon < -180 || Userlon > 180){//Error if it going out of the range because earth only has 360 longitude
            fprintf(stderr, "Error: longitude must be between -180 to 180\n");
            continue;
        }
//Repeats until the lag and lat is kept right
    }while (Userlat < -90 || Userlat > 90 || Userlon < -180 || Userlon > 180);

        cJSON *features = cJSON_GetObjectItem(json, "features");
        //For the method i did for choice, D is fro distance and I is for Impact
        if (type == 'D'){//srting from least to most distance
            sortingDistance(json, Userlat, Userlon);
        }
        if(type == 'I'){//sorting from highest to lowest impact
            sortingImpact(json, Userlat, Userlon);
        }

        for (int i = 0; i < cJSON_GetArraySize(features); i++){
            //it goes line by line to grab the coordinates for distance
            cJSON *feature = cJSON_GetArrayItem(features, i);
            cJSON *geometry = cJSON_GetObjectItemCaseSensitive(feature, "geometry");
            cJSON *coordinates = cJSON_GetObjectItemCaseSensitive(geometry, "coordinates");

            //Coordinates being decleared
            double Shelterlon = cJSON_GetArrayItem(coordinates, 0)->valuedouble;
            double Shelterlat = cJSON_GetArrayItem(coordinates, 1)->valuedouble;

            //Distance is being calcualted with the use of HaversineFormula
            double distance = HaversineFormula(Userlat, Userlon, Shelterlat, Shelterlon);

            //Grabing magnitude for imapact calculation
            cJSON *properties = cJSON_GetObjectItem(feature, "properties");
            cJSON *mag = cJSON_GetObjectItemCaseSensitive (properties, "mag");

            //magnitude is getting an value in double
            double magnitude = mag->valuedouble;

            //If the impact is more then 500 then you have to be consured
            //Imapact bring calculated with distance and magnitude
            double impact = magnitude*1000 / (distance+1);

            //Printing the eartire sorting
            printf("Earthquake %d Distance: %.2f km\n The impact : %.2f\n", i + 1, distance, impact);

            //Repeating the process by make magnitude 0 again
            magnitude = 0.00;
    }
}

