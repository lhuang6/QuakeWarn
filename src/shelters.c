// Calculating distance between shelters and earthquakes

#include "shelters.h"
#include "download.h"
#include "cJSON.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * Checks the shelter GeoJSON for any errors with the formatting or data.
 *
 * Parameters:
 *      json: A pointer to the cJSON object for the FEMA GeoJSON Data.
 *
 * Returns:
 *      bool: A boolean on whether the GeoJSON file has any errors or not
 */
bool checkShelters(cJSON *json) {

    // Checking if earthquake data has an object named "features"
    // This stores all shelters
    if (!cJSON_HasObjectItem(json, "features")) {

        // Error message
        fprintf(stderr, "Earthquake JSON Syntax Error: No object named \"features\"");

        // Returning false because the data did not pass a test
        return false;
    }

    // To-Do
    return true;
}

/**
 * Loads the shelter data from the disk and returns a pointer to the cJSON object.
 *
 * Returns:
 *     json: A pointer to the cJSON object for the FEMA GeoJSON object.
*/
allShelters loadShelters() {
    
    allShelters shelters = {0};

    for (int i = 0; i < 4; i++) {

        char filename[100];

        snprintf(filename, sizeof(filename),
                 "data/shelters_%d.geojson", i);

        cJSON *json = loadJson(filename);

        if (!checkShelters(json)) {
            cJSON_Delete(json);

            cJSON_Delete(shelters.openShelters);
            cJSON_Delete(shelters.closedShelters);
            cJSON_Delete(shelters.fullShelters);
            cJSON_Delete(shelters.alertShelters);

            return (allShelters){0};
        }

        switch (i) {
            case 0:
                shelters.openShelters = json;
                break;
            case 1:
                shelters.closedShelters = json;
                break;
            case 2:
                shelters.fullShelters = json;
                break;
            case 3:
                shelters.alertShelters = json;
                break;
        }
    }

    return shelters;
}

/**
 * Prints out shelter names and their addresses.
 *
 * Parameters:
 *      json: A pointer to the cJSON object created for the GeoJSON data. Preferrably returned from loadShelters
 *      percent: double from 0 - 1 that states the percentage of the data to print out
*/
void printShelters(cJSON *json, double percent) {

    // Checks if the percentage to print is valid
    if (percent <= 0 || percent > 1) {
        fprintf(stderr, "Percent must be between 0-100%%\n");

    // Continue if the percentage is valid
    } else {
        // Fetches earthquake features list
        cJSON *features = cJSON_GetObjectItem(json, "features");

        // Counts how many earthquakes there are
        int count = cJSON_GetArraySize(features);
        int stop = count * percent;

        printf("Found %d shelters.\n", count);
        printf("Printing %d shelters...\n\n", stop);

        // Loops through all the different earthquakes and stores their specific details
        for (int i = 0; i < stop; i++) {

            // Each shelter
            cJSON *feature = cJSON_GetArrayItem(features, i);

            // Array for the latitude and longitude
            // cJSON *coords = cJSON_GetArrayItem(cJSON_GetObjectItem(feature, "geometry"), "coordinates");

            // Rest of the shelter information
            cJSON *properties = cJSON_GetObjectItem(feature, "properties");

            // Name, status (closed or open), and information for the full address
            char *name = cJSON_GetObjectItem(properties, "shelter_name")->valuestring;
            char *status = cJSON_GetObjectItem(properties, "shelter_status_code")->valuestring;
            char *address_1 = cJSON_GetObjectItem(properties, "address_1")->valuestring;
            char *city = cJSON_GetObjectItem(properties, "city")->valuestring;
            char *state = cJSON_GetObjectItem(properties, "state")->valuestring;
            char *zip = cJSON_GetObjectItem(properties, "zip")->valuestring;

            // Printing the status, name and the full address to the shelter
            printf("[%s] %s\n", status, name);
            printf("    %s, %s, %s %s\n", address_1, city, state, zip);
        }
    }
}
