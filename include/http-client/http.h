#pragma once
#include <cstddef>
#include <string>
#include <curl/curl.h>

/*
*
* This file is made to be a wrapper for http request
*
* uses libcurl to make requests, this wraps around that,
* libcurl gets statically linked 
*/

// representaion of http body
struct http_body{
    std::string json;
};

// struct used for curl call back
struct memory{
    char* response;
    size_t size;
};

class http 
{

public:
CURL*       curl;  // CURL session
std::string base_url; // API base url

// init
http();
http(std::string url);
~http();

// http request operations
void send_get(std::string endpoint="", bool fullEndpoint=false);
void send_post(http_body& body, std::string endpoint="", bool fullEndpoint=false);

// data handling
static size_t WriteCallback(char* result, size_t size, size_t nmemb, void* userData);

private:

};