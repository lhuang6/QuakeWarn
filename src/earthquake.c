// Reading and sorting earthquake data from USGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <curl/curl.h>
#include "download.h"
#include "earthquake.h"
#include "cJSON.h"
#include <stdlib.h>
#include <unistd.h>

/**
 * Loads any earthquake GeoJSON file.
 *
 * Parameters:
 *     filePath: A pointer to the path of the file on the disk
 *
 * Returns:
 *     json: A pointer to the cJSON object for the USGS GeoJSON data
*/
cJSON *loadJson(char *filePath) {
    // Opens the file downloaded from USGS
    FILE *file = fopen(filePath, "r");

    // Check if the file opening fails
    if (file == NULL) {
        printf("Unable to open file.\n");
        return NULL;
    }

    // Navigates to the end of the file and
    // determines the size of it in bytes
    // and navigates back to the beginning
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    // Allocates memory to fit the size of the file
    char *buffer = malloc(size + 1);

    // Stores the file contents in memory and appends
    // the string terminator
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    // Closes file
    fclose(file);

    // Parses text into JSON file
    cJSON *json = cJSON_Parse(buffer);

    // Free the memory
    free(buffer);

    // Checks if JSON parsing passed
    if (json == NULL) {
        // cJSON's pointer for an error message
        const char *error_ptr = cJSON_GetErrorPtr();

        // Printing out an error message if one exists
        if (error_ptr != NULL) {
            fprintf(stderr, "JSON Syntax Error before: %s\n", error_ptr);
        } else {
            fprintf(stderr, "JSON Syntax Error: Failed to parse JSON string.\n");
        }
    }

    // Returning the cJSON object for users or other methods to use
    return json;
}

/**
 * Checks the earthquake GeoJSON for any errors with the formatting or data.
 *
 * Parameters:
 *      json: A pointer to the cJSON object for the USGS GeoJSON data
 *
 * Returns:
 *      bool: A boolean on whether the GeoJSON file has any errors or not
 */
bool checkEarthquakes(cJSON *json) {

    // Checking if earthquake data has an object named "features"
    // This stores all each earthquake
    if (!cJSON_HasObjectItem(json, "features")) {

        // Error message
        fprintf(stderr, "Earthquake JSON Syntax Error: No object named \"features\"");

        // Returning false because the data did not pass a test
        return false;
    }

    // Checks if the "features" contains an array
    // Should have an array of earthquakes
    cJSON *features = cJSON_GetObjectItem(json, "features");

    // Returning NULL if it is not an array
    if (!cJSON_IsArray(features)) {
        // Error message
        fprintf(stderr, "Earthquake JSON Syntax Error: \"features\" does not contain an array!\n");

        // Returning false because the data did not pass a test
        return false;
    }

    // ----------------------------
    // CHECKING FOR NULL MAGNITUDES
    // ----------------------------
    // Total amount of earthquakes
    int count = cJSON_GetArraySize(features);

    // Looping through each earthquake
    for (int i = 0; i < count; i++) {

        // Grabbing the magnitude for each earthquake
        cJSON *feature = cJSON_GetArrayItem(features, i);
        cJSON *properties = cJSON_GetObjectItem(feature, "properties");
        cJSON *jsonMag = cJSON_GetObjectItem(properties, "mag");

        // Checking if magnitude is a number or not
        if (!cJSON_IsNumber(jsonMag)) {

            // Printing a warning statement if the magnitude is not a number
            printf("\033[35mWARNING: Earthquake at index %d has a null magnitude!\033[0m\n", i);

            // Removing this earthquake from the data
            cJSON *null = cJSON_DetachItemFromArray(features, 0);

            // Adding it to the end of the JSON file
            cJSON_AddItemToArray(features, null);

            // Subtracting the index by one because the null earthquake swapped places
            // So the other earthquakes moved left
            i--;
            count--;
            continue;
        }
    }

    // Passed all tests
    return true;
}

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
cJSON *loadEarthquakes(enum DataTime time, enum DataMagnitude magnitude) {
    // JsonData struct that stores the download link and path to JSON file
    JsonData jsonPaths = enumToLink(time, magnitude);

    // Parsing the JSON file into a cJSON object to use
    cJSON *json = loadJson(jsonPaths.jsonDiskName);

    // Check for any syntax errors
    bool pass = checkEarthquakes(json);

    // Returning the cJSON object if passed
    if (pass) {

        return json;
    } else {
        // Deleting the memory taken up by the cJSON object because the data is invalid
        cJSON_Delete(json);

        // Passing nothing
        return NULL;
    }
}

/**
 * Sorts the earthquake cJSON object from least to greatest magnitude.
 *
 * Parameters:
 *      json: A pointer to the cJSON object created for the GeoJSON data. Preferrably returned from loadEarthquakes
 */
void sortingMag(cJSON *json){
    cJSON *features = cJSON_GetObjectItem(json, "features");

    // Counts how many earthquakes there are
    int count = cJSON_GetArraySize(features);

    // Performing an Insertion Sort:
    // 1. Start with the second element as the first element is assumed to be sorted.
    // 2. Compare the second element with the first if the second is smaller then swap them.
    // 3. Move to the third element, compare it with the first two, and put it in its correct position
    // 4. Repeat until the entire array is sorted.
    for (int i = 1; i < count; i++) {


        // Grabbing the magnitude for each member on the right
        cJSON *feature = cJSON_GetArrayItem(features, i);
        cJSON *properties = cJSON_GetObjectItem(feature, "properties");
        cJSON *jsonMag = cJSON_GetObjectItem(properties, "mag");
        float magnitude = jsonMag->valuedouble;

        // Compare with elements to the left
        int j = i - 1;
        while (j >= 0) {
            // Grabbing the magnitude for each element on the left
            cJSON *lastFeature = cJSON_GetArrayItem(features, j);
            cJSON *lastProperties = cJSON_GetObjectItem(lastFeature, "properties");
            cJSON *lastJsonMag = cJSON_GetObjectItem(lastProperties, "mag");
            double lastMagnitude = lastJsonMag->valuedouble;

            // Stop when the correct position is found
            if (lastMagnitude <= magnitude) {
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

/**
 * Prints out all earthquake's magnitude and place.
 *
 * Parameters:
 *      json: A pointer to the cJSON object created for the GeoJSON data. Preferrably returned from loadEarthquakes
*/
void printEarthquakes(cJSON *json) {
    // Fetches earthquake features list
    cJSON *features = cJSON_GetObjectItem(json, "features");

    // Counts how many earthquakes there are
    int count = cJSON_GetArraySize(features);

    printf("Found %d earthquakes.\n", count);

    // Loops through all the different earthquakes and stores their specific details
    for (int i = 0; i < count; i++) {

        // Creates many different pointers relating to
        // features of the different earthquakes such as
        // the properties, place, and magnitude
        cJSON *feature = cJSON_GetArrayItem(features, i);
        cJSON *properties = cJSON_GetObjectItem(feature, "properties");
        cJSON *place = cJSON_GetObjectItem(properties, "place");
        cJSON *mag = cJSON_GetObjectItem(properties, "mag");

        // Processes all information and spits it out
        // --------------- Example -----------------
        // Magnitude 2.3 at Alaska
        // Magnitude 1.7 at Nevada
        // Magnitude 4.1 at Japan
        printf("Magnitude %.1f at %s\n", mag->valuedouble, place->valuestring);
    }

}

double HaversineFormula(double Userlat, double Userlon, double Shelterlat, double Shelterlon){
    printf("Enter latitude: ");
    scanf("%lf", &Userlat);

    printf("Enter longitude: ");
    scanf("%lf", &Userlon);

    double radius = 6378.0;
    ///
    /// To convert the degree to radians for trignomatry
    ///
    //M_PI is the Pi to
    Userlat = Userlat * M_PI/180;
    Userlon = Userlon * M_PI/180;
    Shelterlat = Shelterlat * M_PI/180;
    Shelterlon = Shelterlon * M_PI/180;
    ///
    ///To convert the degree to radians for trignomatry
    ///
    double dlat = Shelterlat - Userlat;
    double dlon = Shelterlon - Userlon;

    //One of the steps to find the a temporary value that measures how separated the two locations are on the Earth's sphere.
    double a = sin(dlat / 2) * sin(dlat / 2) + cos(Userlat) * cos(Shelterlat) * sin(dlon / 2) * sin(dlon / 2);
    //To find the Angular distance between the two locations
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    //To convert distance into KM
    return radius * c;
}

