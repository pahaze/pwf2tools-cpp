// Non-Qt headers \/
#include "pwf2options.h"
#include "ui_pwf2options.h"

pwf2options::pwf2options(QWidget *parent): QDialog(parent), ui(new Ui::pwf2options) {
    ui->setupUi(this);
    readSettings();
    setUpButtons();
    #ifndef ENABLE_RICHPRESENCE
        ui->settingsTabs->removeTab(0);
    #endif
}

pwf2options::~pwf2options() {
    delete ui;
}

void pwf2options::cancelButtonAction() {
    this->close();
}

void pwf2options::OKButtonAction() {
    writeSettings();
    this->close();
}

void pwf2options::readSettings() {
    QSettings pwf2settings(pwf2info::settingsPath, QSettings::IniFormat);

    #ifdef ENABLE_RICHPRESENCE
        pwf2settings.beginGroup("Privacy");
        ui->enableRPCbox->setChecked(pwf2settings.value("EnableRPC", true).toBool());
        pwf2settings.endGroup();
    #endif

    pwf2settings.beginGroup("User_Experience");
    ui->enableToolclose->setChecked(pwf2settings.value("CloseFormOnToolUse", true).toBool());
    pwf2settings.endGroup();
}

void pwf2options::writeSettings() {
    QSettings pwf2settings(pwf2info::settingsPath, QSettings::IniFormat);

    #ifdef ENABLE_RICHPRESENCE
        pwf2settings.beginGroup("Privacy");
        pwf2settings.setValue("EnableRPC", ui->enableRPCbox->isChecked());
        pwf2settings.endGroup();
    #endif

    pwf2settings.beginGroup("User_Experience");
    pwf2settings.setValue("CloseFormOnToolUse", ui->enableToolclose->isChecked());
    pwf2settings.endGroup();
}

// This is just cleaner than junking up the QWidget's creation.

void pwf2options::setUpButtons() {
    QObject::connect(ui->OkButton, SIGNAL(clicked()), this, SLOT(OKButtonAction()));
    QObject::connect(ui->CancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonAction()));
}
