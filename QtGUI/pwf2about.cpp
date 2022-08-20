// Non-Qt headers \/
#include "pwf2about.h"
#include "ui_pwf2about.h"

pwf2about::pwf2about(QWidget *parent): QDialog(parent), ui(new Ui::pwf2about) {
    ui->setupUi(this);

    QPixmap pix(":/img/pwf2tools-scaled.png");
    ui->pwf2icon->setPixmap(pix.scaled(71, 71, Qt::KeepAspectRatio));

    QObject::connect(ui->leaveButton, SIGNAL(clicked()), this, SLOT(leaveWindow()));
    ui->versionLabel->setText("Version " + QString(MAJOR_VERSION) + "." + QString(MINOR_VERSION));
    ui->BuiltOnDateLabel->setText("Built on " + QDate::fromString(__DATE__, "MMM dd yyyy").toString("MMM dd, yyyy") + " at " + QString::fromStdString(__TIME__));
}

void pwf2about::leaveWindow() {
    pwf2about::close();
}

pwf2about::~pwf2about() {
    delete ui;
}
