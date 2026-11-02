#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "download.h"
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
 *      jsonPaths: JsonData struct
 *          - jsonFileName = Name of the JSON file on the disk and web
            - jsonDiskName = Full path to the JSON file on the disk
            - jsonLink = Link to the JSON file to download
 */
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
 * Method for downloading the earthquake GeoJSON data onto the disk.
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
void downloadEarthquakes(enum DataTime time, enum DataMagnitude magnitude) {
    // JsonData struct that stores the download link and path to JSON file
    JsonData jsonPaths = enumToLink(time, magnitude);

    downloadJson(jsonPaths);
}

/**
 * Method for downloading the shelter GeoJSON from FEMA onto the disk.
 *
 * This uses the super long http link from FEMA's query. The data included in the query is:
 *      - shelter_name: Name of the shelter
 *      - address_1: Street of the shelter
 *      - city: City of the shelter
 *      - zip: Zip code to the shelter
 *      - org_organization_name: Name of the shelter's organization
 *      - org_address: Address to the organization
 *      - org_city: City to the organization
 *      - org_state: State to the organization
 *      - org_hours_of_operation: Organization's hours of operation
 *      - shelter_status_code: Status of the shelter
 *      - shelter_open_date: When the shelter opened
 *      - shelter_closed_date: When the shelter closed
 */
void downloadShelters() {
    // JsonData struct that stores the download link and path to JSON file
    JsonData jsonPaths;

    snprintf(jsonPaths.jsonFileName, sizeof(jsonPaths.jsonFileName), "%s", "shelters.geojson");
    snprintf(jsonPaths.jsonDiskName, sizeof(jsonPaths.jsonDiskName), "%s", "data/shelters.geojson");

    // Huge link that queries all the FEMA shelter data needed
    char *link =
    "https://gis.fema.gov/arcgis/rest/services/NSS/FEMA_NSS/MapServer/5/query?where=1%3D1&text=&objectIds=&time=&timeRelation=esriTimeRelationOverlaps"
    "&geometry=&geometryType=esriGeometryPoint&inSR=&spatialRel=esriSpatialRelIntersects&distance=&units=esriSRUnit_Foot&relationParam=&"
    "outFields=shelter_name%2C+address_1%2C+city%2C+state%2C+zip%2C+org_organization_name"
    "%2C+org_address%2C+org_city%2C+org_state%2C+org_zip%2C+org_hours_of_operation%2C+shelter_status_code%2C+shelter_open_date%2C+shelter_closed_date"
    "&returnGeometry=true&returnTrueCurves=false&maxAllowableOffset=&geometryPrecision=&outSR=4326&havingClause=&returnIdsOnly=false&returnCountOnly=false"
    "&orderByFields=&groupByFieldsForStatistics=&outStatistics=&returnZ=false&returnM=false&gdbVersion=&historicMoment=&returnDistinctValues=false&resultOffset="
    "&resultRecordCount=&returnExtentOnly=false&sqlFormat=none&datumTransformation=&parameterValues=&rangeValues=&quantizationParameters=&uniqueIds="
    "&returnUniqueIdsOnly=false&featureEncoding=esriDefault&f=geojson";

    snprintf(jsonPaths.jsonLink, sizeof(jsonPaths.jsonLink), "%s", link);

    downloadJson(jsonPaths);
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
