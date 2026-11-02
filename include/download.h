// Header for the download.c source code

// Checking if download code has been defined yet
#ifndef DOWNLOAD
#define DOWNLOAD // Defining download code

#include <stdio.h>
#include "cJSON.h"

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
    char *jsonFileName; // Name of the JSON file on the disk and web
    char *jsonDiskName; // Full path to the JSON file on the disk
    char *jsonLink;     // Link to the JSON file to download
} JsonData;

JsonData enumToLink(enum DataTime time, enum DataMagnitude magnitude);

size_t writeJson(char *data, size_t size, size_t nmemb, void *userdata);

void downloadJson(JsonData jsonPaths);

void downloadEarthquakes(enum DataTime time, enum DataMagnitude magnitude);

void poll(enum DataTime time, enum DataMagnitude magnitude);

#endif
