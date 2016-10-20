#ifndef FAULT_H
#define FAULT_H

#include <QDialog>

namespace Ui {
class fault;
}

class fault : public QDialog
{
    Q_OBJECT

public:
    explicit fault(QWidget *parent = 0);
    ~fault();


private slots:

    void on_faultConfirmedButton_clicked();

    void on_faultCancelledButton_clicked();

private:
    Ui::fault *ui;
};

#endif // FAULT_H
