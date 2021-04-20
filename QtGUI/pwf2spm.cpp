#include "pwf2spm.h"
#include "ui_pwf2spm.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>

pwf2spm::pwf2spm(QWidget *parent) : QDialog(parent),
                                    ui(new Ui::pwf2spm) {
    ui->setupUi(this);
    QObject::connect(ui->openSpmButt, SIGNAL(clicked()), this, SLOT(openSPM()));
    QObject::connect(ui->saveTex0Butt, SIGNAL(clicked()), this, SLOT(saveTex0()));
    QObject::connect(ui->finalSaveTex0Butt, SIGNAL(clicked()), this, SLOT(saveFinal()));
}

// Pre-setup stuff

QString pwf2spm::getOS() {
#if defined(Q_OS_MACOS)
    return QDir::currentPath() + "/bin/pwf2spm";
#elif defined(Q_OS_WIN)
    return QDir::currentPath() + "/bin/pwf2spm.exe";
#elif defined(Q_OS_LINUX)
    return QDir::currentPath() + "/bin/pwf2spm";
#elif defined(Q_OS_UNIX)
    return QDir::currentPath() + "/bin/pwf2spm";
#else
    return QDir::currentPath() + "/bin/pwf2spm";
#endif
}

// Open/save files

void pwf2spm::openSPM() {
    QString OpenSpmPath = QFileDialog::getOpenFileName(this, tr("Open an SPM"), "", tr("SPM files (*.SPM);;All Files (*)"));
    if (OpenSpmPath.isEmpty()) {
        QMessageBox noFileError;
        noFileError.critical(0, "Error opening file!", "Please choose a file!");
        noFileError.setFixedSize(500, 200);
        return;
    } else {
        QFile file(OpenSpmPath);
        if (!file.open(QIODevice::ReadWrite)) {
            QMessageBox::information(this, tr("Failed to open file! Try again.."), file.errorString());
            return;
        }
        ui->SpmPathBox->setText(file.fileName());
    }
}

void pwf2spm::saveTex0() {
    QString saveTex0 = QFileDialog::getSaveFileName(this, tr("Save a tex0"), "", tr("tex0 files (*.tex0);;All Files (*)"));
    if (saveTex0.isEmpty()) {
        QMessageBox noFileError;
        noFileError.critical(0, "Error creating tex0!", "Please save a file!");
        noFileError.setFixedSize(500, 200);
        return;
    } else {
        QFile file(saveTex0);
        if (!file.open(QIODevice::ReadWrite)) {
            QMessageBox::information(this, tr("Failed to save file! Try again.."), file.errorString());
            return;
        }
        ui->Tex0PathBox->setText(file.fileName());
    }
}

void pwf2spm::saveFinal() {
    QProcess pwf2proc;
    if (ui->SpmPathBox->text().isNull() || ui->Tex0PathBox->text().isNull() || ui->SpmPathBox->text().isEmpty() || ui->Tex0PathBox->text().isEmpty()) {
        QMessageBox emptyBoxError;
        emptyBoxError.critical(0, "Error running pwf2spm!", "You must fill all boxes in!");
        emptyBoxError.setFixedSize(500, 200);
    } else {
        QFile openspmloc(ui->SpmPathBox->text());
        if (!openspmloc.exists()) {
            QMessageBox nonExistantFile;
            nonExistantFile.critical(0, "Error opening SPM.",
                "Please make sure the file you "
                "chose exists and try again.");
            nonExistantFile.setFixedSize(500, 200);
        } else {
            QString pwf2intexecloc = pwf2spm::getOS();
            QString pwf2intargs = " gtex0  \"" + ui->SpmPathBox->text() + "\" \"" + ui->Tex0PathBox->text() + "\"";
            pwf2proc.execute(pwf2intexecloc + pwf2intargs);
            pwf2proc.waitForFinished(-1);
            if (pwf2proc.waitForFinished() && pwf2proc.exitStatus() != QProcess::NormalExit) {
                pwf2proc.kill();
                QMessageBox criticalRunError;
                criticalRunError.critical(0, "Error running pwf2spm!", "Something bad happened and spm failed to run! Error: " + pwf2proc.error());
                criticalRunError.setFixedSize(500, 200);
            } else {
                QMessageBox successMessage;
                successMessage.information(0, "Done!", "Finished!");
                successMessage.setFixedSize(500, 200);
            }
            this->close();
        }
    }
}

pwf2spm::~pwf2spm() {
    delete ui;
}
