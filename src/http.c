// Code for the HTTP server

#include "http.h"
#include "earthquake.h"
#include "shelters.h"
#include "mongoose.h"
#include <stdio.h>

/**
 * Method for recieving new events for the webserver with mongoose. Taken from mongoose's API: https://mongoose.ws/docs/api/
 *
 * Parameters:
 *      c: Connection associated with the event
 *      ev: Type of event that occurred
 *      ev_data: Additional information about the event
 */
void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
    // New HTTP request received
    if (ev == MG_EV_HTTP_MSG) { // Parsed HTTP request
        // Additional information about what method to run, http headers, and etc.
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;

        // Test URL reply
        if (mg_match(hm->uri, mg_str("/api/hello"), NULL)) {                // REST API call?
            mg_http_reply(c, 200, "", "{%m:%d}\n", MG_ESC("status"), 1);    // Yes. Respond JSON

        } else if (mg_match(hm->uri, mg_str("/api/earthquakes"), NULL)) {
            mg_http_reply(c, 200,
                "",
                "{\"magnitude\":4.5,\"location\":\"Georgia\"}\n"
            );

        // URL reply for the "Refresh Data" Button
        } else if (mg_match(hm->uri, mg_str("/api/refresh-shelters"), NULL)) {
            allShelters shelters = loadShelters();
            cJSON *closeShelters = sortClosestShelters(shelters.openShelters);



        } else  {
            struct mg_http_serve_opts opts = {.root_dir = "./web", .fs = &mg_fs_posix};
            mg_http_serve_dir(c, hm, &opts);  // For all other URLs, Serve static files
        }
    }
}
