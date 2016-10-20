#ifndef PARTIALGAMEWINDOW_H
#define PARTIALGAMEWINDOW_H

#include <QDialog>
#include "commthread.h"
#include "global.h"
#include "buttonthread.h"
#include "changerwindow.h"

namespace Ui {
class PartialGameWindow;
}

class PartialGameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PartialGameWindow(QWidget *parent = 0);
    ~PartialGameWindow();
    CommThread *commThread;
    buttonThread *bThread;


private slots:

    void on_pin1_clicked();

    void on_pin2_clicked();

    void on_pin3_clicked();

    void on_pin4_clicked();

    void on_pin5_clicked();

    void on_pin6_clicked();

    void on_pin7_clicked();

    void on_pin8_clicked();

    void on_pin9_clicked();

    void on_stavButton_clicked();

    void on_stavanieButton_clicked();

    void on_koniecButton_clicked();

private:
    Ui::PartialGameWindow *ui;

public slots:
    void onRedrawGUI();

};

#endif // PARTIALGAMEWINDOW_H
