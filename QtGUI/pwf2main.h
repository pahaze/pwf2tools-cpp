#ifndef PWF2MAIN_H
#define PWF2MAIN_H
// Qt headers \/
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>
// Non-Qt headers \/
#include <pwf2about.h>
#include <pwf2debug.h>
#include <pwf2discord.h>
#include <pwf2hat.h>
#include <pwf2int.h>
#include <pwf2log.h>
#include <pwf2options.h>
#include <pwf2spm.h>
#include <pwf2tex.h>

QT_BEGIN_NAMESPACE
    namespace Ui {
        class pwf2main;
    }
QT_END_NAMESPACE

class pwf2main: public QMainWindow {
    Q_OBJECT
    public:
        pwf2main(QWidget *parent = nullptr);
        ~pwf2main();
    private slots:
        void checkMenubar();
        void closeEvent(QCloseEvent *event);
        void exitApp();
        void openAbout();
        void openConsole();
        void openInt();
        void openOptions();
        void openSpm();
        void openTex();
        void setUpButtons();
        void updateRichPresence();
    private:
        pwf2about aboutWindow;
        pwf2debug consoleWindow;
        pwf2int intWindow;
        pwf2options optionsWindow;
        pwf2spm spmWindow;
        pwf2tex texWindow;
        Ui::pwf2main *ui;
};

#endif // PWF2MAIN_H
