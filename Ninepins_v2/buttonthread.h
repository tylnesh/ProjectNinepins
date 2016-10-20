#ifndef BUTTONTHREAD_H
#define BUTTONTHREAD_H
#include <QThread>
#include <QtCore>

class buttonThread: public QThread
{
    Q_OBJECT
public:
    explicit buttonThread(QObject *parent = 0);
    void run();
    bool Stop;
signals:

};

#endif // BUTTONTHREAD_H
