#include "buttonthread.h"
#include "gpioclass.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include <iostream>
#include <QtCore>

GPIOClass* gpio21 = new GPIOClass("21"); //create new GPIO object to be attached to  GPIO21
string inputstate;


buttonThread::buttonThread(QObject *parent) :
    QThread(parent)
{
}

void buttonThread::run()
{
    msleep(100);

    std::cout << "ButtonThread is running" <<std::endl;

    gpio21->export_gpio(); //export GPIO21
    gpio21->setdir_gpio("in");

    while (true){


        QMutex mutex;
        mutex.lock();
        if(this->Stop) break;
        mutex.unlock();
        msleep(20);


    gpio21->getval_gpio(inputstate);

    if(inputstate == "0") // if input pin is at state "0" i.e. button pressed
            {
                std::cout << "input pin state is Pressed. Will check input pin state again in 10ms "<<endl;
                    usleep(10000);
                        std::cout << "Checking again ....." << endl;
                        gpio21->getval_gpio(inputstate); // checking again to ensure that state "0" is due to button press and not noise
                if(inputstate == "0")
                {
                    std::cout << "input pin state is definitely Pressed. " <<endl;

faultSwitch = true;
//emit faultHandling();
                   // cout << " Waiting until pin is unpressed....." << endl;
                    while (inputstate == "0"){
                    gpio21->getval_gpio(inputstate);
                    };
                    //cout << "pin is unpressed" << endl;

                }
               // else
                   // cout << "input pin state is definitely UnPressed. That was just noise." <<endl;


}}



}


