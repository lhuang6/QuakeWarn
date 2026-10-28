// Main file
// djgsahhfujgfdjghdsjfhj

#include <stdio.h>
#include "cJSON.h"
#include "earthquake.h"
#include "http.h"
#include "shelters.h"

int main(void)
{
    // Polls earthquakes
    pollEarthquakes(ALL_DAY, ALL);
    return 0;
}
