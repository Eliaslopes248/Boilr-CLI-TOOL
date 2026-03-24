#include "logger.h"
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <streambuf>
#include <string>
#include <cstdlib>

namespace
{
    // Shell expands "~", but std::ofstream / filebuf::open do not — paths must be absolute or cwd-relative.
    std::string expandUserPath(const std::string& path)
    {
        if (path.empty() || path[0] != '~') {
            return path;
        }
        const char* home = nullptr;
    #if defined(_WIN32)
        home = std::getenv("USERPROFILE");
    #else
        home = std::getenv("HOME");
    #endif
        if (!home) {
            return path;
        }
        if (path.size() == 1) {
            return std::string(home);
        }
        if (path[1] == '/') {
            return std::string(home) + path.substr(1);
        }
        // "~otheruser/..." would need getpwnam; leave unchanged
        return path;
    }
}

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
    deleteOldLogFile();
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

    deleteOldLogFile();

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

    deleteOldLogFile();

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

    deleteOldLogFile();

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
        /** FIXME: */
        handleFileLog(lvl, message);
        //handleStdoLog(lvl, message);
    }catch(...)
    {
        std::cout << "[ERROR] when handling log command" << std::endl;
    }
}

/**
* @desc: This method will set the class name to label logs
* @param name
* @return void
*/
void Logger::setClassName(
    std::string name)
{
    if (name.length() == 0) return;
    this->className = name;
}

/**
* @desc: This method will toggle the debug flag
* @param enabled
* @return void
*/
void Logger::setDebugFlag(
    bool enabled)
{
    this->debugModeFlag = enabled;
}

/**
* @desc: This method will toggle the file mode flag
* @param enabled
* @return void
*/
void Logger::setFileModeFlag(
    bool enabled)
{
    this->fileModeFlag = enabled;
}

/**
* @desc: This method will toggle both debug and file mode flags
* @param enabled
* @return void
*/
void Logger::setDebugFileFlags(
    bool dbg, 
    bool fm)
{
    this->debugModeFlag = dbg;
    this->fileModeFlag  = fm;
}

/**
* @desc: This method will delete the pre-existing log file
* @param None
* @return void
*/
void Logger::deleteOldLogFile()
{
    // get resolved path
    const std::string resolvedPath = expandUserPath(this->logFilePath);
    // try to open file with logFilePath which will overwrite if it exists
    std::ofstream file(resolvedPath);
    file.close();
}

/**
* @desc: This method will make a string with the color coded log level
* @param lvl
* @return std::string
*/
std::string Logger::getFormattedLogLevel(
    LOG_LEVEL lvl)
{
    // get string verison of enum name
    std::string LEVEL;
    switch(lvl){
        case INFO:
            LEVEL = "INFO";
            break;
        case DEBUG:
            LEVEL = "DEBUG";
            break;
        case WARNING:
            LEVEL = "WARNING";
            break;
        case ERROR:
            LEVEL = "ERROR";
            break;
        case FATAL:
            LEVEL = "FATAL";
            break;
        default:
            LEVEL = "INFO";
            break;
    }
    // Fixed width so columns align
    constexpr int kLevelFieldWidth = 7;
    std::ostringstream levelField;
    levelField << std::left << std::setw(kLevelFieldWidth) << LEVEL;

    // make sure level is in map
    auto it = logColorMap.find(lvl);
    // get escape code integer
    int color = (it != logColorMap.end()) ? it->second : 37;
    // insert escape code int string
    return "\033[" + std::to_string(color)+ "m" + levelField.str() + "\033[0m";
}

/**
*
* This method will create a formatted line for the log
* LOG_LEVEL | timestamp | CLASS_NAME : message....
*
* @param None
* @return void
*/
std::string Logger::getFormattedLog(
    LOG_LEVEL lvl, 
    std::string message)
{
    std::string line;
    // get color code for log level
    line.append(getFormattedLogLevel(lvl));
    line.append(" | ");

    // get timestamp
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    // output formatted time to a string stream to capture the output
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");

    // convert ss into a string (format is fixed 19 chars; pad for stable column width)
    constexpr int kTimestampFieldWidth = 19;
    std::string timestamp = ss.str();
    std::ostringstream tsField;
    tsField << std::left << std::setw(kTimestampFieldWidth) << timestamp;
    line.append(tsField.str());
    line.append(" | ");

    // get class name
    line.append("["+className+"] : ");

    // append message
    line.append(message);

    return line;
}

/**
*
* This method will handle sending messages to stdout
*
* @param lvl
* @param message
* @return void
*/
void Logger::handleStdoLog(
    LOG_LEVEL   lvl, 
    std::string message)
{
    // only log in console if DEBUG mode is set
    if (!debugModeFlag)          return;
    if (minStdoLevel == NO_LOGS) return;
    if (maxStdoLevel == NO_LOGS) return;

    // convert levels to integers to compare
    int intLevel = (int) lvl;
    int intMin = (int) minStdoLevel;
    int intMax = (int) maxStdoLevel;

    // only log messgaes when min <= lvl <= max 
    if (intLevel < intMin || intLevel > intMax){
        return;
    }

    // create log line
    std::string log = getFormattedLog(lvl, message);

    // send to console
    std::cout << log << std::endl;
}

/**
*
* This method will handle sending messages to log file
*
* @param lvl
* @param message
* @return void
*/
void Logger::handleFileLog(
    LOG_LEVEL   lvl, 
    std::string message)
{
    if (!fileModeFlag)           return;
    if (minFileLevel == NO_LOGS) return;
    if (maxFileLevel == NO_LOGS) return;

    // convert levels to integers to compare
    int intLevel = (int) lvl;
    int intMin = (int) minFileLevel;
    int intMax = (int) maxFileLevel;

    // only log messgaes when min <= lvl <= max 
    if (intLevel < intMin || intLevel > intMax){
        return;
    }

    // create log line
    std::string log = getFormattedLog(lvl, message);

    fileLog(log);
}


/**
*
* @desc: This method will send a log to the log file
* @param log
* @return void
*/
void Logger::fileLog(
    std::string log)
{
    const std::string resolvedPath = expandUserPath(this->logFilePath);

    // try to open file with logFilePath
    std::ofstream file(resolvedPath, std::ostream::app);
    
    // if the directory/files done exist create them
    if (!file){
        // variables to represent the sub dirs and the actual log file
        std::string directoryPath="";
        std::string filePath;

        // pointer to the far right side of the string
        int i= static_cast<int>(resolvedPath.length())-1;

        // slide pointer to the left until we find the lowest subdirectory
        while (i >= 0 && resolvedPath[static_cast<size_t>(i)] != '/'){
            i--;
        }

        // get isolated directory and file
        directoryPath = resolvedPath.substr(0, static_cast<size_t>(i));
        filePath = resolvedPath.substr(static_cast<size_t>(i)+1,(resolvedPath.size() - static_cast<size_t>(i)));

        // create subdirectories if any
        if (directoryPath.size() > 0) {
            std::string cmdStr;
        
            #if defined(_WIN32)
                cmdStr = "mkdir " + directoryPath;
            #else
                cmdStr = "mkdir -p " + directoryPath;
            #endif
        
            system(cmdStr.c_str());
        }

    }

    file.close();

    // create the log file
    std::filebuf fb;

    // open file for output
    fb.open(resolvedPath, std::ostream::app);

    // create a output stream to write to the stream
    std::ostream os(&fb);
    
    // send output to file
    os << log + "\n";

}
