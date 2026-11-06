// Main file
// djgsahhfujgfdjghdsjfhj

#include <stdio.h>
#include "cJSON.h"
#include "download.h"
#include "earthquake.h"
#include "http.h"
#include "shelters.h"
#include "mongoose.h"

int main(void)
{
    // (TEMPORARY) Downloading all earthquakes in the past day
    allShelters shelters = loadShelters();

    printShelters(shelters.openShelters, 1);

    /**
    struct mg_mgr mgr;  // Mongoose event manager. Holds all connections
    mg_mgr_init(&mgr);  // Initialise event manager
    mg_http_listen(&mgr, "http://0.0.0.0:8000", ev_handler, NULL); // Localhost at port 8000

    for (;;) {
        mg_mgr_poll(&mgr, 1000);  // Infinite event loop
    } */

    sortClosestShelters(shelters.openShelters, 34.02, -84.19);

    return 0;
}
