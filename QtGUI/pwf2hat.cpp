// Non-Qt headers \/
#include "pwf2hat.h"
#include "ui_pwf2hat.h"

pwf2hat::pwf2hat(QWidget *parent): QDialog(parent), ui(new Ui::pwf2hat) {
    ui->setupUi(this);
}

pwf2hat::~pwf2hat() {
    delete ui;
}
