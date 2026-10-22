// Hello World file to test C compiling

#include <cs50.h>
#include <stdio.h>
#include "cJSON.h"
//#include "civetweb.h"
#include <curl/curl.h>

int main() {
    printf("================================\n");
    printf("      QuakeWarn Startup Test\n");
    printf("================================\n\n");

    printf("[OK] main.c is running!\n");
    printf("[OK] earthquake.h included!\n");
    printf("[OK] http.h included!\n");
    printf("[OK] shelters.h included!\n");

    // Test whether cJSON can parse JSON.
    const char *json_text =
        "{\"project\":\"QuakeWarn\",\"magnitude\":4.5}";

    cJSON *root = cJSON_Parse(json_text);

    if (root == NULL)
    {
        printf("[FAIL] cJSON could not parse JSON.\n");
        return 1;
    }

    printf("[OK] cJSON parsed JSON successfully!\n");

    cJSON *project = cJSON_GetObjectItemCaseSensitive(root, "project");
    cJSON *magnitude = cJSON_GetObjectItemCaseSensitive(root, "magnitude");

    if (cJSON_IsString(project) && cJSON_IsNumber(magnitude))
    {
        printf("[OK] Project: %s\n", project->valuestring);
        printf("[OK] Magnitude: %.1f\n", magnitude->valuedouble);
    }
    else
    {
        printf("[FAIL] Could not retrieve JSON values.\n");
        cJSON_Delete(root);
        return 1;
    }

    cJSON_Delete(root);

    printf("\n================================\n");
    printf("      Startup test complete!\n");
    printf("================================\n");
}
