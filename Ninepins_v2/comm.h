#ifndef COMM_H
#define COMM_H
#include <RF24/RF24.h>
#define WIRE 150
#include "global.h"




class Comm
{
public:
    Comm();

    //communication

    /*void loadMsg(message msg);
    message saveMsg();*/

    void sndMsg();
    void rcvMsg();
    void loadMsg();


};

#endif // COMM_H
