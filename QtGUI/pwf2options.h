#ifndef PWF2OPTIONS_H
#define PWF2OPTIONS_H

#include <QDialog>
#include <QSettings>

namespace Ui {
    class pwf2options;
}

class pwf2options : public QDialog {
    Q_OBJECT

    public:
    explicit pwf2options(QWidget *parent = nullptr);
    ~pwf2options();

    private slots:
    void cancelButtonAction();
    void OKButtonAction();
    void readSettings();
    void writeSettings();

    private:
    Ui::pwf2options *ui;
};

#endif    // PWF2OPTIONS_H
