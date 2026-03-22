#include "http.h"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>

//==============================================================
// TASKS
//==============================================================
/**

- finish get request method with files and test it
- implement post request and test them
*/


//==============================================================
// INITS
//==============================================================
http::http()
{
    http("");
}

http::http(
    std::string url)
{
    // test initialization of curl session
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK){
        std::cout << "[ERROR] Couldnt initialize curl session" << std::endl;
        exit((int)res);
    }
    // empty base_url
    base_url = url;
}

http::~http(){}

//==============================================================
// HTTP OPERATIONS
//==============================================================
/**
*
*   This method will send a POST request to a given endpoint
*   @improvements: have it return json string so user can custom parse response
*/
void http::send_post(
    http_body& body, 
    std::string endpoint, 
    bool fullEndpoint)
{
    // invalid full endpoint url
    if (fullEndpoint && endpoint.size() == 0){
        return;
    }

    std::string url;

    // prefix with base_url if not given absolute endpoint URL
    if (!fullEndpoint){
        url.append(this->base_url);
    }

    // endpoint URL
    url.append(endpoint);

    // start CURL session
    CURLcode result = curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (result != CURLE_OK){
        std::cout << "[ERROR] CURL session could not be established" << std::endl;
        std::cout << "[ERROR] result=" << result << std::endl;
        exit((int)result);
    }

    // if libcurl failed to make session
    if(!curl){
        std::cout << "[ERROR] CURL session could not be established" << std::endl;
        exit(1);
    }

    // string variable that will contain request response
    std::string response;

    // set up CURL options
    curl_easy_setopt(curl, CURLOPT_URL, url.data());
    // set method to post
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    // set request headers
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER , headers);
    // set call back function to handle data response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    // set post request body
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.json.c_str());

    // perform request
    result = curl_easy_perform(curl);

    // clean up curl client
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    // return response string
    // debug prints
    std::cout << "RESPONSE: " << response << std::endl;
    std::cout << "SIZE: " << response.size() << std::endl;
}


/**
*
*   This method will send a GET request to a given endpoint
*   @improvements: have it return json string so user can custom parse response
*/
void http::send_get(
    std::string endpoint,
    bool fullEndpoint)
{
    std::string response;
    std::string url="";

    // fix endpoint url
    if (fullEndpoint) url += endpoint;
    else { url += base_url; url += endpoint;}

    // reinitialize the session
    CURLcode result = curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl) {
        // set http request options
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        /* Perform the request, result gets the return code */
        result = curl_easy_perform(curl);
        /* Check for errors */
        if(result != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(result));

        // debug prints
        std::cout << "RESPONSE: " << response << std::endl;
        std::cout << "SIZE: " << response.size() << std::endl;
        
        /* always cleanup */
        curl_easy_cleanup(curl);
      }
      curl_global_cleanup();

      
}



//==============================================================
// DATA HANDLING CALLBACK
//==============================================================

size_t http::WriteCallback(
    char* result,   /* data from curl */
    size_t size,    /* always 1 */
    size_t nmemb,   /* size of chunk in bytes */
    void* userData) /* response pointer */
{
    size_t total = size * nmemb;
    auto* str = static_cast<std::string*>(userData);
    if (str && total)
        str->append(result, total);
    return total;
}

/**
*
* This method will turn the data map into a JSON string
* and set the json field with the result
*/
void http_body::stringify(){
    // default with opening braces
    std::string res = "{";

    // loop through each pair in hash map
    for (auto &pair : this->fieldMap){
        res.append("\"" + pair.first +"\"");
        res.append(":");
        res.append("\"" + pair.second +"\"");
        res.append(",");
    }
    // remove trailing comma
    if (res.back() == ','){
        res.pop_back();
    }

    // end with closing braces
    res.append("}");
    // convert to RAW string
    this->json = res;
    std::cout << "JSON: " << res << std::endl;
}