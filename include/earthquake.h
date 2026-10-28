// Header for the earthquake.c source code

// Checking if earthquake code has been defined yet
#ifndef EARTHQUAKE
#define EARTHQUAKE // Defining earthquake code

#include <stdio.h>

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
    char jsonLink[200];     // Link to the JSON file to download
} JsonData;

size_t writeJson(char *data, size_t size, size_t nmemb, void *userdata);

void downloadJson(enum DataTime time, enum DataMagnitude magnitude);

void loadEarthquakes(enum DataTime time, enum DataMagnitude magnitude);

void pollEarthquakes(enum DataTime time, enum DataMagnitude magnitude);

#endif
