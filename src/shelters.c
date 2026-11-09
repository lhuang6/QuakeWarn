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
 *     allShelters shelters: Open, Closed, Full, and Alert shelters in one struct:
 *          cJSON *openShelters - All open shelters
 *          cJSON *closedShelters - Closed shelters
 *          cJSON *fullShelters - Shelters at full capacity
 *          cJSON *alertShelters - Ones that are alert
*/
allShelters loadShelters() {

    // Setting all shelters to null
    allShelters shelters = {0};

    // Looping through each query type
    for (int i = 0; i < 4; i++) {

        // Opening files shelters_0 to shelters_1.geojson
        char filename[100];
        snprintf(filename, sizeof(filename),
                 "data/shelters_%d.geojson", i);

        // Loading the shelter data on JSON
        cJSON *json = loadJson(filename);

        // Adding a new distance object to the geometry
        // Distance will be used for later methods
        cJSON *geometry = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "features"), "geometry");
        cJSON_AddNullToObject(geometry, "distance");

        // Checking if the shelter data passed the checks
        if (!checkShelters(json)) {

            // Deleting the memory of the json file
            cJSON_Delete(json);

            // Deleting the cJSON objects
            cJSON_Delete(shelters.openShelters);
            cJSON_Delete(shelters.closedShelters);
            cJSON_Delete(shelters.fullShelters);
            cJSON_Delete(shelters.alertShelters);

            // Returning all null if not passed
            return (allShelters){0};
        }

        // Setting the json objects to the right place in the struct
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

    // Returning the shelters object
    return shelters;
}

/**
 * Loads the shelter data that only contains OPEN shelters
 *
 * Returns:
 *     json: A pointer to the cJSON object for the FEMA GeoJSON object.
*/
cJSON *loadOpenShelters() {

    char filename[100];
    snprintf(filename, sizeof(filename),
            "data/shelters_0.geojson");

    // Loading the shelter data on JSON
    cJSON *json = loadJson(filename);

    // Checking if the shelter data passed the checks
    if (!checkShelters(json)) {
        // Deleting the memory of the json file
        cJSON_Delete(json);
        // Returning all null if not passed
        return NULL;
    }

    // Returning the shelters object
    return json;
}

/**
 * Loads the shelter data that only contains CLOSED shelters
 *
 * Returns:
 *     json: A pointer to the cJSON object for the FEMA GeoJSON object.
*/
cJSON *loadClosedShelters() {

    char filename[100];
    snprintf(filename, sizeof(filename),
            "data/shelters_1.geojson");

    // Loading the shelter data on JSON
    cJSON *json = loadJson(filename);

    // Checking if the shelter data passed the checks
    if (!checkShelters(json)) {
        // Deleting the memory of the json file
        cJSON_Delete(json);
        // Returning all null if not passed
        return NULL;
    }

    // Returning the shelters object
    return json;
}

/**
 * Loads the shelter data that only contains FULL shelters
 *
 * Returns:
 *     json: A pointer to the cJSON object for the FEMA GeoJSON object.
*/
cJSON *loadFullShelters() {
    char filename[100];
    snprintf(filename, sizeof(filename),
            "data/shelters_2.geojson");

    // Loading the shelter data on JSON
    cJSON *json = loadJson(filename);

    // Checking if the shelter data passed the checks
    if (!checkShelters(json)) {
        // Deleting the memory of the json file
        cJSON_Delete(json);
        // Returning all null if not passed
        return NULL;
    }

    // Returning the shelters object
    return json;
}

/**
 * Loads the shelter data that only contains ALERT shelters
 *
 * Returns:
 *     json: A pointer to the cJSON object for the FEMA GeoJSON object.
*/
cJSON *loadAlertShelters() {
    char filename[100];
    snprintf(filename, sizeof(filename),
            "data/shelters_3.geojson");

    // Loading the shelter data on JSON
    cJSON *json = loadJson(filename);

    // Checking if the shelter data passed the checks
    if (!checkShelters(json)) {
        // Deleting the memory of the json file
        cJSON_Delete(json);
        // Returning all null if not passed
        return NULL;
    }

    // Returning the shelters object
    return json;
}

/**
 * Frees up the memory of all the cJSON objects inside the allShelters struct.
 *
 * Parameters:
 *      allShelters shelters: Open, Closed, Full, and Alert shelters in one struct:
 *          cJSON *openShelters - All open shelters
 *          cJSON *closedShelters - Closed shelters
 *          cJSON *fullShelters - Shelters at full capacity
 *          cJSON *alertShelters - Ones that are alert
 */
void freeShelters(allShelters shelters) {
    // Using cJSON_Delete to delete all fields
    cJSON_Delete(shelters.openShelters);
    cJSON_Delete(shelters.closedShelters);
    cJSON_Delete(shelters.fullShelters);
    cJSON_Delete(shelters.alertShelters);
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
