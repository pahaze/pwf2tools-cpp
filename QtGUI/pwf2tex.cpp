#include "pwf2tex.h"
#include "ui_pwf2tex.h"
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QString>

pwf2tex::pwf2tex(QWidget *parent) : QDialog(parent),
                                    ui(new Ui::pwf2tex) {
    ui->setupUi(this);
    QObject::connect(ui->extOpenTex0FolderButton, SIGNAL(clicked()), this, SLOT(extOpenTex0Folder()));
    QObject::connect(ui->extOpenTex0FileButton, SIGNAL(clicked()), this, SLOT(extOpenTex0File()));
    QObject::connect(ui->extOpenPngFolderButton, SIGNAL(clicked()), this, SLOT(extOpenPngFolder()));
    QObject::connect(ui->extFinalizeExtractButton, SIGNAL(clicked()), this, SLOT(extFinalize()));
    QObject::connect(ui->injOpenTex0FolderButton, SIGNAL(clicked()), this, SLOT(injOpenTex0Folder()));
    QObject::connect(ui->injOpenTex0FileButton, SIGNAL(clicked()), this, SLOT(injOpenTex0File()));
    QObject::connect(ui->injOpenPngFolderButton, SIGNAL(clicked()), this, SLOT(injOpenPngFolder()));
    QObject::connect(ui->injFinalizeInjectButton, SIGNAL(clicked()), this, SLOT(injFinalize()));
}

pwf2tex::~pwf2tex() {
    delete ui;
}

QString pwf2tex::getOS() {
#if defined(Q_OS_MACOS)
    return QDir::currentPath() + "/bin/pwf2tex";
#elif defined(Q_OS_WIN)
    return QDir::currentPath() + "/bin/pwf2tex.exe";
#elif defined(Q_OS_LINUX)
    return QDir::currentPath() + "/bin/pwf2tex";
#elif defined(Q_OS_UNIX)
    return QDir::currentPath() + "/bin/pwf2tex";
#else
    return QDir::currentPath() + "/bin/pwf2tex";
#endif
}

void pwf2tex::extOpenTex0Folder() {
    QString Tex0Folder = QFileDialog::getExistingDirectory(this, tr("Open the TEXTURES folder (from INT)!"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (Tex0Folder.isEmpty()) {
        QMessageBox noFolderError;
        noFolderError.critical(0, "Error opening folder!", "Please choose a folder!");
        noFolderError.setFixedSize(500, 200);
        return;
    } else {
        QDir dir(Tex0Folder);
        if (!dir.exists()) {
            dir.mkdir(Tex0Folder);
            return;
        }
        ui->extTex0FolderLineEdit->setText(dir.path());
    }
}

void pwf2tex::extOpenTex0File() {
    QString Tex0File = QFileDialog::getOpenFileName(this, tr("Open a tex0 file!"), "", tr("tex0 files (*.tex0);;All Files (*)"));
    if (Tex0File.isEmpty()) {
        QMessageBox noFileError;
        noFileError.critical(0, "Error opening file!", "Please choose a file!");
        noFileError.setFixedSize(500, 200);
        return;
    } else {
        QFile file(Tex0File);
        if (!file.open(QIODevice::ReadWrite)) {
            QMessageBox::information(this, tr("Failed to open file! Try again.."), file.errorString());
            return;
        }
        ui->extTex0FileLineEdit->setText(file.fileName());
    }
}

void pwf2tex::extOpenPngFolder() {
    QString PngFolder = QFileDialog::getExistingDirectory(this, tr("Open a folder to extract PNGs to!"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (PngFolder.isEmpty()) {
        QMessageBox noFolderError;
        noFolderError.critical(0, "Error opening folder!", "Please choose a folder!");
        noFolderError.setFixedSize(500, 200);
        return;
    } else {
        QDir dir(PngFolder);
        if (!dir.exists()) {
            dir.mkdir(PngFolder);
            return;
        }
        ui->extPngFolderLineEdit->setText(dir.path());
    }
}

void pwf2tex::extFinalize() {
    QProcess pwf2proc;
    if (ui->extPngFolderLineEdit->text().isNull() || ui->extPngFolderLineEdit->text().isEmpty() || ui->extTex0FileLineEdit->text().isNull() || ui->extTex0FileLineEdit->text().isEmpty() || ui->extTex0FolderLineEdit->text().isNull() || ui->extTex0FolderLineEdit->text().isEmpty()) {
        QMessageBox emptyBoxError;
        emptyBoxError.critical(0, "Error running pwf2tex!", "You must fill all boxes in!");
        emptyBoxError.setFixedSize(500, 200);
    } else {
        QFile texfile(ui->extTex0FileLineEdit->text());
        if (!texfile.exists()) {
            QMessageBox nonExistantFile;
            nonExistantFile.critical(0, "Error opening tex0 file.",
                "Please make sure the file you "
                "chose exists and try again.");
            nonExistantFile.setFixedSize(500, 200);
        } else {
            QString pwf2texeloc = pwf2tex::getOS();
            QString args = " extract \"" + ui->extTex0FolderLineEdit->text() + "\" \"" + ui->extTex0FileLineEdit->text() + "\" \"" + ui->extPngFolderLineEdit->text() + "\"";
            pwf2proc.execute(pwf2texeloc + args);
            pwf2proc.waitForFinished(-1);
            if (pwf2proc.waitForFinished() && pwf2proc.exitStatus() != QProcess::NormalExit) {
                pwf2proc.kill();
                QMessageBox criticalRunError;
                criticalRunError.critical(0, "Error running pwf2tex!", "Something bad happened and pwf2tex failed to run! Error: " + pwf2proc.error());
                criticalRunError.setFixedSize(500, 200);
            } else {
                QMessageBox successMessage;
                successMessage.information(0, "Done!", "Finished!");
                successMessage.setFixedSize(500, 200);
            }
            QSettings pwf2settings((QDir(QApplication::applicationDirPath()).filePath("settings.ini")), QSettings::IniFormat);
            if (pwf2settings.value("User_Experience/CloseFormOnToolUse", true).toBool() == true) {
                this->close();
            }
        }
    }
}

void pwf2tex::injOpenTex0Folder() {
    QString Tex0Folder = QFileDialog::getExistingDirectory(this, tr("Open the TEXTURES folder (from INT)!"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (Tex0Folder.isEmpty()) {
        QMessageBox noFolderError;
        noFolderError.critical(0, "Error opening folder!", "Please choose a folder!");
        noFolderError.setFixedSize(500, 200);
        return;
    } else {
        QDir dir(Tex0Folder);
        if (!dir.exists()) {
            dir.mkdir(Tex0Folder);
            return;
        }
        ui->injTex0FolderLineEdit->setText(dir.path());
    }
}

void pwf2tex::injOpenTex0File() {
    QString Tex0File = QFileDialog::getOpenFileName(this, tr("Open a tex0 file!"), "", tr("tex0 files (*.tex0);;All Files (*)"));
    if (Tex0File.isEmpty()) {
        QMessageBox noFileError;
        noFileError.critical(0, "Error opening file!", "Please choose a file!");
        noFileError.setFixedSize(500, 200);
        return;
    } else {
        QFile file(Tex0File);
        if (!file.open(QIODevice::ReadWrite)) {
            QMessageBox::information(this, tr("Failed to open file! Try again.."), file.errorString());
            return;
        }
        ui->injTex0FileLineEdit->setText(file.fileName());
    }
}

void pwf2tex::injOpenPngFolder() {
    QString PngFolder = QFileDialog::getExistingDirectory(this, tr("Open a folder to extract PNGs to!"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (PngFolder.isEmpty()) {
        QMessageBox noFolderError;
        noFolderError.critical(0, "Error opening folder!", "Please choose a folder!");
        noFolderError.setFixedSize(500, 200);
        return;
    } else {
        QDir dir(PngFolder);
        if (!dir.exists()) {
            dir.mkdir(PngFolder);
            return;
        }
        ui->injPngFolderLineEdit->setText(dir.path());
    }
}

void pwf2tex::injFinalize() {
    QProcess pwf2proc;
    if (ui->injPngFolderLineEdit->text().isNull() || ui->injPngFolderLineEdit->text().isEmpty() || ui->injTex0FileLineEdit->text().isNull() || ui->injTex0FileLineEdit->text().isEmpty() || ui->injTex0FolderLineEdit->text().isNull() || ui->injTex0FolderLineEdit->text().isEmpty()) {
        QMessageBox emptyBoxError;
        emptyBoxError.critical(0, "Error running pwf2tex!", "You must fill all boxes in!");
        emptyBoxError.setFixedSize(500, 200);
    } else {
        QFile texfile(ui->injTex0FileLineEdit->text());
        if (!texfile.exists()) {
            QMessageBox nonExistantFile;
            nonExistantFile.critical(0, "Error opening tex0 file.",
                "Please make sure the file you "
                "chose exists and try again.");
            nonExistantFile.setFixedSize(500, 200);
        } else {
            QString pwf2texeloc = pwf2tex::getOS();
            QString args = " inject \"" + ui->injTex0FolderLineEdit->text() + "\" \"" + ui->injTex0FileLineEdit->text() + "\" \"" + ui->injPngFolderLineEdit->text() + "\"";
            pwf2proc.execute(pwf2texeloc + args);
            pwf2proc.waitForFinished(-1);
            if (pwf2proc.waitForFinished() && pwf2proc.exitStatus() != QProcess::NormalExit) {
                pwf2proc.kill();
                QMessageBox criticalRunError;
                criticalRunError.critical(0, "Error running pwf2tex!", "Something bad happened and pwf2tex failed to run! Error: " + pwf2proc.error());
                criticalRunError.setFixedSize(500, 200);
            } else {
                QMessageBox successMessage;
                successMessage.information(0, "Done!", "Finished!");
                successMessage.setFixedSize(500, 200);
            }
            QSettings pwf2settings((QDir(QApplication::applicationDirPath()).filePath("settings.ini")), QSettings::IniFormat);
            if (pwf2settings.value("User_Experience/CloseFormOnToolUse", true).toBool() == true) {
                this->close();
            }
        }
    }
}
