/**
 * All the methods for downloading earthquake or shelter GeoJSON
 * into the project. All of the GeoJSON is downloaded in /data/
 *
 */

#ifndef DOWNLOAD
#define DOWNLOAD

#include "cJSON.h"
#include <stdlib.h>

/**
 * Time period of earthquake data to download
 */
enum DataTime {
    ALL_HOUR,   // One hour
    ALL_DAY,    // One day
    ALL_WEEK,   // Seven days
    ALL_MONTH   // Thirty days
};


/**
 * Specifies what magnitude of earthquakes to include to download.
 */
enum DataMagnitude {
    SIGNIFICANT,    // SIGNIFICANT = Significant earthquakes or 7.5+
    M4_5,           // M4_5 = Magnitude 4.5+
    M2_5,           // M2_5 = Magnitude 2.5+
    M1_0,           // M1_0 = Magnitude 1.0+
    ALL             // ALL = All earthquakes
};

/**
 * Important strings for the locations of the JSON files
 */
typedef struct {
    char jsonFileName[100]; // Name of the JSON file on the disk and web
    char jsonDiskName[100]; // Full path to the JSON file on the disk
    char jsonLink[1100];    // Link to the JSON file to download
} JsonData;

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
JsonData enumToLink(enum DataTime time, enum DataMagnitude magnitude);

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
size_t writeJson(char *data, size_t size, size_t nmemb, void *userdata);

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
void downloadJson(JsonData jsonPaths);

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
void downloadEarthquakes(enum DataTime time, enum DataMagnitude magnitude);

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
void downloadShelters();

/**
 * Loads any earthquake GeoJSON file.
 *
 * Parameters:
 *     filePath: A pointer to the path of the file on the disk
 *
 * Returns:
 *     json: A pointer to the cJSON object for the USGS GeoJSON data
*/
cJSON *loadJson(char *filePath);

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
void pollData(enum DataTime time, enum DataMagnitude magnitude);

#endif
