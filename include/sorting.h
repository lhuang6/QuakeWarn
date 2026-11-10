// Header for http.c

#ifndef SORTING
#define SORTING

#include <stdio.h>
#include <stdbool.h>
#include "cJSON.h"
#include "mongoose.h"
#include <math.h>

double haversineFormula(double lat_1, double lon_1, double lat_2, double lon_2);

void addDistances(cJSON *json, double lat, double lon);

cJSON *sortClosestShelters(cJSON *json, double lat, double lon);

cJSON *sortEarthquakeImpact(cJSON *json, double latitude, double longitude);

cJSON *sortEarthquakeDistance(cJSON *json, double latitude, double longitude);

char choice();

void PrintDistance(cJSON *json);

#endif
