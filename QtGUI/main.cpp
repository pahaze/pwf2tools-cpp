#include "pwf2main.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    pwf2main w;
    w.show();
    return a.exec();
}
