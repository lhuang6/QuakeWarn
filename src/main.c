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
    // Mongoose event manager. Holds all connections
    struct mg_mgr mgr;

    // Initialise event manager
    mg_mgr_init(&mgr);

    // Creates an http listener for any events (like clicking)
    mg_http_listen(&mgr, "http://0.0.0.0:8000", ev_handler, NULL);

    // Begins polling for events
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }

    return 0;
}
