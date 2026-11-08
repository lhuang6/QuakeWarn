// Header for http.c

#ifndef HTTP
#define HTTP

#include <stdio.h>
#include <stdbool.h>
#include "cJSON.h"
#include "mongoose.h"
#include <math.h>

double HaversineFormula(double Userlat, double Userlon, double Shelterlat, double Shelterlon);

cJSON *sortingImpact(cJSON *json, double latitude, double longitude);

cJSON *sortingDistance(cJSON *json, double latitude, double longitude);

char choice();

void PrintDistance(cJSON *json);

void ev_handler(struct mg_connection *c, int ev, void *ev_data);

#endif
