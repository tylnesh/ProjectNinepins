#include "message.h"
#define WIRE 150
#include <iostream>
#include "global.h"

int cmd;
bool pins[9] = {false,false,false,false,false,false,false,false,false};
int wire;
int rounds;
int points;
int score;

message::message()
{

}


int message::getCmd() {
    return cmd;
}
int message::getWire() {
    return wire;
}
int message::getPoints() {
    return points;
}
int message::getRounds() {
    return rounds;
}
int message::getScore() {
    return score;
}

void message::getPins(bool b[]){
   for (int i=0; i<9; i++) b[i] = pins[i];

}


void message::setCmd(int c){
    //std::cout<<"Setting up command with "<< c << std::endl;
    cmd = c;
    //std::cout<<"Command set as "<< cmd << std::endl;
}
void message::setWire(int w){wire = w;}
void message::setPoints(int p){points  = p;}
void message::setRounds(int r){rounds = r;}
void message::setScore(int s){ score = s;}
void message::setPins(bool b[]){

    for (int i=0; i<9; i++) pins[i] = b[i];
}


void message::delMsg()
{
    int d = 0;
    bool dd[9] = {false,false,false,false,false,false,false,false,false};

    cmd =d;
    wire =d;
    points =d;
    rounds =d;
    score =d;
    for (int i=0; i<9; i++) pins[i] = dd[i];

}


void message::setMsg()
{
    cmd = cmdOutGF;
    wire = WIRE;

}
