#include "http.h"
#include <cstddef>
#include <cstdlib>
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
    CURLcode result = curl_global_init(CURL_GLOBAL_DEFAULT);
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

        std::cout << "RESPONSE: " << response << std::endl;
        std::cout << "SIZE: " << response.size() << std::endl;
        
        /* always cleanup */
        curl_easy_cleanup(curl);
      }
      curl_global_cleanup();
}

void http::send_post(
    http_body& body, 
    std::string endpoint,
    bool fullEndpoint)
{

}

//==============================================================
// DATA HANDLING
//==============================================================

size_t http::WriteCallback(
    char* result,   /* data from curl */
    size_t size,    /* always 1 */
    size_t nmemb,   /* size of chunk in bytes */
    void* userData)
{
    size_t total = size * nmemb;
    auto* str = static_cast<std::string*>(userData);
    if (str && total)
        str->append(result, total);
    return total;
}
