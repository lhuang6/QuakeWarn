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

    // Test whether cJSON can parse JSON.
    const char *json_text =
        "{\"project\":\"QuakeWarn\",\"magnitude\":4.5}";

    cJSON *root = cJSON_Parse(json_text);

    // If the parsed result returns nothing
    if (root == NULL)
    {
        printf("[FAIL] cJSON could not parse JSON.\n");
        return 1;
    } else {
        printf("[OK] cJSON parsed JSON successfully!\n");
    }

    // Checking if the keys correspond to the right values
    cJSON *project = cJSON_GetObjectItemCaseSensitive(root, "project");
    cJSON *magnitude = cJSON_GetObjectItemCaseSensitive(root, "magnitude");

    // Checking if JSON could read through the keys and values correctly
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

    // Delete all the allocated memory for the test JSON object
    cJSON_Delete(root);

    // Trying to initialize libcurl globally
    // (Trying to start libcurl and everything it needs)
    CURLcode result = curl_global_init(CURL_GLOBAL_DEFAULT); // Initializing libcurl

    // If the curl code is ok or not
    if(result == CURLE_OK) {
        printf("[OK] Curl global initialization\n");
    } else {
        printf("[FAIL] Curl unable to initialize\n");
    }

    // Cleaning up after initializing
    curl_global_cleanup();

    printf("----TESTING ALL GEOJSON FILES----");

    

    printf("\n================================\n");
    printf("      Startup test complete!\n");
    printf("================================\n");
}
