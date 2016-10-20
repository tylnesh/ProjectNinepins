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
#include "partialgamewindow.h"
#include "ui_partialgamewindow.h"
#include "comm.h"
#include "commthread.h"
#include <QtCore>
#include <iostream>
#include "global.h"
#include "fault.h"
#include "changerwindow.h"

using namespace std;
Comm commPG;

PartialGameWindow::PartialGameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PartialGameWindow)
{
    isPartial = true;
    pointsGF = 0;
    scoreGF = 0;
    roundsGF = 0;

    ui->setupUi(this);
    this ->setWindowFlags(Qt::Window);
    this ->showFullScreen();
    this ->setWindowTitle("PARTIAL GAME");

    commThread = new CommThread(this);
    connect(commThread,SIGNAL(redrawGUI()),this,SLOT(onRedrawGUI()));
    commThread->start();


}

PartialGameWindow::~PartialGameWindow()
{
    delete ui;
}

//redrawing pins icons || prekreslovanie ikoniek kolkov

void PartialGameWindow::onRedrawGUI()
{
    ui->scoreLCD->display(scoreGF);
    ui->roundsLCD->display(roundsGF);
    ui->pointsLCD->display(pointsGF);

    if (!pinsGF[0]) ui->pin1->setIcon(QIcon(":/images/kolok1.png")); else ui->pin1->setIcon(QIcon(":/images/kolok2.png"));
    if (!pinsGF[1]) ui->pin2->setIcon(QIcon(":/images/kolok1.png")); else ui->pin2->setIcon(QIcon(":/images/kolok2.png"));
    if (!pinsGF[2]) ui->pin3->setIcon(QIcon(":/images/kolok1.png")); else ui->pin3->setIcon(QIcon(":/images/kolok2.png"));
    if (!pinsGF[3]) ui->pin4->setIcon(QIcon(":/images/kolok1.png")); else ui->pin4->setIcon(QIcon(":/images/kolok2.png"));
    if (!pinsGF[4]) ui->pin5->setIcon(QIcon(":/images/kolok1.png")); else ui->pin5->setIcon(QIcon(":/images/kolok2.png"));
    if (!pinsGF[5]) ui->pin6->setIcon(QIcon(":/images/kolok1.png")); else ui->pin6->setIcon(QIcon(":/images/kolok2.png"));
    if (!pinsGF[6]) ui->pin7->setIcon(QIcon(":/images/kolok1.png")); else ui->pin7->setIcon(QIcon(":/images/kolok2.png"));
    if (!pinsGF[7]) ui->pin8->setIcon(QIcon(":/images/kolok1.png")); else ui->pin8->setIcon(QIcon(":/images/kolok2.png"));
    if (!pinsGF[8]) ui->pin9->setIcon(QIcon(":/images/kolok1.png")); else ui->pin9->setIcon(QIcon(":/images/kolok2.png"));

    if (faultSwitch && !faultRunning) {
        fault f;
        cmdOutGF = -1;
        commThread->mode = false;
        faultRunning = true;

            f.exec();
            while (faultRunning){}
            commThread->mode = false;

    }

    if(
            pinsGF[0] &&
            pinsGF[1] &&
            pinsGF[2] &&
            pinsGF[3] &&
            pinsGF[4] &&
            pinsGF[5] &&
            pinsGF[6] &&
            pinsGF[7] &&
            pinsGF[8]
       )
    {
        for (int i = 0; i< 2000000; i++) {}
        for (int i =0; i<9; i++) pinsGF[i] = false;}


}

// controlling the pins via push buttons || ovladanie kolkov dotykovymi tlacitkami

void PartialGameWindow::on_pin1_clicked()
{
 //  if (pinsGF[0]) pinsGF[0] = false; else pinsGF[0] = true;
}

void PartialGameWindow::on_pin2_clicked()
{
  // if (pinsGF[1]) pinsGF[1] = false; else pinsGF[1] = true;
}

void PartialGameWindow::on_pin3_clicked()
{
  // if (pinsGF[2]) pinsGF[2] = false; else pinsGF[2] = true;
}

void PartialGameWindow::on_pin4_clicked()
{
  //  if (pinsGF[3]) pinsGF[3] = false; else pinsGF[3] = true;
}

void PartialGameWindow::on_pin5_clicked()
{
   // if (pinsGF[4]) pinsGF[4] = false; else pinsGF[4] = true;
}

void PartialGameWindow::on_pin6_clicked()
{
  //  if (pinsGF[5]) pinsGF[5] = false; else pinsGF[5] = true;
}

void PartialGameWindow::on_pin7_clicked()
{
   // if (pinsGF[6]) pinsGF[6] = false; else pinsGF[6] = true;
}

void PartialGameWindow::on_pin8_clicked()
{
   // if (pinsGF[7]) pinsGF[7] = false; else pinsGF[7] = true;
}

void PartialGameWindow::on_pin9_clicked()
{
 //  if (pinsGF[8]) pinsGF[8] = false; else pinsGF[8] = true;
}

void PartialGameWindow::on_stavanieButton_clicked()
{
    cmdOutGF = 21;
    commThread->mode = false;
}

void PartialGameWindow::on_stavButton_clicked()
{
 cmdOutGF = -999;
 commThread->mode =  false;
 ChangerWindow ch;
 changerRunning = true;
 ch.exec();
 while (changerRunning) {}

 if (changer) {
     cmdGF = cmdOutGF;
     pointsGF = pointsOutGF;
     roundsGF = roundsOutGF;
     scoreGF = scoreOutGF;
     for (int i = 0; i<9; i++) pinsGF[i] = pinsOutGF[i];
     onRedrawGUI();
 }
 commThread->mode = false;
}

void PartialGameWindow::on_koniecButton_clicked()
{
    cmdOutGF = -100;

    commThread->mode = false;
    while (commThread->mode == false){}
    commThread->Stop = true;
    while (commThread->isRunning()
           ) { }



    currentRound = pointsGF = scoreGF = roundsGF = cmdGF = lastPoints = 0;

    for (int i = 0; i<9; i++) pinsGF[i] = 0;

    faultSwitch = false;

    close();
}





