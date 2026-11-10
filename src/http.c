// Code for the HTTP server

#include "http.h"
#include "earthquake.h"
#include "shelters.h"
#include "sorting.h"
#include "mongoose.h"
#include "cJSON.h"
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
 * Parses a request made by the frontend JavaScript into a cJSON object to use.
 *
 * Parameters:
 *      c: Connection associated with the event
 *      hm: HTTP request
 *
 * Returns:
 *      request: Parsed cJSON object to be used
 */
cJSON *parseRequest(struct mg_connection *c, struct mg_http_message *hm) {
    // Checking if the URL recieved is of type "POST"
    // Earlier debugging error
    if (!mg_match(hm->method, mg_str("POST"), NULL)) {
        // Replying with an error message
        mg_http_reply(c, 405,
            "Content-Type: application/json\r\n",
            "{\"error\":\"Expected POST\",\"actual_method\":\"%.*s\"}",
            (int) hm->method.len,
            hm->method.buf
        );
        return NULL;
    }

    // Latitude and Longitude set by user
    // C recieves as JSON, requested by javascript
    cJSON *request = cJSON_ParseWithLength(
        hm->body.buf,
        hm->body.len
    );

    // Check whether JSON parsing succeeded
    if (request == NULL) {
        // Replying with an error message
        mg_http_reply(c, 400,
            "Content-Type: application/json\r\n",
            "{\"error\":\"JSON parsing failed\",\"body_length\":%lu}",
            (unsigned long) hm->body.len
        );
    }

    return request;
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


        // Sending all earthquake JSON data
        } else if (mg_match(hm->uri, mg_str("/api/earthquakes"), NULL)) {

            // Downloading and loading earthquake data
            downloadEarthquakes(ALL_DAY, ALL);
            cJSON *earthquakes = loadEarthquakes(ALL_DAY, ALL);

            // Sending the JSON to JavaScript
            sendJson(c, 200, earthquakes);

            // Deleting earthquake JSON
            cJSON_Delete(earthquakes);


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


        // Sending a sorted shelter JSON based on location
        } else if (mg_match(hm->uri, mg_str("/api/sort-shelter-location"), NULL)) {

            // Parsing the request for latitude and longitude to be used
            cJSON *request = parseRequest(c, hm);
            if (request == NULL) {
                // Not continuing if the parse failed
                return;
            }

            // Shelter data as a cJSON object
            cJSON *closeShelters = loadOpenShelters();

            // Extract the coordinates
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


        // Sending a sorted JSON table based on location
        } else if (mg_match(hm->uri, mg_str("/api/sort-earthquake-location"), NULL)) {
            // Parsing the request for latitude and longitude to be used
            cJSON *request = parseRequest(c, hm);
            if (request == NULL) {
                // Not continuing if the parse failed
                return;
            }

            // Extract the coordinates
            double latitude = cJSON_GetObjectItem(request, "latitude")->valuedouble;
            double longitude = cJSON_GetObjectItem(request, "longitude")->valuedouble;

            cJSON *earthquakes = loadEarthquakes(ALL_DAY, ALL);
            cJSON *sortedShelters = sortClosestShelters(closeShelters, latitude, longitude)

        } else  {

            struct mg_http_serve_opts opts = {.root_dir = "./web", .fs = &mg_fs_posix};
            mg_http_serve_dir(c, hm, &opts);  // For all other URLs, Serve static files
        }
    }
}
