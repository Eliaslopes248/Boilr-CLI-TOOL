/**
*
* This class is used to log output of program to a file
* on running system.
*
* file -> ~/boilr/LOG/log.txt
*
*/
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

// enum to represent the log levels by severity
enum LOG_LEVEL {
    INFO=0,
    DEBUG=1,
    WARNING=2,
    ERROR=3,
    FATAL=4,
    NO_LOGS=5
};

class Logger
{
private:
// file that logs will output to
std::string logFilePath = "~/boilr/LOG/log.txt";

// class name is used to identify what file it called the log
std::string className = "NO CLASS_NAME";

// min/max log level that will be outputted to the file
LOG_LEVEL minFileLevel = LOG_LEVEL(0);
LOG_LEVEL maxFileLevel = LOG_LEVEL(4);

// min/max to be outputed to standard output
LOG_LEVEL minStdoLevel = LOG_LEVEL(0);
LOG_LEVEL maxStdoLevel = LOG_LEVEL(4);

// flag to tell if we stdout logs to console
bool debugModeFlag;

// checks ENV var for BOILR_ENV_TYPE == debug | d | DEBUG
bool inDebugMode();

void handleFileLog(LOG_LEVEL   lvl, std::string message);
void handleStdoLog(LOG_LEVEL   lvl, std::string message);

public:

Logger();

Logger(char* logFilePath);

~Logger();

Logger(
    char*     logFilePath,
    LOG_LEVEL minFileLevel,
    LOG_LEVEL maxFileLevel,
    LOG_LEVEL minStdoLevel,
    LOG_LEVEL maxStdoLevel
);

Logger(
    char* logFilePath,
    int minFileLevel,
    int maxFileLevel,
    int minStdoLevel,
    int maxStdoLevel
);

// logs message to stdout and or log file
void invoke(
    LOG_LEVEL   lvl,
    std::string message
);

};