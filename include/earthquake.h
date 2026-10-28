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

size_t writeJson(char *data, size_t size, size_t nmemb, void *userdata);

void downloadJson(enum DataTime time, enum DataMagnitude magnitude);

void test();

void SortingFromMag();

#endif
