#ifndef PWF2DEBUG_H
#define PWF2DEBUG_H

#include <QDialog>

namespace Ui {
class pwf2debug;
}

class pwf2debug : public QDialog
{
    Q_OBJECT

public:
    explicit pwf2debug(QWidget *parent = nullptr);
    ~pwf2debug();

private:
    Ui::pwf2debug *ui;
};

#endif // PWF2DEBUG_H
