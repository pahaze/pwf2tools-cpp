#ifndef PWF2TEX_H
#define PWF2TEX_H

#include <QDialog>
#include <QString>

namespace Ui {
    class pwf2tex;
}

class pwf2tex : public QDialog {
    Q_OBJECT

    public:
    explicit pwf2tex(QWidget *parent = nullptr);
    ~pwf2tex();

    private:
    Ui::pwf2tex *ui;

    private slots:
    void extOpenTex0Folder();
    void extOpenTex0File();
    void extOpenPngFolder();
    void extFinalize();
    void injOpenTex0Folder();
    void injOpenTex0File();
    void injOpenPngFolder();
    void injFinalize();
    QString getOS();
};

#endif    // PWF2TEX_H
