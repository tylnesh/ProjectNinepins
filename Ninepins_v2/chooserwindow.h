#ifndef CHOOSERWINDOW_H
#define CHOOSERWINDOW_H

#include <QDialog>

namespace Ui {
class ChooserWindow;
}

class ChooserWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChooserWindow(QWidget *parent = 0);
    ~ChooserWindow();

private slots:
    void on_plnaButton_clicked();
    
    void on_dorazkaButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::ChooserWindow *ui;
};

#endif // CHOOSERWINDOW_H
