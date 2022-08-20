#ifndef PWF2INFO_H
#define PWF2INFO_H
// Qt headers \/
#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QString>

// Defines
#define MAJOR_VERSION "14"
#define MINOR_VERSION "5"

// Class
/// pwf2info is meant to be a "helper" class
class pwf2info {
    public:
        static bool closeWindowOnToolUse();
        static QString getOSTool(QString tool);
        static QString logPath;
        static QString logFile;
        static QString settingsPath;
        static void showMessage(QString messageLevel, QString title, QString message);
};

#endif // PWF2INFO_H
