// Reading and sorting earthquake data from USGS

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "earthquake.h"
#include "cJSON.h"
#include <stdlib.h>
#include <unistd.h>

/**
 * Converts the time-range enum and the minimum magnitude into the JsonData struct that
 * stores the links and the paths to a JSON file.
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
 *      data: JsonData struct
 *          - jsonFileName = Name of the JSON file on the disk and web
            - jsonDiskName = Full path to the JSON file on the disk
            - jsonLink = Link to the JSON file to download
 */
JsonData enumToLink(enum DataTime time, enum DataMagnitude magnitude) {
    // Characters used in the GeoJSON link based on time-period and minimum magnitude
    char const *timeLink;   // Characters used for the time-period
    char const *magLink;    // Characters used for the minimum magnitude

    // Defining the characters used for the time-period
    // Defines for each enum DataTime
    switch(time) {
        case ALL_HOUR:
            timeLink = "hour";
            break;
        case ALL_DAY:
            timeLink = "day";
            break;
        case ALL_WEEK:
            timeLink = "week";
            break;
        case ALL_MONTH:
            timeLink = "month";
            break;
        // Defaults to one day
        default:
            timeLink = "day";
    }

    // Defining the characters used for the minimum magnitude
    // Defines for each enum DataMagnitude
    switch(magnitude) {
        case SIGNIFICANT:
            magLink = "significant";
            break;
        case M4_5:
            magLink = "4.5";
            break;
        case M2_5:
            magLink = "2.5";
            break;
        case M1_0:
            magLink = "1.0";
            break;
        case ALL:
            magLink = "all";
            break;
        // Defaults to all magnitude earthquakes
        default:
            magLink = "all";
            break;
    }

    // Putting all links and paths inside the JsonData struct
    JsonData data;

    // Name of the file on the USGS website
    snprintf(data.jsonFileName, sizeof(data.jsonFileName), "%s_%s.geojson", magLink, timeLink);

    // Where the data is going to be downloaded in
    snprintf(data.jsonDiskName, sizeof(data.jsonDiskName), "data/%s", data.jsonFileName);

    // Full link to the GeoJSON data
    snprintf(data.jsonLink, sizeof(data.jsonLink), "https://earthquake.usgs.gov/earthquakes/feed/v1.0/summary/%s", data.jsonFileName);

    return data;
}

/**
 * Used by libcurl to write data onto the disk.
 *
 * Parameters:
 *      data: A pointer for all of the data to download
 *      size: How much memory each item to download is
 *      nmemb: How many items there are to download
 *      userdata: Pointer of additional user specified data; in this case, it's the saved file.
 * Returns:
 *      written: Total size of everything written with fwrite()
 */
size_t writeJson(char *data, size_t size, size_t nmemb, void *userdata) {
    size_t written = fwrite(data, size, nmemb, (FILE *)userdata);
    return written;
}

/**
 * Method for downloading and updating the json file on the disk. Run it again to update it to the
 * most recent time.
 *
 * Parameters:
 *      time: Time period of the earthquakes, of datatype enum DataTime
 *          - ALL_HOUR = One hour
 *          - ALL_DAY = One day
 *          - ALL_WEEK = Seven days
 *          - ALL_MONTH = Thirty days
 *      magnitude: Minimum magnitude included in the data
 *          - SIGNIFICANT = Significant earthquakes or 7.5+
 *          - M4_5 = Magnitude 4.5+
 *          - M2_5 = Magnitude 2.5+
 *          - M1_0 = Magnitude 1.0+
 *          - ALL = All earthquakes
 */
void downloadJson(enum DataTime time, enum DataMagnitude magnitude) {
    // JsonData struct that stores the download link and path to JSON file
    JsonData jsonPaths = enumToLink(time, magnitude);

    // Creating or replacing file
    // Able to replace the file for updating it to a more recent time
    FILE *jsonDiskFile = fopen(jsonPaths.jsonDiskName, "w");

    // Initializing libcurl globally to be used
    // Using default parameters - "CURL_GLOBAL_DEFAULT"
    CURLcode result; // Code for the status of libcurl methods
    result = curl_global_init(CURL_GLOBAL_DEFAULT);

    // Printing out the status of libcurl
    if (result == CURLE_OK) {
        printf("[OK] Curl global initialization\n");
    } else {
        printf("[FAIL] Curl unable to initialize\n");
    }

    // "Easy initializing" of curl
    // Basically creating an instance for libcurl to download data
    // Easy because it's the easy way to get the data
    CURL *curl = curl_easy_init();

    // Setting options for download
    curl_easy_setopt(curl, CURLOPT_URL, jsonPaths.jsonLink);              // Setting the URL to the JSON
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeJson);   // Function for writing data onto the disk
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, jsonDiskFile);    // Specifying where to write the data

    // Downloading the data from USGS
    result = curl_easy_perform(curl);

    if (result == CURLE_OK) {
        printf("[OK] Downloaded data from:\n%s\n", jsonPaths.jsonLink);
    } else {
        printf("[OK] Unable to download data from:\n%s\n", jsonPaths.jsonLink);
    }

    // Closing the file
    fclose(jsonDiskFile);

    // Cleaning up curl globally and the easy instance
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

/**
 * Updates the JSON data every one minute for new earthquakes
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
*/
void pollEarthquakes(enum DataTime time, enum DataMagnitude magnitude){

    // Fix later to stop once program is turned off
    while (1){
        printf("Downloading new earthquake data...\n");

        // Downloads the most recent CSV file of earthquake data
        downloadJson(time, magnitude);

        // Waits 60 seconds to poll again
        sleep(60);
    }
}

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
        printf("JSON parse failed.\n");
        return NULL;
    }

    return json;
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

    return loadJson(jsonPaths.jsonDiskName);
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
