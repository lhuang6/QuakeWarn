// Main file
// djgsahhfujgfdjghdsjfhj

#include <stdio.h>
#include "cJSON.h"
#include "download.h"
#include "earthquake.h"
#include "http.h"
#include "shelters.h"

int main(void)
{
    // (TEMPORARY) Downloading all earthquakes in the past day
    allShelters shelters = loadShelters();

    printShelters(shelters.openShelters, 1);

    cJSON *json = loadEarthquakes(ALL_DAY, ALL);

    printEarthquakes(json, 0.1);
    return 0;
}
