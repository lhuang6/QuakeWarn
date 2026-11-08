// Header for http.c

#ifndef HTTP
#define HTTP

#include <stdio.h>
#include <stdbool.h>
#include "cJSON.h"
#include "mongoose.h"
#include <math.h>

void ev_handler(struct mg_connection *c, int ev, void *ev_data);

double haversineFormula(double Userlat, double Userlon, double Shelterlat, double Shelterlon);

// void sortByImpact(cJSON *json, double lat, double lon);

cJSON *sortClosestShelters(cJSON *json, double lat, double lon);

void PrintDistance(cJSON *json);

char choice();
#endif
