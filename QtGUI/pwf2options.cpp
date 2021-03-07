#include "pwf2options.h"
#include "ui_pwf2options.h"
#include <QMessageBox>

pwf2options::pwf2options(QWidget *parent) : QDialog(parent),
                                            ui(new Ui::pwf2options) {
    ui->setupUi(this);
    QObject::connect(ui->OkButton, SIGNAL(clicked()), this, SLOT(OKButtonAction()));
    QObject::connect(ui->CancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonAction()));
}

void pwf2options::cancelButtonAction() {
    this->close();
}

void pwf2options::OKButtonAction() {
    QMessageBox msg;
    msg.information(0, "Not finished yet!", "This dialog doesn't do anything yet!");
    msg.setFixedSize(500, 200);
    this->close();
}

pwf2options::~pwf2options() {
    delete ui;
}
