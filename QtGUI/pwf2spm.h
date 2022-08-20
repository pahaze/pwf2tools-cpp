#ifndef PWF2SPM_H
#define PWF2SPM_H
// Qt headers \/
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QTimer>
// Non-Qt headers \/
#include "pwf2discord.h"
#include "pwf2info.h"
#include "pwf2log.h"

namespace Ui {
    class pwf2spm;
}

class pwf2spm: public QDialog {
    Q_OBJECT
    public:
        explicit pwf2spm(QWidget *parent = nullptr);
        ~pwf2spm();
    private slots:
        void openSPM();
        void runSpm();
        void saveTex0();
        void setUpButtons();
        void updateRichPresence();
    private:
        Ui::pwf2spm *ui;
        QString spmExecutable = pwf2info::getOSTool("pwf2spm");
};

#endif // PWF2SPM_H
