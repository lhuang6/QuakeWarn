// Header for http.c

#ifndef SORTING
#define SORTING

#include <stdio.h>
#include <stdbool.h>
#include "cJSON.h"
#include "mongoose.h"
#include <math.h>

double haversineFormula(double lat_1, double lon_1, double lat_2, double lon_2);

cJSON *sortClosestShelters(cJSON *json, double lat, double lon);

cJSON *sortingImpact(cJSON *json, double latitude, double longitude);

cJSON *sortingDistance(cJSON *json, double latitude, double longitude);

char choice();

void PrintDistance(cJSON *json);

#endif
