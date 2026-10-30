// Hello World file to test C compiling

#include <cs50.h>
#include <stdio.h>
#include "cJSON.h"
#include "earthquake.h"
//#include "civetweb.h"
#include <curl/curl.h>

int main(void) {

    /*
    -----------------
    TESTING LIBRARIES
    -----------------
    */
    printf("================================\n");
    printf("      QuakeWarn Startup Test\n");
    printf("================================\n\n");

    int pass = 0;
    int fail = 0;

    // Test whether cJSON can parse JSON.
    const char *json_text =
        "{\"project\":\"QuakeWarn\",\"magnitude\":4.5}";

    cJSON *root = cJSON_Parse(json_text);

    // If the parsed result returns nothing
    if (root == NULL)
    {
        printf("[FAIL] cJSON could not parse JSON.\n");
        fail++;
    } else {
        printf("[OK] cJSON parsed JSON successfully!\n");
        pass++;
    }
    /*
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
    cJSON_Delete(root);*/

    // Trying to initialize libcurl globally
    // (Trying to start libcurl and everything it needs)
    CURLcode result = curl_global_init(CURL_GLOBAL_DEFAULT); // Initializing libcurl

    // If the curl code is ok or not
    if(result == CURLE_OK) {
        printf("[OK] Curl global initialization\n");
        pass++;
    } else {
        printf("[FAIL] Curl unable to initialize\n");
        fail++;
    }

    // Cleaning up after initializing
    curl_global_cleanup();


    /*
    ---------------
    TESTING GEOJSON
    ---------------
    */
    printf("\n----TESTING ALL GEOJSON FILES----\n");


    // ----------FIRST FILE-------------
    // Testing the first file (Unsorted)
    cJSON *json_1 = loadJson("tests/data/01_valid_unsorted.geojson");

    // Should be able to read an unsorted file
    if (json_1 != NULL) {
        printf("[OK] Able to read a valid, unsorted dataset\n");
        pass++;
    } else {
        fail++;
    }

    // Deleting first file JSON object
    cJSON_Delete(json_1);


    // ----------SECOND FILE-------------
    // Testing second file of array length 0
    cJSON *json_2 = loadJson("tests/data/02_valid_empty.geojson");
    sortingMag(json_2);

    // Grabbing features (each earthquake)
    // Should have nothing in it
    cJSON *features = cJSON_GetObjectItem(json_2, "features");

    // Running sorting method
    sortingMag(json_2);

    // Checking if features is still empty
    if (cJSON_GetArraySize(features) == 0) {
        printf("[OK] Empty array handled correctly\n");
        pass++;
    } else {
        printf("[FAIL] Array should be empty.\n\n");
        fail++;
    }

    cJSON_Delete(json_2);


    // ----------THIRD FILE-------------
    // Testing third file which is missing a column
    cJSON *json_3 = loadJson("tests/data/03_invalid_json.geojson");

    // If the JSON file has incorrect formatting, the method should return NULL
    if (json_3 == NULL) {
        printf("[OK] Able to detect incorrect formatting\n");
        pass++;
    } else {
        printf("[FAIL] Unable to detect incorrect formatting\n");
        fail++;
    }

    cJSON_Delete(json_3);


    // ----------FOURTH FILE-------------
    // Instead of an array storing magnitude, location, etc., there is just a string
    cJSON *json_4 = loadJson("tests/data/04_invalid_structure.geojson");

    // If the JSON file has incorrect formatting, the method should return NULL
    if (json_4 == NULL) {
        printf("[OK] Able to detect incorrect structure\n");
        pass++;
    } else {
        printf("[FAIL] Unable to detect incorrect structure\n");
        fail++;
    }

    cJSON_Delete(json_4);

    // ----------FIFTH FILE-------------
    // The earthquake has a magnitude of null
    cJSON *json_5 = loadJson("tests/data/05_invalid_fields.geojson");
    sortingMag(json_5);
    printEarthquakes(json_5);

    cJSON_Delete(json_5);


    printf("\n================================\n");
    printf("      Startup test complete!\n");
    printf("================================\n");
}
