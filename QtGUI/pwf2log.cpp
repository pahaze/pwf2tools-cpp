// Non-Qt headers \/
#include "pwf2log.h"

// Strings
std::string pwf2log::logOutput;

/// I don't think there's a reason anybody would need this,
/// but it's here just in case.
std::string pwf2log::readFromLog() {
    logOutput = "";
    QFile logFile(pwf2info::logPath + pwf2info::logFile);
    if (logFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)) {
        QTextStream fileStream(&logFile);
        logOutput = fileStream.readAll().toStdString();
    }
    return logOutput;
}

void pwf2log::closeLog() {
    writeToLog(LOG_INFO, "Closing pwf2tools...");
    logOutput = "";
}

void pwf2log::startLog() {
    logOutput = "";

    QDir logPath(pwf2info::logPath);
    if(!logPath.exists())
        logPath.mkpath(pwf2info::logPath);

    writeToLog(LOG_INFO, "pwf2tools started!");
}

void pwf2log::writeToLog(LOG_LEVELS logLevel, std::string logContent) {
    std::string input;
    input = "[" + QDateTime::currentDateTime().toString("dddd, MMMM dd, yyyy '-' hh:mm:ss").toStdString() + ", " + LOG_LEVELS_CHAR[logLevel] + "] - " + logContent + "\n";
    QFile logFile(pwf2info::logPath + pwf2info::logFile);
    if (logFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)) {
        QTextStream fileStream(&logFile);
        fileStream << input.c_str();
    }
    logFile.close();
}

void pwf2log::writeToLogNN(LOG_LEVELS logLevel, std::string logContent) {
    std::string input;
    input = "[" + QDateTime::currentDateTime().toString("dddd, MMMM dd, yyyy '-' hh:mm:ss").toStdString() + ", " + LOG_LEVELS_CHAR[logLevel] + "] - " + logContent;
    QFile logFile(pwf2info::logPath + pwf2info::logFile);
    if (logFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)) {
        QTextStream fileStream(&logFile);
        fileStream << input.c_str();
    }
    logFile.close();
}
