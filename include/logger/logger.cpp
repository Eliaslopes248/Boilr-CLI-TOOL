#include "logger.h"
#include <cctype>
#include <cstdlib>
#include <stdlib.h>

/**
*
* This file implements Logger class
*
*/

/**
* @desc: This method will init a logger to its default values
* @param None
* @return bool
*/
Logger::Logger(){
    this->debugModeFlag = inDebugMode();
}

/**
* @desc: This method will init a logger to its default values
* @param None
* @return bool
*/
Logger::Logger(char* logFilePath){
    // set debug mode flag
    this->debugModeFlag = inDebugMode();

    // if a new log file path was given
    if (logFilePath && logFilePath[0] != '\0'){
        // convert to string and set as logfilepath
        this->logFilePath = static_cast<std::string>(logFilePath);
    }
}

/**
* @desc: This method will init a logger with log level
*        modifiers and a new filePath for log file
* @param None
* @return bool
*/
Logger::Logger(
    char*     logFilePath,
    LOG_LEVEL minFileLevel,
    LOG_LEVEL maxFileLevel,
    LOG_LEVEL minStdoLevel,
    LOG_LEVEL maxStdoLevel)
{
    // if a new log file path was given
    if (logFilePath && logFilePath[0] != '\0'){
        // convert to string and set as logfilepath
        this->logFilePath = static_cast<std::string>(logFilePath);
    }

    // file level modifiers
    this->minFileLevel = minFileLevel;
    this->maxFileLevel = maxFileLevel;

    // stdout level modifiers
    this->minStdoLevel = minStdoLevel;
    this->maxStdoLevel = maxStdoLevel;

    // set debug mode flag
    this->debugModeFlag = inDebugMode();
}

/**
* @desc: This method will init a logger with log level
*        modifiers and a new filePath for log file
* @param None
* @return None
*/
Logger::Logger(
    char* logFilePath,
    int minFileLevel,
    int maxFileLevel,
    int minStdoLevel,
    int maxStdoLevel)
{
    // if a new log file path was given
    if (logFilePath && logFilePath[0] != '\0'){
        // convert to string and set as logfilepath
        this->logFilePath = static_cast<std::string>(logFilePath);
    }

    // min/max log level that will be outputted to the file
    this->minFileLevel = LOG_LEVEL(minFileLevel);
    this->maxFileLevel = LOG_LEVEL(maxFileLevel);

    // min/max to be outputed to standard output
    this->minStdoLevel = LOG_LEVEL(minStdoLevel);
    this->maxStdoLevel = LOG_LEVEL(maxStdoLevel);

    // set debug mode flag
    this->debugModeFlag = inDebugMode();
}

/**
* @desc: This method will destroy a logger from memory
* @return bool
*/
Logger::~Logger(){}


/**
* @desc: This method will check system variable to see if running in debug mode
* @param None
* @return bool
*/
bool Logger::inDebugMode()
{
    // get env varibales from system
    const char* value = std::getenv("BOILR_DEBUG_MODE");
    if (!value || value == nullptr) return false;

    // if it is set to true then return true
    std::string strVal(value);

    // convert to lowercase
    for(char& c: strVal){
        c = static_cast<char>(std::tolower(c));
    }

    // see if its set to true
    if (strVal == "true"){
        return true;
    }
   
    return false;
}



/**
* @desc: This method will log a message to the log file and or stdout
* @param None
* @return void
*/
void Logger::invoke(
    LOG_LEVEL   lvl,
    std::string message)
{
    try
    {
        handleFileLog(lvl, message);
        handleStdoLog(lvl, message);
    }catch(...)
    {
        std::cout << "[ERROR] when handling log command" << std::endl;
    }
}