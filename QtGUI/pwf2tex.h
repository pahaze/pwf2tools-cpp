#ifndef PWF2TEX_H
#define PWF2TEX_H
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
    class pwf2tex;
}

class pwf2tex: public QDialog {
    Q_OBJECT
    public:
        explicit pwf2tex(QWidget *parent = nullptr);
        ~pwf2tex();
    private slots:
        void openPNGFolder();
        void openTEX0File();
        void openTEX0Folder();
        void runTex();
        void setUpButtons();
        void updateRichPresence();
    private:
        Ui::pwf2tex *ui;
        QString texExecutable = pwf2info::getOSTool("pwf2tex");
};

#endif // PWF2TEX_H
