#include "fullgameguithread.h"
#include <QtCore>

FullGameGuiThread::FullGameGuiThread(QObject *parent) :
    QThread(parent)
{

}

void FullGameGuiThread::run()
{
    while (true)
    {
    QMutex mutex;
    mutex.lock();
    if(this->Stop) break;
    mutex.unlock();

    emit redrawIcons();
    emit pointsChanged();
    emit roundsChanged();
    emit scoreChanged();
    msleep(200);

    emit redrawIcons();
    emit pointsChanged();
    emit roundsChanged();
    emit scoreChanged();

}}

