#ifndef CHANGERWINDOW_H
#define CHANGERWINDOW_H

#include <QDialog>

namespace Ui {
class ChangerWindow;
}

class ChangerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChangerWindow(QWidget *parent = 0);
    ~ChangerWindow();

    void RedrawGUI();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

    void on_scorePlus_clicked();

    void on_scoreMinus_clicked();

    void on_roundsPlus_clicked();

    void on_roundsMinus_clicked();

    void on_pin4_clicked();

    void on_pin7_clicked();

    void on_pin1_clicked();

    void on_pin2_clicked();

    void on_pin3_clicked();

    void on_pin5_clicked();

    void on_pin6_clicked();

    void on_pin8_clicked();

    void on_pin9_clicked();


private:
    Ui::ChangerWindow *ui;

};

#endif // CHANGERWINDOW_H
