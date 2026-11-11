/**
 * All the methods for downloading earthquake or shelter GeoJSON into the project. All of the
 * GeoJSON is downloaded in /data/
 */

#include <stdio.h>
#include <curl/curl.h>
#include "download.h"
#include <stdlib.h>
#include <unistd.h>
#include "cJSON.h"

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

size_t writeJson(char *data, size_t size, size_t nmemb, void *userdata) {
    size_t written = fwrite(data, size, nmemb, (FILE *)userdata);
    return written;
}

void downloadJson(JsonData jsonPaths) {
    // Creating or replacing file
    // Able to replace the file for updating it to a more recent time
    FILE *jsonDiskFile = fopen(jsonPaths.jsonDiskName, "w");

    // Initializing libcurl globally to be used
    // Using default parameters - "CURL_GLOBAL_DEFAULT"
    CURLcode result; // Code for the status of libcurl methods
    result = curl_global_init(CURL_GLOBAL_DEFAULT);

    // Printing out the status of libcurl
    if (result == CURLE_OK) {
        printf("\n[OK] Curl global initialization\n");
    } else {
        printf("\n[FAIL] Curl unable to initialize\n");
    }

    // "Easy initializing" of curl
    // Basically creating an instance for libcurl to download data
    // Easy because it's the easy way to get the data
    CURL *curl = curl_easy_init();

    // Setting options for download
    curl_easy_setopt(curl, CURLOPT_URL, jsonPaths.jsonLink);    // Setting the URL to the JSON
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

void downloadEarthquakes(enum DataTime time, enum DataMagnitude magnitude) {
    // JsonData struct that stores the download link and path to JSON file
    JsonData jsonPaths = enumToLink(time, magnitude);

    downloadJson(jsonPaths);
}

void downloadShelters() {

    // Downloading shelter data queries 0-4 from FEMA
    // 0 - Open Shelters
    // 1 - Closed Shelters
    // 2 - Full Shelters
    // 3 - Alert Shelters
    for (int i = 0; i <= 3; i++) {

        // Struct containing paths to all the files
        JsonData jsonPaths;

        // GeoJSON file
        snprintf(jsonPaths.jsonFileName, sizeof(jsonPaths.jsonFileName),
                "shelters_%d.geojson", i);

        // Disk file
        snprintf(jsonPaths.jsonDiskName, sizeof(jsonPaths.jsonDiskName),
                "data/shelters_%d.geojson", i);

        char link[2000];

        // Big link for FEMA query to download
        snprintf(link, sizeof(link),
            "https://gis.fema.gov/arcgis/rest/services/NSS/FEMA_NSS/MapServer/%d/query?where=1%%3D1&text=&objectIds=&time=&timeRelation=esriTimeRelationOverlaps"
            "&geometry=&geometryType=esriGeometryPoint&inSR=&spatialRel=esriSpatialRelIntersects&distance=&units=esriSRUnit_Foot&relationParam=&"
            "outFields=shelter_name%%2C+address_1%%2C+city%%2C+state%%2C+zip%%2C+org_organization_name"
            "%%2C+org_address%%2C+org_city%%2C+org_state%%2C+org_zip%%2C+org_hours_of_operation%%2C+shelter_status_code%%2C+shelter_open_date%%2C+shelter_closed_date"
            "&returnGeometry=true&returnTrueCurves=false&maxAllowableOffset=&geometryPrecision=&outSR=4326&havingClause=&returnIdsOnly=false&returnCountOnly=false"
            "&orderByFields=&groupByFieldsForStatistics=&outStatistics=&returnZ=false&returnM=false&gdbVersion=&historicMoment=&returnDistinctValues=false&resultOffset="
            "&resultRecordCount=&returnExtentOnly=false&sqlFormat=none&datumTransformation=&parameterValues=&rangeValues=&quantizationParameters=&uniqueIds="
            "&returnUniqueIdsOnly=false&featureEncoding=esriDefault&f=geojson",
            i);

        // Adding it to the jsonPaths struct
        snprintf(jsonPaths.jsonLink, sizeof(jsonPaths.jsonLink), "%s", link);

        // Downloading all of the data
        downloadJson(jsonPaths);
    }
}

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

    cJSON *features = cJSON_GetObjectItem(json, "features");
    int count = cJSON_GetArraySize(features);

    // Adding a new distance object to the geometry
    // Distance will be used for later methods
    for(int i = 0; i < count; i++) {

        // Adding "distance: " to every geometry section of shelter data
        cJSON *geometry = cJSON_GetObjectItem(cJSON_GetArrayItem(features, i), "geometry");
        cJSON_AddNullToObject(geometry, "distance");
    }

    // Returning the cJSON object for users or other methods to use
    return json;
}

void pollData(enum DataTime time, enum DataMagnitude magnitude){

    // Fix later to stop once program is turned off
    while (1){
        printf("Downloading new earthquake data...\n");

        // Downloads the most recent CSV file of earthquake data
        downloadEarthquakes(time, magnitude);
        downloadShelters();

        // Waits 60 seconds to poll again
        sleep(60);
    }
}
