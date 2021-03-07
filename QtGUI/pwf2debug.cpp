#include "pwf2debug.h"
#include "ui_pwf2debug.h"

pwf2debug::pwf2debug(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pwf2debug)
{
    ui->setupUi(this);
}

pwf2debug::~pwf2debug()
{
    delete ui;
}
