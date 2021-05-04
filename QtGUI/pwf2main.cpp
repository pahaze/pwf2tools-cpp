#include "pwf2main.h"
#include "./ui_pwf2main.h"
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QSettings>

pwf2main::pwf2main(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::pwf2main) {
    ui->setupUi(this);
    QObject::connect(ui->pwf2hat, SIGNAL(clicked()), this, SLOT(openHat()));
    QObject::connect(ui->pwf2int, SIGNAL(clicked()), this, SLOT(openInt()));
    QObject::connect(ui->pwf2spm, SIGNAL(clicked()), this, SLOT(openSpm()));
    QObject::connect(ui->pwf2tex, SIGNAL(clicked()), this, SLOT(openTex()));
}

pwf2main::~pwf2main() {
    delete ui;
}

// menubar stuff :)

void pwf2main::on_actionAbout_triggered() {
    pwf2about pwf2about;
    pwf2about.setModal(true);
    pwf2about.exec();
}

void pwf2main::on_actionOptions_triggered() {
    pwf2options pwf2options;
    pwf2options.setModal(true);
    pwf2options.exec();
}

void pwf2main::on_actionExit_triggered() {
    this->close();
}

void pwf2main::on_actionConsole_triggered() {
}

// actual app usage and stuffz

void pwf2main::openInt() {
    pwf2int pwf2int;
    pwf2int.setModal(true);
    pwf2int.exec();
}

void pwf2main::openSpm() {
    pwf2spm pwf2spm;
    pwf2spm.setModal(true);
    pwf2spm.exec();
}

void pwf2main::openTex() {
    pwf2tex pwf2tex;
    pwf2tex.setModal(true);
    pwf2tex.exec();
}

void pwf2main::openHat() {
    QMessageBox hatError;
    hatError.critical(0, "Error opening pwf2hat", "pwf2hat is not on the C++ port yet!");
    hatError.setFixedSize(500, 200);
}
