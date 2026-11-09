// Code for the HTTP server

#include "http.h"
#include "earthquake.h"
#include "shelters.h"
#include "sorting.h"
#include "mongoose.h"
#include <stdio.h>

/**
 * Sends shelter JSON data to JavaScript for the interface to use.
 *
 * Parameters:
 *      c: Connection associated with the event
 *      status: HTTP status code(Usually 200 for success)
 *      json: cJSON object of the shelter data
 */
void sendJson(struct mg_connection *c, int status, cJSON *json) {

    // Retrieve table of
    cJSON *features = cJSON_GetObjectItem(json, "features");

    // JSON data all in one char
    char *response = cJSON_PrintUnformatted(features);

    // If cJSON was unable to turn into a char
    if (response == NULL) {
        mg_http_reply(c, 500, "", "{\"error\":\"JSON serialization failed\"}\n");
        return;
    }

    // Printing JSON data for JavaScript
    mg_http_reply(c, status,
        "Content-Type: application/json\r\n",
        "%s\n", response
    );

    // Freeing up the memory used for the JSON text
    cJSON_free(response);
}

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
            //To-Do

        // Starting up the Shelter table
        } else if (mg_match(hm->uri, mg_str("/api/shelters"), NULL)) {

            // Loads the open shelters file
            cJSON *closeShelters = loadOpenShelters();

            // Sends JSON data to JavaScript
            sendJson(c, 200, closeShelters);

            // Freeing up memory taken by cJSON object
            cJSON_Delete(closeShelters);

        // URL reply for the "Refresh Data" Button
        } else if (mg_match(hm->uri, mg_str("/api/refresh-shelters"), NULL)) {
            downloadShelters();

            // Loads the open shelters file
            cJSON *closeShelters = loadOpenShelters();

            // Sends JSON data to JavaScript
            sendJson(c, 200, closeShelters);

            // Freeing up memory taken by cJSON object
            cJSON_Delete(closeShelters);

        // Sending a sorted JSON table based on location
        } else if (mg_match(hm->uri, mg_str("/api/sort-shelter-location"), NULL)) {

            // Shelter data as a cJSON object
            cJSON *closeShelters = loadOpenShelters();

            // Latitude and Longitude set by user
            // C recieves as JSON, requested by javascript
            cJSON *request = cJSON_ParseWithLength(
                hm->body.buf,
                hm->body.len
            );

            if (request == NULL) {
                mg_http_reply(c, 400, "", "{\"error\":\"%zu\"}\n", hm->body.len);
                return;
            }

            // Extract the state field
            double latitude = cJSON_GetObjectItem(request, "latitude")->valuedouble;
            double longitude = cJSON_GetObjectItem(request, "longitude")->valuedouble;

            // Pass the state to your C function
            cJSON *sortedShelters = sortClosestShelters(closeShelters, latitude, longitude);

            // Send a response back to JavaScript
            sendJson(c, 200, sortedShelters);

            // Deleting all JSON objects to free up memory
            cJSON_Delete(request);
            cJSON_Delete(closeShelters);
            cJSON_Delete(sortedShelters);

        } else  {

            struct mg_http_serve_opts opts = {.root_dir = "./web", .fs = &mg_fs_posix};
            mg_http_serve_dir(c, hm, &opts);  // For all other URLs, Serve static files
        }
    }
}
