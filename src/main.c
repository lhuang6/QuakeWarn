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
    cJSON *json = loadEarthquakes(ALL_DAY, ALL);

    PrintDistance(json);

    cJSON_delete(json);
    return 0;
}
