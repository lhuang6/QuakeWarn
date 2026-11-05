// Header for http.c

#ifndef HTTP
#define HTTP

#include <stdio.h>
#include <stdbool.h>
#include "cJSON.h"

double HaversineFormula(double Userlat, double Userlon, double Shelterlat, double Shelterlon);

void PrintDistance(cJSON *json);
#endif
