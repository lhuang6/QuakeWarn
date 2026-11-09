// Main file
// djgsahhfujgfdjghdsjfhj

#include <stdio.h>
#include "cJSON.h"
#include "download.h"
#include "earthquake.h"
#include "http.h"
#include "sorting.h"
#include "shelters.h"
#include "mongoose.h"

int main(void)
{
    // (TEMPORARY) Downloading all earthquakes in the past day

    cJSON *json = loadOpenShelters();

    printf("Main JSON: %p\n", (void *)json);

    cJSON *test = cJSON_Duplicate(json, true);
    printf("Main duplicate: %p\n", (void *)test);
    cJSON_Delete(test);

    cJSON *result = sortClosestShelters(json, 34, -84);

    // printShelters(sortedShelters, 1);

    cJSON_Delete(json);
    cJSON_Delete(result);
    // cJSON_Delete(sortedShelters);

    /*
    struct mg_mgr mgr;  // Mongoose event manager. Holds all connections
    mg_mgr_init(&mgr);  // Initialise event manager
    mg_http_listen(&mgr, "http://0.0.0.0:8000", ev_handler, NULL);

    for (;;) {
        mg_mgr_poll(&mgr, 1000);  // Infinite event loop
    }*/

    return 0;
}
