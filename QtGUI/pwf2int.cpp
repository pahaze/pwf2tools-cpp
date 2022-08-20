// Non-Qt headers \/
#include "pwf2int.h"
#include "ui_pwf2int.h"

pwf2int::pwf2int(QWidget *parent): QDialog(parent), ui(new Ui::pwf2int) {
    ui->setupUi(this);
    setUpButtons();
    #ifdef ENABLE_RICHPRESENCE
        QTimer *update = new QTimer(this);
        connect(update, &QTimer::timeout, this, &pwf2int::updateRichPresence);
        update->start(500);
    #endif
}

pwf2int::~pwf2int() {
    delete ui;
}

// Functions
/// All functions work with both tabs now

void pwf2int::openINT() {
    QString OpenIntPath;
    QFile intFile;
    /// Extract an INT
    if(ui->intTabs->currentIndex() == 0) {
        OpenIntPath = QFileDialog::getOpenFileName(this, tr("Open an INT"), "", tr("INT files (*.INT, *.int);;All Files (*)"));
        if (OpenIntPath.isEmpty()) {
            pwf2log::writeToLog(LOG_ERROR, "FAILED to open a file with reason: no file chosen");
            pwf2info::showMessage("critical", "Error opening file!", "Please choose a file!");
            return;
        } else {
            intFile.setFileName(OpenIntPath);
            if (!intFile.open(QIODevice::ReadWrite)) {
                pwf2log::writeToLog(LOG_ERROR, "FAILED to open file " + OpenIntPath.toStdString() + " with reason: " + intFile.errorString().toStdString());
                pwf2info::showMessage("critical", "Failed to open file! Try again...", intFile.errorString());
                return;
            }
            ui->extractIntPath->setText(intFile.fileName());
            pwf2log::writeToLog(LOG_INFO, "Successfully opened file at " + OpenIntPath.toStdString());
        }
    /// Create an INT
    } else {
        OpenIntPath = QFileDialog::getSaveFileName(this, tr("Save an INT"), "", tr("INT files (*.INT, *.int);;All Files (*)"));
        if (OpenIntPath.isEmpty()) {
            pwf2log::writeToLog(LOG_ERROR, "FAILED to save a file with reason: no file chosen");
            pwf2info::showMessage("critical", "Error creating INT!", "Please save a file!");
            return;
        } else {
            intFile.setFileName(OpenIntPath);
            if (!intFile.open(QIODevice::ReadWrite)) {
                pwf2log::writeToLog(LOG_ERROR, "FAILED to save file " + OpenIntPath.toStdString() + " with reason: " + intFile.errorString().toStdString());
                pwf2info::showMessage("critical", "Failed to save file! Try again...", intFile.errorString());
                return;
            }
            ui->createIntPath->setText(intFile.fileName());
            pwf2log::writeToLog(LOG_INFO, "Successfully saved file at " + OpenIntPath.toStdString());
        }
    }
}

void pwf2int::openFolder() {
    QString OpenFolderPath;
    QDir intFolder;
    /// Extract an INT
    if(ui->intTabs->currentIndex() == 0) {
        OpenFolderPath = QFileDialog::getExistingDirectory(this, tr("Open a folder to extract to!"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (OpenFolderPath.isEmpty()) {
            pwf2log::writeToLog(LOG_ERROR, "FAILED to open a folder with reason: no folder chosen");
            pwf2info::showMessage("critical", "Error opening folder!", "Please choose a folder!");
            return;
        } else {
            intFolder.setPath(OpenFolderPath);
            if (!intFolder.exists()) {
                intFolder.mkpath(OpenFolderPath);
                pwf2log::writeToLog(LOG_WARNING, "Folder at " + OpenFolderPath.toStdString() + " doesn't exist!");
                pwf2log::writeToLog(LOG_INFO, "Creating folder at " + OpenFolderPath.toStdString() + "...");
            }
            ui->extractFolderPath->setText(intFolder.path());
            pwf2log::writeToLog(LOG_INFO, "Successfully opened folder at " + OpenFolderPath.toStdString());
        }
    /// Create an INT
    } else {
        //// I didn't actually realize this until now, but in v14, this was also set to "Open a folder to extract to!". I genuinely don't know how I missed that.
        OpenFolderPath = QFileDialog::getExistingDirectory(this, tr("Open the folder with your modded INT contents!"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (OpenFolderPath.isEmpty()) {
            pwf2log::writeToLog(LOG_ERROR, "FAILED to open a folder with reason: no folder chosen");
            pwf2info::showMessage("critical", "Error opening folder!", "Please choose a folder!");
            return;
        } else {
            QDir dir(OpenFolderPath);
            if (!dir.exists()) {
                dir.mkdir(OpenFolderPath);
                pwf2log::writeToLog(LOG_WARNING, "Folder at " + OpenFolderPath.toStdString() + " doesn't exist!");
                pwf2log::writeToLog(LOG_INFO, "Creating folder at " + OpenFolderPath.toStdString() + "...");
            }
            ui->createFolderPath->setText(dir.path());
            pwf2log::writeToLog(LOG_INFO, "Successfully opened folder at " + OpenFolderPath.toStdString());
        }
    }
}

void pwf2int::runInt() {
    QStringList intArguments;
    QFile intLocation;
    QProcess pwf2proc;
    /// Extract an INT
    if(ui->intTabs->currentIndex() == 0) {
        if (ui->extractIntPath->text().isNull() || ui->extractFolderPath->text().isNull() || ui->extractIntPath->text().isEmpty() || ui->extractFolderPath->text().isEmpty()) {
            pwf2log::writeToLog(LOG_WARNING, "FAILED to run pwf2int with reason: not all arguments are filled in");
            pwf2info::showMessage("warning", "Error running pwf2int!", "You must fill all boxes in!");
        } else {
            intLocation.setFileName(ui->extractIntPath->text());
            if (!intLocation.exists()) {
                pwf2log::writeToLog(LOG_ERROR, "FAILED to open file " + intLocation.fileName().toStdString() + " with reason: " + intLocation.errorString().toStdString());
                pwf2info::showMessage("critical", "Error opening INT file!", "Please make sure the file you chose exists and try again!");
            } else {
                intArguments << "extract" << ui->extractIntPath->text() << ui->extractFolderPath->text();
                pwf2proc.start(intExecutable, intArguments);
                pwf2proc.waitForFinished(-1);
                if(!pwf2proc.waitForFinished()) {
                    /// In case something disastrous happens, this should(?) help.
                    while(pwf2proc.canReadLine()) {
                        pwf2log::writeToLogNN(LOG_PWF2INT, pwf2proc.readLine().toStdString());
                    }
                    if (pwf2proc.exitStatus() != 0) {
                        pwf2proc.kill();
                        pwf2log::writeToLog(LOG_ERROR, "pwf2int FAILED to run. Status code: " + QString(pwf2proc.exitStatus()).toStdString());
                        /// Not always given an error.
                        if(!pwf2proc.errorString().isEmpty() && !pwf2proc.errorString().isNull())
                            pwf2log::writeToLog(LOG_ERROR, "It seems pwf2int threw an error too. Error: " + pwf2proc.errorString().toStdString());
                        pwf2info::showMessage("critical", "Error running pwf2int!", "Something bad happened and pwf2int failed to run! Error: " + QString(pwf2proc.error()));
                    } else {
                        pwf2log::writeToLog(LOG_INFO, "Successfully ran pwf2int with executable " + intExecutable.toStdString() + " and arguments " + intArguments.join(" ").toStdString() + "!");
                        pwf2info::showMessage("info", "Done!", "Finished!");
                    }
                    if (pwf2info::closeWindowOnToolUse()) {
                        this->close();
                    }
                }
            }
        }
    /// Create an INT
    } else {
        if (ui->createIntPath->text().isNull() || ui->createFolderPath->text().isNull() || ui->createIntPath->text().isEmpty() || ui->createFolderPath->text().isEmpty()) {
            pwf2log::writeToLog(LOG_WARNING, "FAILED to run pwf2int with reason: not all arguments are filled in");
            pwf2info::showMessage("warning", "Error running pwf2int!", "You must fill all boxes in!");
        } else {
            intArguments << "create" << ui->createIntPath->text() << ui->createFolderPath->text();
            pwf2proc.start(intExecutable, intArguments);
            pwf2proc.waitForFinished(-1);
            if(!pwf2proc.waitForFinished()) {
                /// In case something disastrous happens, this should(?) help.
                while(pwf2proc.canReadLine()) {
                    pwf2log::writeToLogNN(LOG_PWF2INT, pwf2proc.readLine().toStdString());
                }
                if (pwf2proc.exitStatus() != 0) {
                    pwf2proc.kill();
                    pwf2log::writeToLog(LOG_ERROR, "pwf2int FAILED to run. Status code: " + QString(pwf2proc.exitStatus()).toStdString());
                    /// Not always given an error.
                    if(!pwf2proc.errorString().isEmpty() && !pwf2proc.errorString().isNull())
                        pwf2log::writeToLog(LOG_ERROR, "It seems pwf2int threw an error too. Error: " + pwf2proc.errorString().toStdString());
                    pwf2info::showMessage("critical", "Error running pwf2int!", "Something bad happened and pwf2int failed to run! Error: " + QString(pwf2proc.error()));
                } else {
                    pwf2log::writeToLog(LOG_INFO, "Successfully ran pwf2int with executable " + intExecutable.toStdString() + " and arguments " + intArguments.join(" ").toStdString() + "!");
                    pwf2info::showMessage("info", "Done!", "Finished!");
                }
                if (pwf2info::closeWindowOnToolUse()) {
                    this->close();
                }
            }
        }
    }
}

// This is just cleaner than junking up the QWidget's creation.

void pwf2int::setUpButtons() {
    QObject::connect(ui->extractOpenInt, SIGNAL(clicked()), this, SLOT(openINT()));
    QObject::connect(ui->extractOpenFolder, SIGNAL(clicked()), this, SLOT(openFolder()));
    QObject::connect(ui->extractRun, SIGNAL(clicked()), this, SLOT(runInt()));
    QObject::connect(ui->createSaveInt, SIGNAL(clicked()), this, SLOT(openINT()));
    QObject::connect(ui->createOpenFolder, SIGNAL(clicked()), this, SLOT(openFolder()));
    QObject::connect(ui->createRun, SIGNAL(clicked()), this, SLOT(runInt()));
}

void pwf2int::updateRichPresence() {
    #ifdef ENABLE_RICHPRESENCE
        if(this->isActiveWindow()) {
            if(ui->intTabs->currentIndex() == 0)
                pwf2discord::updateStatus("Using pwf2int!", "Extracting an INT!");
            else
                pwf2discord::updateStatus("Using pwf2int!", "Creating an INT!");
        }
    #endif
}
