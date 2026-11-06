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


        if (mg_match(hm->uri, mg_str("/api/hello"), NULL)) {                // REST API call?
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
double haversineFormula(double lat_1, double lon_1, double lat_2, double lon_2){
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
/*
void sortByImpact(cJSON *json, double lat, double lon) {

}*/

void sortClosestShelters(cJSON *json, double lat, double lon) {
    // Fetches earthquake features list
    cJSON *features = cJSON_GetObjectItem(json, "features");

    // Counts how many earthquakes there are
    int count = cJSON_GetArraySize(features);
    double distances[count];

    // Creating a list of distances which index cooresponds to the shelter in the JSON
    for (int i = 0; i < count; i++) {
        // Each shelter
        cJSON *feature = cJSON_GetArrayItem(features, i);

        // Array for the latitude and longitude
        cJSON *coords = cJSON_GetObjectItem(cJSON_GetObjectItem(feature, "geometry"), "coordinates");

        // Latitude and Longitude from the array
        double shelterLon = cJSON_GetArrayItem(coords, 0)->valuedouble;
        double shelterLat = cJSON_GetArrayItem(coords, 1)->valuedouble;

        // Distance from haversine formula
        double distance = haversineFormula(lat, lon, shelterLat, shelterLon);

        // Adding the distance to the shelter into a new list
        distances[i] = distance;
    }

    // Performing an Insertion Sort:
    // 1. Start with the second element as the first element is assumed to be sorted.
    // 2. Compare the second element with the first if the second is smaller then swap them.
    // 3. Move to the third element, compare it with the first two, and put it in its correct position
    // 4. Repeat until the entire array is sorted.
    for (int i = 1; i < count; i++) {


        // Grabbing the magnitude for each member on the right
        cJSON *feature = cJSON_GetArrayItem(features, i);
        cJSON *properties = cJSON_GetObjectItem(feature, "properties");
        cJSON *jsonMag = cJSON_GetObjectItem(properties, "mag");
        float magnitude = jsonMag->valuedouble;

        // Compare with elements to the left
        int j = i - 1;
        while (j >= 0) {
            // Grabbing the magnitude for each element on the left
            cJSON *lastFeature = cJSON_GetArrayItem(features, j);
            cJSON *lastProperties = cJSON_GetObjectItem(lastFeature, "properties");
            cJSON *lastJsonMag = cJSON_GetObjectItem(lastProperties, "mag");
            double lastMagnitude = lastJsonMag->valuedouble;

            // Stop when the correct position is found
            if (lastMagnitude <= magnitude) {
                break;
            }

            // Going through each element towards the left
            j--;
        }

        // Move the current element into its sorted position
        if (j != i - 1) {
            // Detatching the element and inserting it back into the swapped position
            cJSON *item = cJSON_DetachItemFromArray(features, i);
            cJSON_InsertItemInArray(features, j + 1, item);
        }
    }
}

