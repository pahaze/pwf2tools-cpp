#ifndef PWF2OPTIONS_H
#define PWF2OPTIONS_H
// Qt headers \/
#include <QDialog>
#include <QDir>
#include <QMessageBox>
#include <QSettings>
// Non-Qt headers \/
#include "pwf2info.h"

namespace Ui {
    class pwf2options;
}

class pwf2options: public QDialog {
    Q_OBJECT
    public:
        explicit pwf2options(QWidget *parent = nullptr);
        ~pwf2options();
    private slots:
        void cancelButtonAction();
        void OKButtonAction();
        void readSettings();
        void setUpButtons();
        void writeSettings();
    private:
        Ui::pwf2options *ui;
};

#endif // PWF2OPTIONS_H
