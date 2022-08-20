// Non-Qt headers \/
#include "pwf2debug.h"
#include "ui_pwf2debug.h"

pwf2debug::pwf2debug(QWidget *parent): QDialog(parent), ui(new Ui::pwf2debug) {
    ui->setupUi(this);
    readOutput();
    QTimer *update = new QTimer(this);
    connect(update, &QTimer::timeout, this, &pwf2debug::update);
    update->start(1000);
}

pwf2debug::~pwf2debug() {
    delete ui;
}

void pwf2debug::readOutput() {
    QString path = pwf2info::logPath + pwf2info::logFile;
    QFile logFile(path);
    if (logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream fileContent(&logFile);
        logOutput = fileContent.readAll();
    }
}

void pwf2debug::update() {
    readOutput();
    if (logOutput != ui->logOutputText->toPlainText()) {
        ui->logOutputText->setText(logOutput);
        ui->logOutputText->verticalScrollBar()->setValue(ui->logOutputText->verticalScrollBar()->maximum());
    }
}
