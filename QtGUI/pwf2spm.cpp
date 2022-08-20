// Non-Qt headers \/
#include "pwf2spm.h"
#include "ui_pwf2spm.h"

pwf2spm::pwf2spm(QWidget *parent): QDialog(parent), ui(new Ui::pwf2spm) {
    ui->setupUi(this);
    setUpButtons();
    #ifdef ENABLE_RICHPRESENCE
        QTimer *update = new QTimer(this);
        connect(update, &QTimer::timeout, this, &pwf2spm::updateRichPresence);
        update->start(500);
    #endif
}

pwf2spm::~pwf2spm() {
    delete ui;
}

// Functions

void pwf2spm::openSPM() {
    QString OpenSpmPath = QFileDialog::getOpenFileName(this, tr("Open an SPM"), "", tr("SPM files (*.SPM, *.spm);;All Files (*)"));
    if (OpenSpmPath.isEmpty()) {
        pwf2log::writeToLog(LOG_ERROR, "FAILED to open a file with reason: no file chosen");
        pwf2info::showMessage("critical", "Error opening file!", "Please choose a file!");
        return;
    } else {
        QFile spmFile(OpenSpmPath);
        if (!spmFile.open(QIODevice::ReadWrite)) {
            pwf2log::writeToLog(LOG_ERROR, "FAILED to open file " + OpenSpmPath.toStdString() + " with reason: " + spmFile.errorString().toStdString());
            pwf2info::showMessage("critical", "Failed to open file! Try again...", spmFile.errorString());
            return;
        }
        ui->SpmPathBox->setText(spmFile.fileName());
    }
}

void pwf2spm::runSpm() {
    QProcess pwf2proc;
    if (ui->SpmPathBox->text().isNull() || ui->Tex0PathBox->text().isNull() || ui->SpmPathBox->text().isEmpty() || ui->Tex0PathBox->text().isEmpty()) {
        pwf2log::writeToLog(LOG_WARNING, "FAILED to run pwf2spm with reason: not all arguments are filled in");
        pwf2info::showMessage("warning", "Error running pwf2spm!", "You must fill all boxes in!");
    } else {
        QFile spmLocation(ui->SpmPathBox->text());
        if (!spmLocation.exists()) {
            pwf2log::writeToLog(LOG_ERROR, "FAILED to open file " + spmLocation.fileName().toStdString() + " with reason: " + spmLocation.errorString().toStdString());
            pwf2info::showMessage("critical", "Error opening SPM file!", "Please make sure the file you chose exists and try again!");
        } else {
            QStringList spmArguments;
            spmArguments << "gtex0" << ui->SpmPathBox->text() << ui->Tex0PathBox->text();
            pwf2proc.start(spmExecutable, spmArguments);
            pwf2proc.waitForFinished(-1);
            if(!pwf2proc.waitForFinished()) {
                /// In case something disastrous happens, this should(?) help.
                while(pwf2proc.canReadLine()) {
                    pwf2log::writeToLogNN(LOG_PWF2SPM, pwf2proc.readLine().toStdString());
                }
                if (pwf2proc.exitStatus() != 0) {
                    pwf2proc.kill();
                    pwf2log::writeToLog(LOG_ERROR, "pwf2spm FAILED to run. Status code: " + QString(pwf2proc.exitStatus()).toStdString());
                    /// Not always given an error.
                    if(!pwf2proc.errorString().isEmpty() && !pwf2proc.errorString().isNull())
                        pwf2log::writeToLog(LOG_ERROR, "It seems pwf2spm threw an error too. Error: " + pwf2proc.errorString().toStdString());
                    pwf2info::showMessage("critical", "Error running pwf2spm!", "Something bad happened and pwf2spm failed to run! Error: " + QString(pwf2proc.error()));
                } else {
                    pwf2log::writeToLog(LOG_INFO, "Successfully ran pwf2spm with executable " + spmExecutable.toStdString() + " and arguments " + spmArguments.join(" ").toStdString() + "!");
                    pwf2info::showMessage("info", "Done!", "Finished!");
                }
                QSettings pwf2settings(pwf2info::settingsPath, QSettings::IniFormat);
                if (pwf2info::closeWindowOnToolUse()) {
                    this->close();
                }
            }
        }
    }
}

void pwf2spm::saveTex0() {
    QString OpenTex0Path = QFileDialog::getSaveFileName(this, tr("Save a tex0"), "", tr("tex0 files (*.TEX0, *.tex0);;All Files (*)"));
    if (OpenTex0Path.isEmpty()) {
        pwf2log::writeToLog(LOG_ERROR, "FAILED to save a file with reason: no file chosen");
        pwf2info::showMessage("critical", "Error creating tex0!", "Please save a file!");
        return;
    } else {
        QFile tex0File(OpenTex0Path);
        if (!tex0File.open(QIODevice::ReadWrite)) {
            pwf2log::writeToLog(LOG_ERROR, "FAILED to save file " + OpenTex0Path.toStdString() + " with reason: " + tex0File.errorString().toStdString());
            pwf2info::showMessage("critical", "Failed to save file! Try again...", tex0File.errorString());
            return;
        }
        ui->Tex0PathBox->setText(tex0File.fileName());
    }
}

// This is just cleaner than junking up the QWidget's creation.

void pwf2spm::setUpButtons() {
    QObject::connect(ui->openSpmButton, SIGNAL(clicked()), this, SLOT(openSPM()));
    QObject::connect(ui->saveTex0Button, SIGNAL(clicked()), this, SLOT(saveTex0()));
    QObject::connect(ui->run, SIGNAL(clicked()), this, SLOT(runSpm()));
}

void pwf2spm::updateRichPresence() {
    #ifdef ENABLE_RICHPRESENCE
        if(this->isActiveWindow()) {
            pwf2discord::updateStatus("Using pwf2spm!", "Creating a TEX0!");
        }
    #endif
}
