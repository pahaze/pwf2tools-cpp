#ifndef PWF2DEBUG_H
#define PWF2DEBUG_H
// Qt headers \/
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QScrollBar>
#include <QString>
#include <QTextStream>
#include <QTimer>
// Non-Qt headers \/
#include "pwf2info.h"

namespace Ui {
    class pwf2debug;
}

class pwf2debug: public QDialog {
    Q_OBJECT
    public:
        explicit pwf2debug(QWidget *parent = nullptr);
        ~pwf2debug();
        void readOutput();
        void update();
    private:
        Ui::pwf2debug *ui;
        QString logOutput;
};

#endif // PWF2DEBUG_H
