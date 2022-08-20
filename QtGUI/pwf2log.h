#ifndef PWF2LOG_H
#define PWF2LOG_H
// Qt headers \/
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QString>
#include <QTextStream>
// Non-Qt headers \/
#include <iostream>
#include "pwf2info.h"
#include <string>
#include <vector>

// Log levels
enum LOG_LEVELS {
    LOG_ERROR,
    LOG_INFO,
    LOG_PWF2INT,
    LOG_PWF2SPM,
    LOG_PWF2TEX,
    LOG_WARNING
};

static const char *LOG_LEVELS_CHAR[] = {
    "ERROR",
    "INFO",
    "PWF2INT",
    "PWF2SPM",
    "PWF2TEX",
    "WARNING"
};

// Class
class pwf2log {
    public:
        static std::string logOutput;
        static std::string readFromLog();
        static void closeLog();
        static void startLog();
        static void writeToLog(LOG_LEVELS logLevel = LOG_INFO, std::string logContent = "");
        static void writeToLogNN(LOG_LEVELS logLevel = LOG_INFO, std::string logContent = "");
};

#endif // PWF2LOG_H
