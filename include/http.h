/**
 * Methods for handling the HTML server. This uses the library mongoose to send and recieve data
 * from the front and backend.
 */

#ifndef HTTP
#define HTTP

#include "mongoose.h"

/**
 * Sends shelter JSON data to JavaScript for the interface to use.
 *
 * Parameters:
 *      c: Connection associated with the event
 *      status: HTTP status code (Usually 200 for success)
 *      json: cJSON object of the shelter data
 */
void sendJson(struct mg_connection *c, int status, cJSON *json);

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
cJSON *parseRequest(struct mg_connection *c, struct mg_http_message *hm);

/**
 * Method for recieving new events for the webserver with mongoose. Use this method when running
 * the mongoose listening and polling loop.
 *
 * API endpoints used for the frontend:
 *      - "/api/hello": Test endpoint that sends a JSON that stores the status
 *      - "/api/earthquakes": Downloads earthquake data and sends the JSON
 *      - "/api/shelters": Downloads shelter data and sends the JSON
 *      - "/api/refresh-earthquakes": Re-downloads shelter data and sends an updated JSON
 *      - "/api/sort-shelter-location": Returns a sorted shelter JSON from closest to farthest distance.
 *                                      Must recieve a JSON that only contains "latitude" and "longitude"
 *      - "/api/sort-earthquake-magnitude": Returns a sorted earthquake JSON from closest to farthest.
 *                                           Must recieve a JSON that only contains "latitude" and "longitude"
 *      - "/api/sort-earthquake-impact": 
 *
 * Parameters:
 *      c: Connection associated with the event
 *      ev: Type of event that occurred
 *      ev_data: Additional information about the event
 */
void ev_handler(struct mg_connection *c, int ev, void *ev_data);

#endif
