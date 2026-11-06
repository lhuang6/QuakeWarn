// Header for http.c

#ifndef HTTP
#define HTTP

#include <stdio.h>
#include <stdbool.h>
#include "cJSON.h"
#include "mongoose.h"

void ev_handler(struct mg_connection *c, int ev, void *ev_data);

double HaversineFormula(double Userlat, double Userlon, double Shelterlat, double Shelterlon);

#endif
