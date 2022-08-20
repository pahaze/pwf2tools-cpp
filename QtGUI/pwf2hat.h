#ifndef PWF2HAT_H
#define PWF2HAT_H
// Qt headers \/
#include <QDialog>

namespace Ui {
    class pwf2hat;
}

class pwf2hat: public QDialog {
    Q_OBJECT
    public:
        explicit pwf2hat(QWidget *parent = nullptr);
        ~pwf2hat();
    private:
        Ui::pwf2hat *ui;
};

#endif    // PWF2HAT_H
