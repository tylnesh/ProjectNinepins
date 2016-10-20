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
#include "comm.h"
#include <RF24/RF24.h>
#include <unistd.h>
#include <iostream>
#include "global.h"

#define WIRE 150  // pairing the Arduino with RPI

using namespace std;

struct ardMsg
{

    int cmd=0;
    bool pins[9]={false,false,false,false,false,false,false,false,false};
    int wire = WIRE;
    int rounds=0;

};

ardMsg incoming;
ardMsg outgoing;

    const uint64_t addresses[2] = { 0xABCDABCD71LL, 0x544d52687CLL };


    RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);



Comm::Comm()
{

    radio.begin();                           // Setup and configure rf radio
    radio.setChannel(1);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_250KBPS);
    radio.setAutoAck(1);                     // Ensure autoACK is enabled
    radio.setRetries(5,30);                  // Optionally, increase the delay between retries & # of retries
    radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance
    radio.printDetails();
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);


}



void Comm::sndMsg() {
    outgoing.cmd = cmdOutGF;
    outgoing.wire = WIRE;
    radio.stopListening();
    printf("Sending message...\n");
    radio.write(&outgoing,sizeof(ardMsg));
}

void Comm::loadMsg() {
    outgoing.cmd = cmdOutGF;
    outgoing.wire = WIRE;
    outgoing.rounds = roundsOutGF;
    for (int i = 0; i< 9; i++) outgoing.pins[i] = pinsOutGF[i];
    }


void Comm::rcvMsg() {
    radio.startListening();
    while (radio.available())
    {
        radio.read(&incoming,sizeof(ardMsg));

std::cout<< "wire: " << incoming.wire << " cmd: " <<incoming.cmd << std::endl;

         if (incoming.wire == WIRE){

         std::cout<< "wire: " << incoming.wire << " cmd: " << incoming.cmd << std::endl;
         cmdGF = incoming.cmd;

         if (incoming.rounds != currentRound) {

             if (!isPartial) {

                 pointsGF = 0;
                 for (int i = 0; i<9; i++) {pinsGF[i] = incoming.pins[i]; if (pinsGF[i]) pointsGF++;}
                 roundsGF = currentRound;
                 scoreGF += pointsGF;
                 currentRound = incoming.rounds;
                 roundsGF = currentRound;
             }

             else {
                 pointsGF = 0;
                 for (int i = 0; i<9; i++) { pinsGF[i] = incoming.pins[i]; if (pinsGF[i]) pointsGF++;}
                 roundsGF = currentRound;

                 if (pointsGF == 9)
                 {scoreGF += pointsGF - lastPoints;  lastPoints = 0; pointsGF = 0;}
                 else {
                 scoreGF += pointsGF - lastPoints;
                 lastPoints  = pointsGF;
                 }
                 currentRound = incoming.rounds;
                 roundsGF = currentRound;

             }

         }
         else currentRound = incoming.rounds;

         }

 }

}

