// Main file
// djgsahhfujgfdjghdsjfhj

#include <stdio.h>
#include "cJSON.h"
#include "earthquake.h"
#include "http.h"
#include "shelters.h"

int main(void)
{
    // (TEMPORARY) Downloading all earthquakes in the past day
    cJSON *json = loadEarthquakes(ALL_DAY, ALL);
    sortingMag(json);
    printEarthquakes(json);

    return 0;
}
