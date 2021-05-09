#include "pwf2about.h"
#include "pwf2main.h"
#include "ui_pwf2about.h"
#include <QLabel>
#include <QObject>
#include <QPixmap>

pwf2about::pwf2about(QWidget *parent) : QDialog(parent),
                                        ui(new Ui::pwf2about) {
    ui->setupUi(this);

    QPixmap pix(":/img/pwf2tools-scaled.png");
    ui->pwf2ico->setPixmap(pix.scaled(71, 71, Qt::KeepAspectRatio));

    QObject::connect(ui->leaveButton, SIGNAL(clicked()), this, SLOT(leaveWindow()));
    ui->BuiltOnDateLabel->setText("Built on " + QString::fromStdString(__DATE__) + " at " + QString::fromStdString(__TIME__));
}

void pwf2about::leaveWindow() {
    pwf2about::close();
}

pwf2about::~pwf2about() {
    delete ui;
}
