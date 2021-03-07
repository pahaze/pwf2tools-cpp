#ifndef PWF2INT_H
#define PWF2INT_H

#include <QDialog>

namespace Ui {
    class pwf2int;
}

class pwf2int : public QDialog {
    Q_OBJECT

    public:
    explicit pwf2int(QWidget *parent = nullptr);
    ~pwf2int();

    private slots:
    void openINT();
    void opnFolderExt();
    void runExt();
    void openINTCrt();
    void opnFolderCrt();
    void runCrt();
    QString getOS();

    private:
    Ui::pwf2int *ui;
};

#endif    // PWF2INT_H
