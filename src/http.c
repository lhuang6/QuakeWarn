// HTTP server for connecting C code with HTML

#include "http.h"
#include <stdio.h>
#include <stdbool.h>
#include "cJSON.h"
#include <math.h>
#include "mongoose.h"

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


        if (mg_match(hm->uri, mg_str("/api/hello"), NULL)) {              // REST API call?
            mg_http_reply(c, 200, "", "{%m:%d}\n", MG_ESC("status"), 1);    // Yes. Respond JSON

        } else if (mg_match(hm->uri, mg_str("/api/earthquakes"), NULL)) {
            mg_http_reply(c, 200,
                "",
                "{\"magnitude\":4.5,\"location\":\"Georgia\"}\n"
            );
            
        } else  {
            struct mg_http_serve_opts opts = {.root_dir = "./web", .fs = &mg_fs_posix};
            mg_http_serve_dir(c, hm, &opts);  // For all other URLs, Serve static files
        }


    }
}

/**
 * Computes the Haversine Formula for finding the distance between two points with their latitude and longitude.
 *
 * Parameters:
 *      lat_1: -90 to 90 Degrees latitude for the first point
 *      lon_1: -180 to 180 Degrees latitude for the first point
 *      lat_2: -90 to 90 Degrees latitude for the second point
 *      lon_2: -180 to 180 Degrees latitude for the second point
 *
 * Returns:
 *      double: Distance between the two points in kilometers
 */
double HaversineFormula(double lat_1, double lon_1, double lat_2, double lon_2){
    double radius = 6378.0;

    // To convert the degree to radians for trignomatry
    //M_PI is the Pi to
    lat_1 = lat_1 * M_PI/180;
    lon_1 = lon_1 * M_PI/180;
    lat_2 = lat_2 * M_PI/180;
    lon_2 = lon_2 * M_PI/180;

    // To convert the degree to radians for trignomatry
    double dlat = lat_2 - lat_1;
    double dlon = lon_2 - lon_1;

    // One of the steps to find the a temporary value that measures how separated the two locations are on the Earth's sphere.
    double a = sin(dlat / 2) * sin(dlat / 2) + cos(lat_1) * cos(lat_2) * sin(dlon / 2) * sin(dlon / 2);

    // To find the Angular distance between the two locations
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    // To convert distance into KM
    return radius * c;
}
