// Header for the HTTP server

#ifndef HTTP
#define HTTP

#include "mongoose.h"

void sendJson(struct mg_connection *c, int status, cJSON *json);

cJSON *parseRequest(struct mg_connection *c, struct mg_http_message *hm);

void ev_handler(struct mg_connection *c, int ev, void *ev_data);

#endif
