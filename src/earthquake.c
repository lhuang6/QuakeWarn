// Reading and sorting earthquake data from USGS

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "earthquake.h"
#include "cJSON.h"
#include <stdlib.h>
#include <unistd.h>

/**
 * Converts the time-range enum and the minimum magnitude into the JsonData struct that
 * stores the links and the paths to a JSON file.
 *
 * Parameters:
 *     time: Time period of the earthquakes, of datatype enum DataTime
 *          - ALL_HOUR = One hour
 *          - ALL_DAY = One day
 *          - ALL_WEEK = Seven days
 *          - ALL_MONTH = Thirty days
 *     magnitude: Minimum magnitude included in the data
 *          - SIGNIFICANT = Significant earthquakes or 7.5+
 *          - M4_5 = Magnitude 4.5+
 *          - M2_5 = Magnitude 2.5+
 *          - M1_0 = Magnitude 1.0+
 *          - ALL = All earthquakes
 *
 * Returns:
 *      data: JsonData struct
 *          - jsonFileName = Name of the JSON file on the disk and web
            - jsonDiskName = Full path to the JSON file on the disk
            - jsonLink = Link to the JSON file to download
 */
JsonData enumToLink(enum DataTime time, enum DataMagnitude magnitude) {
    // Characters used in the GeoJSON link based on time-period and minimum magnitude
    char const *timeLink;   // Characters used for the time-period
    char const *magLink;    // Characters used for the minimum magnitude

    // Defining the characters used for the time-period
    // Defines for each enum DataTime
    switch(time) {
        case ALL_HOUR:
            timeLink = "hour";
            break;
        case ALL_DAY:
            timeLink = "day";
            break;
        case ALL_WEEK:
            timeLink = "week";
            break;
        case ALL_MONTH:
            timeLink = "month";
            break;
        // Defaults to one day
        default:
            timeLink = "day";
    }

    // Defining the characters used for the minimum magnitude
    // Defines for each enum DataMagnitude
    switch(magnitude) {
        case SIGNIFICANT:
            magLink = "significant";
            break;
        case M4_5:
            magLink = "4.5";
            break;
        case M2_5:
            magLink = "2.5";
            break;
        case M1_0:
            magLink = "1.0";
            break;
        case ALL:
            magLink = "all";
            break;
        // Defaults to all magnitude earthquakes
        default:
            magLink = "all";
            break;
    }

    // Putting all links and paths inside the JsonData struct
    JsonData data;

    // Name of the file on the USGS website
    snprintf(data.jsonFileName, sizeof(data.jsonFileName), "%s_%s.geojson", magLink, timeLink);

    // Where the data is going to be downloaded in
    snprintf(data.jsonDiskName, sizeof(data.jsonDiskName), "data/%s", data.jsonFileName);

    // Full link to the GeoJSON data
    snprintf(data.jsonLink, sizeof(data.jsonLink), "https://earthquake.usgs.gov/earthquakes/feed/v1.0/summary/%s", data.jsonFileName);

    return data;
}

/**
 * Used by libcurl to write data onto the disk.
 *
 * Parameters:
 *      data: A pointer for all of the data to download
 *      size: How much memory each item to download is
 *      nmemb: How many items there are to download
 *      userdata: Pointer of additional user specified data; in this case, it's the saved file.
 * Returns:
 *      written: Total size of everything written with fwrite()
 */
size_t writeJson(char *data, size_t size, size_t nmemb, void *userdata) {
    size_t written = fwrite(data, size, nmemb, (FILE *)userdata);
    return written;
}

/**
 * Method for downloading and updating the json file on the disk. Run it again to update it to the
 * most recent time.
 *
 * Parameters:
 *      time: Time period of the earthquakes, of datatype enum DataTime
 *          - ALL_HOUR = One hour
 *          - ALL_DAY = One day
 *          - ALL_WEEK = Seven days
 *          - ALL_MONTH = Thirty days
 *      magnitude: Minimum magnitude included in the data
 *          - SIGNIFICANT = Significant earthquakes or 7.5+
 *          - M4_5 = Magnitude 4.5+
 *          - M2_5 = Magnitude 2.5+
 *          - M1_0 = Magnitude 1.0+
 *          - ALL = All earthquakes
 */
void downloadJson(enum DataTime time, enum DataMagnitude magnitude) {
    // JsonData struct that stores the download link and path to JSON file
    JsonData jsonPaths = enumToLink(time, magnitude);

    // Creating or replacing file
    // Able to replace the file for updating it to a more recent time
    FILE *jsonDiskFile = fopen(jsonPaths.jsonDiskName, "w");

    // Initializing libcurl globally to be used
    // Using default parameters - "CURL_GLOBAL_DEFAULT"
    CURLcode result; // Code for the status of libcurl methods
    result = curl_global_init(CURL_GLOBAL_DEFAULT);

    // Printing out the status of libcurl
    if (result == CURLE_OK) {
        printf("[OK] Curl global initialization\n");
    } else {
        printf("[FAIL] Curl unable to initialize\n");
    }

    // "Easy initializing" of curl
    // Basically creating an instance for libcurl to download data
    // Easy because it's the easy way to get the data
    CURL *curl = curl_easy_init();

    // Setting options for download
    curl_easy_setopt(curl, CURLOPT_URL, jsonPaths.jsonLink);              // Setting the URL to the JSON
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeJson);   // Function for writing data onto the disk
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, jsonDiskFile);    // Specifying where to write the data

    // Downloading the data from USGS
    result = curl_easy_perform(curl);

    if (result == CURLE_OK) {
        printf("[OK] Downloaded data from:\n%s\n", jsonPaths.jsonLink);
    } else {
        printf("[OK] Unable to download data from:\n%s\n", jsonPaths.jsonLink);
    }

    // Closing the file
    fclose(jsonDiskFile);

    // Cleaning up curl globally and the easy instance
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

void SortingFromMag(enum DataTime time, enum DataMagnitude magnitude){
    // Turning the DataTime and DataMagnitude enum into file paths and links to use
    JsonData data = enumToLink(time, magnitude);

    //To open the FILE(Used to work and sort w files) named all_day.geojson and called it twodays
    FILE *twodays = fopen(data.jsonDiskName,"r");
    if (twodays == NULL) {  //To see if the geojson filder can open or not, if there is nothing then its null
        printf("Error: Unable to open the file.\n");
        return;
    }

    //I have an array that can hold 500000 char because of how long the geojson file is
    char line[500000];

    //using size_t(which is the store size), fread reads and stores everything
    //So 'line' is where it is put, the '1' is the byte for each char,
    //'sizeof(line)-1' is just the 500000 that is kept remembering if i change the char
    //'twodays' is just the veriable i called .geojson
    size_t bytesRead = fread(line, 1, sizeof(line)-1, twodays);

    //so the '-1' i wrote in the above line is to live room for '\0'
    //because it mandatory to know where it ends
    line[bytesRead] = '\0';
    //We are making the variable 'convertedLine' store the Parse(char line)
    cJSON *convertedLine = cJSON_Parse(line);//it is used to convert the line char into readable json sturcture
    if (convertedLine == NULL){ //To see if convertedLine is working or there is nothing(null)
        printf("Parse failed\n");
        return;
    }
    //To open the feature that are was in parse veriable convertedLine, called it features
    cJSON *features = cJSON_GetObjectItemCaseSensitive(convertedLine, "features");
    //The struct named Magnitude that is going to store 500 mag to sort them from least to most
    Magnitude earthquakes[500];
    int count = 0;  //This is to see how many mag were stored but for now its 0
    //this is used 'cJSON_GetArraySize' to see, how many fetures are in JSON, in numbers/int
    for (int i = 0; i < cJSON_GetArraySize(features); i++)
    {
        //IT finds one feature at an time from 'GetArrayItem'
        cJSON *feature = cJSON_GetArrayItem(features, i);
        //To get the ojbect/item from feature named properties
        cJSON *properties = cJSON_GetObjectItemCaseSensitive(feature, "properties");
        //To get the ojbect/item from feature named properties
        cJSON *magnitude = cJSON_GetObjectItemCaseSensitive(properties, "mag");
        //The count which is 0 for now is starting to get storen by earthquakes
        //.mag was used because of it being named at struct
        earthquakes[count].mag = magnitude->valuedouble;
        //.data was used to store all the features from Json
        earthquakes[count].data = feature;

        count++;//it adds by one by th eend of the loop the rreapet the process
    }
    //To make the earthquake which now have the data to organize from least to most
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (earthquakes[j].mag < earthquakes[i].mag)
            {
                //this is where sorting happenns whne the 'for loop' looks through the json
                Magnitude arrange = earthquakes[i];
                earthquakes[i] = earthquakes[j];
                earthquakes[j] = arrange;
            }
        }
    }
    for (int i = 0; i < count; i++)//as the i increases it becomes more in order
    {

    // Convert the cJSON earthquake object into a printable string
    char *text = cJSON_Print(earthquakes[i].data);

    // Print the entire earthquake record
    printf("%s\n", text);

    // Free the memory created by cJSON_Print()
    free(text);

    }

}

/**
 * Loads the earthquakes data from the earthquake files and prints out all of the data.
 * This also serves as a check for the data's readability or not.
 *
 * Parameters:
 *     time: Time period of the earthquakes, of datatype enum DataTime
 *          - ALL_HOUR = One hour
 *          - ALL_DAY = One day
 *          - ALL_WEEK = Seven days
 *          - ALL_MONTH = Thirty days
 *     magnitude: Minimum magnitude included in the data
 *          - SIGNIFICANT = Significant earthquakes or 7.5+
 *          - M4_5 = Magnitude 4.5+
 *          - M2_5 = Magnitude 2.5+
 *          - M1_0 = Magnitude 1.0+
 *          - ALL = All earthquakes
*/
void loadEarthquakes(enum DataTime time, enum DataMagnitude magnitude) {
    // JsonData struct that stores the download link and path to JSON file
    JsonData jsonPaths = enumToLink(time, magnitude);

    // Opens the file downloaded from USGS
    FILE *file = fopen(jsonPaths.jsonDiskName, "r");

    // Check if the file opening fails
    if (file == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    // Navigates to the end of the file and
    // determines the size of it in bytes
    // and navigates back to the beginning
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    // Allocates memory to fit the size of the file
    char *buffer = malloc(size + 1);

    // Stores the file contents in memory and appends
    // the string terminator
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    // Closes file
    fclose(file);

    // Parses text into JSON file
    cJSON *json = cJSON_Parse(buffer);

    // Free the memory
    free(buffer);

    // Checks if JSON parsing passed
    if (json == NULL) {
        printf("JSON parse failed.\n");
        return;
    }

    // Fetches earthquake features list
    cJSON *features = cJSON_GetObjectItem(json, "features");

    // Counts how many earthquakes there are
    int count = cJSON_GetArraySize(features);

    printf("Found %d earthquakes.\n", count);

    // Loops through all the different earthquakes and stores their specific details
    for (int i = 0; i < count; i++) {

        // Creates many different pointers relating to
        // features of the different earthquakes such as
        // the properties, place, and magnitude
        cJSON *feature = cJSON_GetArrayItem(features, i);
        cJSON *properties = cJSON_GetObjectItem(feature, "properties");
        cJSON *place = cJSON_GetObjectItem(properties, "place");
        cJSON *mag = cJSON_GetObjectItem(properties, "mag");

        // Processes all information and spits it out
        // --------------- Example -----------------
        // Magnitude 2.3 at Alaska
        // Magnitude 1.7 at Nevada
        // Magnitude 4.1 at Japan
        printf("Magnitude %.1f at %s\n", mag->valuedouble, place->valuestring);
    }

    // Delete the file at the end
    cJSON_Delete(json);
}

/**
 * Updates the JSON data every one minute for new earthquakes
 *
 * Parameters:
 *     time: Time period of the earthquakes, of datatype enum DataTime
 *          - ALL_HOUR = One hour
 *          - ALL_DAY = One day
 *          - ALL_WEEK = Seven days
 *          - ALL_MONTH = Thirty days
 *     magnitude: Minimum magnitude included in the data
 *          - SIGNIFICANT = Significant earthquakes or 7.5+
 *          - M4_5 = Magnitude 4.5+
 *          - M2_5 = Magnitude 2.5+
 *          - M1_0 = Magnitude 1.0+
 *          - ALL = All earthquakes
*/
void pollEarthquakes(enum DataTime time, enum DataMagnitude magnitude){

    // Fix later to stop once program is turned off
    while (1){
        printf("Downloading new earthquake data...\n");

        // Downloads the most recent CSV file of earthquake data
        downloadJson(time, magnitude);

        printf("Loading earthquake data...\n");
        loadEarthquakes(time, magnitude);

        // Waits 60 seconds to poll again
        sleep(60);
    }
}

void parseJson() {
    FILE *fp = fopen("data/all_day.geojson", "r");
    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
    }

    // Variable where all of the read data goes in
    char buffer[10000];
    // Buffer to put it in, size of each element, amount of elements, and file
    fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    cJSON *json = cJSON_Parse(buffer);
    cJSON *features = cJSON_GetObjectItem(json, "features");

    cJSON *feature = cJSON_GetArrayItem(features, 1);
    cJSON *properties = cJSON_GetObjectItem(feature, "properties");
    cJSON *mag = cJSON_GetObjectItem(properties, "mag");

    printf("%s\n", mag->valuestring);

    cJSON_Delete(json);
}

