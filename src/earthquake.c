// Reading and sorting earthquake data from USGS

#include <stdio.h>
#include <curl/curl.h>
#include "earthquake.h"
#include "cJSON.h"

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

    // Name of the file on the USGS website
    char jsonFileName[100];
    snprintf(jsonFileName, sizeof(jsonFileName), "%s_%s.geojson", magLink, timeLink);

    // Where the data is going to be downloaded in
    char jsonDiskName[100];
    snprintf(jsonDiskName, sizeof(jsonDiskName), "data/%s", jsonFileName);

    // Full link to the GeoJSON data
    char jsonLink[100];
    snprintf(jsonLink, sizeof(jsonLink), "https://earthquake.usgs.gov/earthquakes/feed/v1.0/summary/%s", jsonFileName);

    // Creating or replacing file
    // Able to replace the file for updating it to a more recent time
    FILE *jsonDiskFile = fopen(jsonDiskName, "w");

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
    curl_easy_setopt(curl, CURLOPT_URL, jsonLink);              // Setting the URL to the JSON
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeJson);   // Function for writing data onto the disk
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, jsonDiskFile);    // Specifying where to write the data

    // Downloading the data from USGS
    result = curl_easy_perform(curl);

    if (result == CURLE_OK) {
        printf("[OK] Downloaded data from:\n%s\n", jsonLink);
    } else {
        printf("[OK] Unable to download data from:\n%s\n", jsonLink);
    }

    // Closing the file
    fclose(jsonDiskFile);

    // Cleaning up curl globally and the easy instance
    //
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

