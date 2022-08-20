// Non-Qt headers \/
#include "pwf2info.h"

// Strings

QString pwf2info::logFile = "log-" + QDateTime::currentDateTime().toString("MMddyyyy'-'hhmmss") + ".txt";
QString pwf2info::logPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/pahaze/pwf2tools/Logs/";
QString pwf2info::settingsPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/pahaze/pwf2tools/settings.ini";

// Functions

bool pwf2info::closeWindowOnToolUse() {
    QSettings pwf2settings(settingsPath, QSettings::IniFormat);
    return pwf2settings.value("User_Experience/CloseFormOnToolUse", true).toBool();
}

QString pwf2info::getOSTool(QString tool) {
    #if defined(Q_OS_WIN)
        return QDir::currentPath() + "/bin/" + tool + ".exe";
    #else
        return QApplication::applicationDirPath() + "/bin/" + tool;
    #endif
}

void pwf2info::showMessage(QString messageLevel, QString title, QString message) {
    QMessageBox pwf2message;
    if(messageLevel == "crit" || messageLevel == "critical") {
        pwf2message.critical(0, title, message);
        pwf2message.setFixedSize(500, 200);
    } else if(messageLevel == "info" || messageLevel == "information") {
        pwf2message.information(0, title, message);
        pwf2message.setFixedSize(500, 200);
    } else if(messageLevel == "warn" || messageLevel == "warning") {
        pwf2message.warning(0, title, message);
        pwf2message.setFixedSize(500, 200);
    }
}
