#ifndef PWF2ABOUT_H
#define PWF2ABOUT_H

#include <QDialog>

namespace Ui {
    class pwf2about;
}

class pwf2about : public QDialog {
    Q_OBJECT

    public:
    explicit pwf2about(QWidget *parent = nullptr);
    ~pwf2about();

    private slots:
    void leaveWindow();

    private:
    Ui::pwf2about *ui;
};

#endif    // PWF2ABOUT_H
