// Header for the earthquake.c source code

// Checking if earthquake code has been defined yet
#ifndef EARTHQUAKE
#define EARTHQUAKE // Defining earthquake code

#include <stdio.h>
#include "cJSON.h"

cJSON *loadJson(char *filePath);

bool checkEarthquakes(cJSON *json);

cJSON *loadEarthquakes(enum DataTime time, enum DataMagnitude magnitude);

void sortingMag(cJSON *json);

void printEarthquakes(cJSON *json);

#endif
