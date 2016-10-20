#ifndef FULLGAMEGUITHREAD_H
#define FULLGAMEGUITHREAD_H
#include <QThread>
#include <QtCore>


class FullGameGuiThread: public QThread
{
    Q_OBJECT
public:
    explicit FullGameGuiThread(QObject *parent = 0);
    void run();
    bool Stop;
signals:
    void redrawIcons();
    void pointsChanged();
    void roundsChanged();
    void scoreChanged();

};

#endif // FULLGAMEGUITHREAD_H
