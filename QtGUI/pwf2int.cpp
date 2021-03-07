#include "pwf2int.h"
#include "pwf2main.h"
#include "ui_pwf2int.h"
#include <QDir>
#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QtGlobal>

pwf2int::pwf2int(QWidget *parent) : QDialog(parent),
                                    ui(new Ui::pwf2int) {
    ui->setupUi(this);
    QObject::connect(ui->openIntExt, SIGNAL(clicked()), this, SLOT(openINT()));
    QObject::connect(ui->openFolderExt, SIGNAL(clicked()), this, SLOT(opnFolderExt()));
    QObject::connect(ui->RunExtButt, SIGNAL(clicked()), this, SLOT(runExt()));
    QObject::connect(ui->saveIntButt, SIGNAL(clicked()), this, SLOT(openINTCrt()));
    QObject::connect(ui->findModdedFolderButt, SIGNAL(clicked()), this, SLOT(opnFolderCrt()));
    QObject::connect(ui->CrtIntButt, SIGNAL(clicked()), this, SLOT(runCrt()));
}

// Pre-setup stuff

QString pwf2int::getOS() {
#if defined(Q_OS_MACOS)
    return QDir::currentPath() + "/bin/pwf2int";
#elif defined(Q_OS_WIN)
    return QDir::currentPath() + "/bin/pwf2int.exe";
#elif defined(Q_OS_LINUX)
    return QDir::currentPath() + "/bin/pwf2int";
#elif defined(Q_OS_UNIX)
    return QDir::currentPath() + "/bin/pwf2int";
#else
    return QDir::currentPath() + "/bin/pwf2int";
#endif
}

// Extract an INT

void pwf2int::openINT() {
    QString OpenIntPath = QFileDialog::getOpenFileName(this, tr("Open an INT"), "", tr("INT files (*.INT);;All Files (*)"));
    if (OpenIntPath.isEmpty()) {
        QMessageBox noFileError;
        noFileError.critical(0, "Error opening file!", "Please choose a file!");
        noFileError.setFixedSize(500, 200);
        return;
    } else {
        QFile file(OpenIntPath);
        if (!file.open(QIODevice::ReadWrite)) {
            QMessageBox::information(this, tr("Failed to open file! Try again.."), file.errorString());
            return;
        }
        ui->ExtIntPathBox->setText(file.fileName());
    }
}

void pwf2int::opnFolderExt() {
    QString OpenFolderPathExt = QFileDialog::getExistingDirectory(this, tr("Open a folder to extract to!"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (OpenFolderPathExt.isEmpty()) {
        QMessageBox noFolderError;
        noFolderError.critical(0, "Error opening folder!", "Please choose a folder!");
        noFolderError.setFixedSize(500, 200);
        return;
    } else {
        QDir dir(OpenFolderPathExt);
        if (!dir.exists()) {
            dir.mkdir(OpenFolderPathExt);
            return;
        }
        ui->openFolderExtPathBox->setText(dir.path());
    }
}

void pwf2int::runExt() {
    QProcess pwf2proc;
    if (ui->openFolderExtPathBox->text().isNull() || ui->ExtIntPathBox->text().isNull() || ui->openFolderExtPathBox->text().isEmpty() || ui->ExtIntPathBox->text().isEmpty()) {
        QMessageBox emptyBoxError;
        emptyBoxError.critical(0, "Error running pwf2int!", "You must fill all boxes in!");
        emptyBoxError.setFixedSize(500, 200);
    } else {
        QString pwf2intexecloc = pwf2int::getOS();
        QString pwf2intargs = " extract  \"" + ui->ExtIntPathBox->text() + "\" \"" + ui->openFolderExtPathBox->text() + "\"";
        pwf2proc.execute(pwf2intexecloc + pwf2intargs);
        pwf2proc.waitForFinished(-1);
        if (pwf2proc.waitForFinished() && pwf2proc.exitStatus() != QProcess::NormalExit) {
            pwf2proc.kill();
            QMessageBox criticalRunError;
            criticalRunError.critical(0, "Error running pwf2int!", "Something bad happened and pwf2int failed to run! Error: " + pwf2proc.error());
            criticalRunError.setFixedSize(500, 200);
        } else {
            QMessageBox successMessage;
            successMessage.information(0, "Done!", "Finished!");
            successMessage.setFixedSize(500, 200);
        }
        this->close();
    }
}

// Create an INT

void pwf2int::openINTCrt() {
    QString saveInt = QFileDialog::getSaveFileName(this, tr("Save an INT"), "", tr("INT files (*.INT);;All Files (*)"));
    if (saveInt.isEmpty()) {
        QMessageBox noFileError;
        noFileError.critical(0, "Error creating INT!", "Please save a file!");
        noFileError.setFixedSize(500, 200);
        return;
    } else {
        QFile file(saveInt);
        if (!file.open(QIODevice::ReadWrite)) {
            QMessageBox::information(this, tr("Failed to save file! Try again.."), file.errorString());
            return;
        }
        ui->saveIntPathBox->setText(file.fileName());
    }
}

void pwf2int::opnFolderCrt() {
    QString OpenFolderPathCrt = QFileDialog::getExistingDirectory(this, tr("Open a folder to extract to!"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (OpenFolderPathCrt.isEmpty()) {
        QMessageBox noFolderError;
        noFolderError.critical(0, "Error opening folder!", "Please choose a folder!");
        noFolderError.setFixedSize(500, 200);
        return;
    } else {
        QDir dir(OpenFolderPathCrt);
        if (!dir.exists()) {
            dir.mkdir(OpenFolderPathCrt);
            return;
        }
        ui->moddedIntFolderPathBox->setText(dir.path());
    }
}

void pwf2int::runCrt() {
    QProcess pwf2proc;
    if (ui->saveIntPathBox->text().isNull() || ui->moddedIntFolderPathBox->text().isNull() || ui->saveIntPathBox->text().isEmpty() || ui->saveIntPathBox->text().isEmpty()) {
        QMessageBox emptyBoxError;
        emptyBoxError.critical(0, "Error running pwf2int!", "You must fill all boxes in!");
        emptyBoxError.setFixedSize(500, 200);
    } else {
        QString pwf2intexecloc = pwf2int::getOS();
        QString pwf2intargs = " create  \"" + ui->saveIntPathBox->text() + "\" \"" + ui->moddedIntFolderPathBox->text() + "\"";
        pwf2proc.execute(pwf2intexecloc + pwf2intargs);
        pwf2proc.waitForFinished(-1);
        if (pwf2proc.waitForFinished() && pwf2proc.exitStatus() != QProcess::NormalExit) {
            pwf2proc.kill();
            QMessageBox criticalRunError;
            criticalRunError.critical(0, "Error running pwf2int!", "Something bad happened and pwf2int failed to run! Error: " + pwf2proc.error());
            criticalRunError.setFixedSize(500, 200);
        } else {
            QMessageBox successMessage;
            successMessage.information(0, "Done!", "Finished!");
            successMessage.setFixedSize(500, 200);
        }
        this->close();
    }
}

pwf2int::~pwf2int() {
    delete ui;
}
