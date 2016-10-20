/*
This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "commthread.h"
#include <QtCore>
#include "comm.h"
#include <iostream>
#include "fullgamewindow.h"
#include "gpioclass.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "global.h"

#include <QtCore>

GPIOClass* gpio21 = new GPIOClass("21"); //create new GPIO object to be attached to  GPIO21
string inputstate;


#define RX true
#define TX false
#define QUIT 3



Comm ct;

CommThread::CommThread(QObject *parent) :
    QThread(parent)
{
mode = RX;
}

void CommThread::run()

{
    gpio21->export_gpio(); //export GPIO21
    gpio21->setdir_gpio("in");
    int timer = 0;
    running = true;
    msleep(500);
    std::cout << "CommThread is running" <<std::endl;

    while (running)
    {
        //QMutex mutex;
       // mutex.lock();
        if(this->Stop) break;
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
    while (inputstate == "0"){
    gpio21->getval_gpio(inputstate);
    };

                    }
        }


        //mutex.unlock();
        //msleep(10);
timer++;
if (timer > 500) { emit redrawGUI(); timer = 0;}

        switch (mode){
        case RX:
            //std::cout<<"RX mode"<<std::endl;
            ct.rcvMsg();
            break;

        case TX:
            std::cout<<"TX mode"<<std::endl;
            if (changer) ct.loadMsg();
            //for (int i = 0; i <3; i++)
            ct.sndMsg();
            mode = RX;
            break;

        default:
            break;
        }
            }



}

