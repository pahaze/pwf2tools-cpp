// Non-Qt headers \/
#include "pwf2main.h"
#include "./ui_pwf2main.h"

pwf2main::pwf2main(QWidget *parent): QMainWindow(parent), ui(new Ui::pwf2main) {
    ui->setupUi(this);
    pwf2log::startLog();
    setUpButtons();
    checkMenubar();
    #ifdef ENABLE_RICHPRESENCE
        pwf2discord::initialize();
        QTimer *update = new QTimer(this);
        connect(update, &QTimer::timeout, this, &pwf2main::updateRichPresence);
        update->start(500);
    #endif
}

pwf2main::~pwf2main() {
    delete ui;
}

// Menubar stuff :)

void pwf2main::exitApp() {
    this->close();
}

void pwf2main::openAbout() {
    pwf2log::writeToLog(LOG_INFO, "Opened about page");
    aboutWindow.show();
}

void pwf2main::openConsole() {
    pwf2log::writeToLog(LOG_INFO, "Opened console");
    consoleWindow.show();
}

void pwf2main::openOptions() {
    pwf2log::writeToLog(LOG_INFO, "Opened settings page");
    optionsWindow.show();
}

// Actual app usage

void pwf2main::openInt() {
    pwf2log::writeToLog(LOG_INFO, "Opened pwf2int");
    intWindow.show();
}

void pwf2main::openSpm() {
    pwf2log::writeToLog(LOG_INFO, "Opened pwf2spm");
    spmWindow.show();
}

void pwf2main::openTex() {
    pwf2log::writeToLog(LOG_INFO, "Opened pwf2tex");
    texWindow.show();
}

// Easier setup

void pwf2main::checkMenubar() {
    if(ui->menubar->height() == 0) {
        this->setMinimumHeight(81);
        this->setMaximumHeight(81);
        this->resize(292, 81);
    }
}

/// Close event, used for closing multiple tools at one time
void pwf2main::closeEvent(QCloseEvent *event) {
    #ifdef ENABLE_RICHPRESENCE
        pwf2discord::deinitialize();
    #endif
    pwf2log::closeLog();
    aboutWindow.close();
    consoleWindow.close();
    intWindow.close();
    optionsWindow.close();
    spmWindow.close();
    texWindow.close();
}

void pwf2main::setUpButtons() {
    QObject::connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(openAbout()));
    QObject::connect(ui->actionConsole, SIGNAL(triggered()), this, SLOT(openConsole()));
    QObject::connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exitApp()));
    QObject::connect(ui->actionOptions, SIGNAL(triggered()), this, SLOT(openOptions()));
    QObject::connect(ui->pwf2int, SIGNAL(clicked()), this, SLOT(openInt()));
    QObject::connect(ui->pwf2spm, SIGNAL(clicked()), this, SLOT(openSpm()));
    QObject::connect(ui->pwf2tex, SIGNAL(clicked()), this, SLOT(openTex()));
}

void pwf2main::updateRichPresence() {
    #ifdef ENABLE_RICHPRESENCE
        if(this->isActiveWindow() && !intWindow.isActiveWindow() && !spmWindow.isActiveWindow() && !texWindow.isActiveWindow())
            pwf2discord::updateStatus("Choosing a tool!");
    #endif
}
