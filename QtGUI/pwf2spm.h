#ifndef PWF2SPM_H
#define PWF2SPM_H

#include <QDialog>

namespace Ui {
    class pwf2spm;
}

class pwf2spm : public QDialog {
    Q_OBJECT

    public:
    explicit pwf2spm(QWidget *parent = nullptr);
    ~pwf2spm();

    private slots:
    void openSPM();
    void saveTex0();
    void saveFinal();
    QString getOS();

    private:
    Ui::pwf2spm *ui;
};

#endif    // PWF2SPM_H
