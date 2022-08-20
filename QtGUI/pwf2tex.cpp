// Non-Qt headers \/
#include "pwf2tex.h"
#include "ui_pwf2tex.h"

pwf2tex::pwf2tex(QWidget *parent): QDialog(parent), ui(new Ui::pwf2tex) {
    ui->setupUi(this);
    setUpButtons();
    #ifdef ENABLE_RICHPRESENCE
        QTimer *update = new QTimer(this);
        connect(update, &QTimer::timeout, this, &pwf2tex::updateRichPresence);
        update->start(500);
    #endif
}

pwf2tex::~pwf2tex() {
    delete ui;
}

// Functions
/// All functions work with both tabs now

void pwf2tex::openPNGFolder() {
    QString OpenPNGFolderPath = QFileDialog::getExistingDirectory(this, tr("Open a folder to extract PNGs to!"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (OpenPNGFolderPath.isEmpty()) {
        pwf2log::writeToLog(LOG_ERROR, "FAILED to open a folder with reason: no folder chosen");
        pwf2info::showMessage("critical", "Error opening folder!", "Please choose a folder!");
        return;
    } else {
        QDir PNGFolder(OpenPNGFolderPath);
        if (!PNGFolder.exists()) {
            PNGFolder.mkpath(OpenPNGFolderPath);
            pwf2log::writeToLog(LOG_WARNING, "Folder at " + OpenPNGFolderPath.toStdString() + " doesn't exist!");
            pwf2log::writeToLog(LOG_INFO, "Creating folder at " + OpenPNGFolderPath.toStdString() + "...");
        }
        if(ui->texTabs->currentIndex() == 0)
            ui->extractPNGFolderPath->setText(PNGFolder.path());
        else
            ui->injectPNGFolderPath->setText(PNGFolder.path());
        pwf2log::writeToLog(LOG_INFO, "Successfully opened folder at " + OpenPNGFolderPath.toStdString());
    }
}

void pwf2tex::openTEX0File() {
    QString OpenTEX0FilePath = QFileDialog::getOpenFileName(this, tr("Open a tex0 file!"), "", tr("tex0 files (*.TEX0, *.tex0);;All Files (*)"));
    if (OpenTEX0FilePath.isEmpty()) {
        pwf2log::writeToLog(LOG_ERROR, "FAILED to open a file with reason: no file chosen");
        pwf2info::showMessage("critical", "Error opening file!", "Please choose a file!");
        return;
    } else {
        QFile TEX0File(OpenTEX0FilePath);
        if (!TEX0File.open(QIODevice::ReadWrite)) {
            pwf2log::writeToLog(LOG_ERROR, "FAILED to open file " + OpenTEX0FilePath.toStdString() + " with reason: " + TEX0File.errorString().toStdString());
            pwf2info::showMessage("critical", "Failed to open file! Try again...", TEX0File.errorString());
            return;
        }
        if(ui->texTabs->currentIndex() == 0)
            ui->extractTEX0FilePath->setText(TEX0File.fileName());
        else
            ui->injectTEX0FilePath->setText(TEX0File.fileName());
        pwf2log::writeToLog(LOG_INFO, "Successfully opened file at " + OpenTEX0FilePath.toStdString());
    }
}

void pwf2tex::openTEX0Folder() {
    QString OpenTEX0FolderPath = QFileDialog::getExistingDirectory(this, tr("Open the TEXTURES folder (from your INT)!"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (OpenTEX0FolderPath.isEmpty()) {
        pwf2log::writeToLog(LOG_ERROR, "FAILED to open a folder with reason: no folder chosen");
        pwf2info::showMessage("critical", "Error opening folder!", "Please choose a folder!");
        return;
    } else {
        QDir dir(OpenTEX0FolderPath);
        if (!dir.exists()) {
            dir.mkdir(OpenTEX0FolderPath);
            pwf2log::writeToLog(LOG_WARNING, "Folder at " + OpenTEX0FolderPath.toStdString() + " doesn't exist!");
            pwf2log::writeToLog(LOG_INFO, "Creating folder at " + OpenTEX0FolderPath.toStdString() + "...");
        }
        if(ui->texTabs->currentIndex() == 0)
            ui->extractTEX0FolderPath->setText(dir.path());
        else
            ui->injectTEX0FolderPath->setText(dir.path());
        pwf2log::writeToLog(LOG_INFO, "Successfully opened folder at " + OpenTEX0FolderPath.toStdString());
    }
}

void pwf2tex::runTex() {
    QFile TEX0FileLocation;
    QProcess pwf2proc;
    QStringList texArguments;
    if(ui->texTabs->currentIndex() == 0) {
        if (ui->extractPNGFolderPath->text().isNull() || ui->extractTEX0FilePath->text().isEmpty() || ui->extractTEX0FolderPath->text().isNull() || ui->extractPNGFolderPath->text().isEmpty() || ui->extractTEX0FilePath->text().isNull() || ui->extractTEX0FolderPath->text().isEmpty()) {
            pwf2log::writeToLog(LOG_WARNING, "FAILED to run pwf2tex with reason: not all arguments are filled in");
            pwf2info::showMessage("warning", "Error running pwf2tex!", "You must fill all boxes in!");
        } else {
            TEX0FileLocation.setFileName(ui->extractTEX0FilePath->text());
            if (!TEX0FileLocation.exists()) {
                pwf2log::writeToLog(LOG_ERROR, "FAILED to open file " + TEX0FileLocation.fileName().toStdString() + " with reason: " + TEX0FileLocation.errorString().toStdString());
                pwf2info::showMessage("critical", "Error opening TEX0 file!", "Please make sure the file you chose exists and try again!");
            } else {
                texArguments << "extract" << ui->extractTEX0FolderPath->text() << ui->extractTEX0FilePath->text() << ui->extractPNGFolderPath->text();
                pwf2proc.start(texExecutable, texArguments);
                pwf2proc.waitForFinished(-1);
                if(!pwf2proc.waitForFinished()) {
                    /// In case something disastrous happens, this should(?) help.
                    while(pwf2proc.canReadLine()) {
                        pwf2log::writeToLogNN(LOG_PWF2TEX, pwf2proc.readLine().toStdString());
                    }
                    if (pwf2proc.exitStatus() != 0) {
                        pwf2proc.kill();
                        pwf2log::writeToLog(LOG_ERROR, "pwf2tex FAILED to run. Status code: " + QString(pwf2proc.exitStatus()).toStdString());
                        /// Not always given an error.
                        if(!pwf2proc.errorString().isEmpty() && !pwf2proc.errorString().isNull())
                            pwf2log::writeToLog(LOG_ERROR, "It seems pwf2tex threw an error too. Error: " + pwf2proc.errorString().toStdString());
                        pwf2info::showMessage("critical", "Error running pwf2tex!", "Something bad happened and pwf2tex failed to run! Error: " + QString(pwf2proc.error()));
                    } else {
                        pwf2log::writeToLog(LOG_INFO, "Successfully ran pwf2tes with executable " + texExecutable.toStdString() + " and arguments " + texArguments.join(" ").toStdString() + "!");
                        pwf2info::showMessage("info", "Done!", "Finished!");
                    }
                    if (pwf2info::closeWindowOnToolUse()) {
                        this->close();
                    }
                }
            }
        }
    } else {
        if (ui->injectPNGFolderPath->text().isNull() || ui->injectTEX0FilePath->text().isEmpty() || ui->injectTEX0FolderPath->text().isNull() || ui->injectPNGFolderPath->text().isEmpty() || ui->injectTEX0FilePath->text().isNull() || ui->injectTEX0FolderPath->text().isEmpty()) {
            pwf2log::writeToLog(LOG_WARNING, "FAILED to run pwf2tex with reason: not all arguments are filled in");
            pwf2info::showMessage("warning", "Error running pwf2tex!", "You must fill all boxes in!");
        } else {
            TEX0FileLocation.setFileName(ui->injectTEX0FilePath->text());
            if (!TEX0FileLocation.exists()) {
                pwf2log::writeToLog(LOG_ERROR, "FAILED to open file " + TEX0FileLocation.fileName().toStdString() + " with reason: " + TEX0FileLocation.errorString().toStdString());
                pwf2info::showMessage("critical", "Error opening TEX0 file!", "Please make sure the file you chose exists and try again!");
            } else {
                texArguments << "inject" << ui->injectTEX0FolderPath->text() << ui->injectTEX0FilePath->text() << ui->injectPNGFolderPath->text();
                pwf2proc.start(texExecutable, texArguments);
                pwf2proc.waitForFinished(-1);
                if(!pwf2proc.waitForFinished()) {
                    /// In case something disastrous happens, this should(?) help.
                    while(pwf2proc.canReadLine()) {
                        pwf2log::writeToLogNN(LOG_PWF2TEX, pwf2proc.readLine().toStdString());
                    }
                    if (pwf2proc.exitStatus() != 0) {
                        pwf2proc.kill();
                        pwf2log::writeToLog(LOG_ERROR, "pwf2tex FAILED to run. Status code: " + QString(pwf2proc.exitStatus()).toStdString());
                        /// Not always given an error.
                        if(!pwf2proc.errorString().isEmpty() && !pwf2proc.errorString().isNull())
                            pwf2log::writeToLog(LOG_ERROR, "It seems pwf2tex threw an error too. Error: " + pwf2proc.errorString().toStdString());
                        pwf2info::showMessage("critical", "Error running pwf2tex!", "Something bad happened and pwf2tex failed to run! Error: " + QString(pwf2proc.error()));
                    } else {
                        pwf2log::writeToLog(LOG_INFO, "Successfully ran pwf2tes with executable " + texExecutable.toStdString() + " and arguments " + texArguments.join(" ").toStdString() + "!");
                        pwf2info::showMessage("info", "Done!", "Finished!");
                    }
                    if (pwf2info::closeWindowOnToolUse()) {
                        this->close();
                    }
                }
            }
        }
    }
}

// This is just cleaner than junking up the QWidget's creation.

void pwf2tex::setUpButtons() {
    QObject::connect(ui->extractOpenTEX0File, SIGNAL(clicked()), this, SLOT(openTEX0File()));
    QObject::connect(ui->extractOpenTEX0Folder, SIGNAL(clicked()), this, SLOT(openTEX0Folder()));
    QObject::connect(ui->extractOpenPNGFolder, SIGNAL(clicked()), this, SLOT(openPNGFolder()));
    QObject::connect(ui->extractRun, SIGNAL(clicked()), this, SLOT(runTex()));
    QObject::connect(ui->injectOpenTEX0File, SIGNAL(clicked()), this, SLOT(openTEX0File()));
    QObject::connect(ui->injectOpenTEX0Folder, SIGNAL(clicked()), this, SLOT(openTEX0Folder()));
    QObject::connect(ui->injectOpenPNGFolder, SIGNAL(clicked()), this, SLOT(openPNGFolder()));
    QObject::connect(ui->injectRun, SIGNAL(clicked()), this, SLOT(runTex()));
}

void pwf2tex::updateRichPresence() {
    #ifdef ENABLE_RICHPRESENCE
        if(this->isActiveWindow()) {
            if(ui->texTabs->currentIndex() == 0)
                pwf2discord::updateStatus("Using pwf2tex!", "Extracting textures!");
            else
                pwf2discord::updateStatus("Using pwf2tex!", "Injecting textures!");
        }
    #endif
}
