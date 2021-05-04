#include "pwf2options.h"
#include "ui_pwf2options.h"
#include <QDir>
#include <QMessageBox>
#include <QSettings>

pwf2options::pwf2options(QWidget *parent) : QDialog(parent),
                                            ui(new Ui::pwf2options) {
    ui->setupUi(this);
    QObject::connect(ui->OkButton, SIGNAL(clicked()), this, SLOT(OKButtonAction()));
    QObject::connect(ui->CancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonAction()));
    readSettings();
}

void pwf2options::cancelButtonAction() {
    this->close();
}

void pwf2options::OKButtonAction() {
    QSettings pwf2settings((QDir(QApplication::applicationDirPath()).filePath("settings.ini")), QSettings::IniFormat);
    writeSettings();
    this->close();
}

void pwf2options::readSettings() {
    QSettings pwf2settings((QDir(QApplication::applicationDirPath()).filePath("settings.ini")), QSettings::IniFormat);

    pwf2settings.beginGroup("Privacy");
    ui->enableRPCbox->setChecked(pwf2settings.value("EnableRPC", true).toBool());
    pwf2settings.endGroup();

    pwf2settings.beginGroup("User_Experience");
    ui->enableToolclose->setChecked(pwf2settings.value("CloseFormOnToolUse", true).toBool());
    pwf2settings.endGroup();
}

void pwf2options::writeSettings() {
    QSettings pwf2settings((QDir(QApplication::applicationDirPath()).filePath("settings.ini")), QSettings::IniFormat);

    pwf2settings.beginGroup("Privacy");
    pwf2settings.setValue("EnableRPC", ui->enableRPCbox->isChecked());
    pwf2settings.endGroup();

    pwf2settings.beginGroup("User_Experience");
    pwf2settings.setValue("CloseFormOnToolUse", ui->enableToolclose->isChecked());
    pwf2settings.endGroup();
}

pwf2options::~pwf2options() {
    delete ui;
}
