// Header for the earthquake.c source code

// Checking if earthquake code has been defined yet
#ifndef EARTHQUAKE
#define EARTHQUAKE // Defining earthquake code

#include <stdio.h>

// Time period of earthquake data
// ALL_HOUR = One hour
// ALL_DAY = One day
// ALL_WEEK = Seven days
// ALL_MONTH = Thirty days
enum DataTime {
    ALL_HOUR,
    ALL_DAY,
    ALL_WEEK,
    ALL_MONTH
};

// What magnitude of earthquakes to include
// SIGNIFICANT = Significant earthquakes or 7.5+
// M4_5 = Magnitude 4.5+
// M2_5 = Magnitude 2.5+
// M1_0 = Magnitude 1.0+
// ALL = All earthquakes
enum DataMagnitude {
    SIGNIFICANT,
    M4_5,
    M2_5,
    M1_0,
    ALL
};

// Important strings for the locations of the JSON files
// jsonFileName = Name of the JSON file on the disk and web
// jsonDiskName = Full path to the JSON file on the disk
// jsonLink = Link to the JSON file to download
typedef struct {
    char jsonFileName[100];
    char jsonDiskName[100];
    char jsonLink[200];
} JsonData;

// All the major important parts of the earthquake data to update when polling
typedef struct
{
    char id[64];
    char place[256];

    double magnitude;

    double latitude;
    double longitude;
    double depth;

} Earthquake;

size_t writeJson(char *data, size_t size, size_t nmemb, void *userdata);

void downloadJson(enum DataTime time, enum DataMagnitude magnitude);

void loadEarthquakes();

void pullEarthquakes();

#endif
