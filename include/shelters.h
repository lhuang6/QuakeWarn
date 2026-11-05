// Header for the shelter.c source code

#ifndef SHELTERS
#define SHELTERS

#include <stdio.h>
#include "cJSON.h"

bool checkShelters(cJSON *json);

cJSON *loadShelters();

void printShelters(cJSON *json, double percent)

#endif
