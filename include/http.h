// Header for the HTTP server

#ifndef HTTP
#define HTTP

#include "mongoose.h"

void ev_handler(struct mg_connection *c, int ev, void *ev_data);

#endif
