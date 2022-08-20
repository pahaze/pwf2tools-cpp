#ifndef PWF2INT_H
#define PWF2INT_H
// Qt headers \/
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QTimer>
// Non-Qt headers \/
#include "pwf2discord.h"
#include "pwf2info.h"
#include "pwf2log.h"

namespace Ui {
    class pwf2int;
}

class pwf2int: public QDialog {
    Q_OBJECT
    public:
        explicit pwf2int(QWidget *parent = nullptr);
        ~pwf2int();
    private slots:
        void openINT();
        void openFolder();
        void runInt();
        void setUpButtons();
        void updateRichPresence();
    private:
        Ui::pwf2int *ui;
        QString intExecutable = pwf2info::getOSTool("pwf2int");
};

#endif // PWF2INT_H
