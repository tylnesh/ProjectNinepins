#ifndef COMMTHREAD_H
#define COMMTHREAD_H
#include <QThread>
#define WIRE 150
#include "comm.h"
#include "global.h"


class CommThread: public QThread
{
    Q_OBJECT
public:
   explicit CommThread(QObject *parent = 0);
   Comm cm;
   bool mode;

   bool Stop;
   void run();
   bool running;


signals:
    void redrawGUI();
public slots:

};

#endif // COMMTHREAD_H
