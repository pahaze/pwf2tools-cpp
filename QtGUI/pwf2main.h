#ifndef PWF2MAIN_H
#define PWF2MAIN_H

#include <QMainWindow>
#include <pwf2about.h>
#include <pwf2debug.h>
#include <pwf2hat.h>
#include <pwf2int.h>
#include <pwf2options.h>
#include <pwf2spm.h>
#include <pwf2tex.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class pwf2main;
}
QT_END_NAMESPACE

class pwf2main : public QMainWindow {
    Q_OBJECT

    public:
    pwf2about *pwf2aboutmain;
    pwf2options *pwf2optionsmain;
    pwf2int *pwf2intmain;
    pwf2hat *pwf2hatmain;
    pwf2main(QWidget *parent = nullptr);
    ~pwf2main();

    private slots:
    void loadSettings();
    void on_actionAbout_triggered();
    void on_actionOptions_triggered();
    void on_actionExit_triggered();
    void on_actionConsole_triggered();
    void openInt();
    void openSpm();
    void openTex();
    void openHat();

    private:
    Ui::pwf2main *ui;
};
#endif    // PWF2MAIN_H
