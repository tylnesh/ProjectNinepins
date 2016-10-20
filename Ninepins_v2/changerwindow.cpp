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

#include "changerwindow.h"
#include "ui_changerwindow.h"

#include "global.h"


int cmd = 0;
int points=0;
int rounds=0;
int score=0;
bool pins[9] = {false, false, false, false, false, false, false, false, false};



ChangerWindow::ChangerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangerWindow)
{
    ui->setupUi(this);
    this ->setWindowFlags(Qt::Window);
    this ->showFullScreen();
    cmd = cmdGF;
    points = pointsGF;
    rounds = roundsGF;
    score = scoreGF;
    for (int i = 0; i<9; i++) pins[i] = pinsGF[i];
    RedrawGUI();

}

ChangerWindow::~ChangerWindow()
{
    delete ui;
}

void ChangerWindow::on_okButton_clicked()
{
    cmdOutGF = -1000;
    pointsOutGF = points;
    roundsOutGF = rounds;
    scoreOutGF = score;
    for (int i = 0; i<9; i++) pinsOutGF[i] = pins[i];
    changer = true; changerRunning=false; close();
}

void ChangerWindow::on_cancelButton_clicked()
{
  cmdOutGF = -1001; changer = false;  changerRunning = false; close();
}

void ChangerWindow::on_scorePlus_clicked()
{
score++;
RedrawGUI();
}

void ChangerWindow::on_scoreMinus_clicked()
{
score--;
RedrawGUI();
}

void ChangerWindow::on_roundsPlus_clicked()
{
rounds++;
RedrawGUI();
}

void ChangerWindow::on_roundsMinus_clicked()
{
rounds--;
RedrawGUI();
}



void ChangerWindow::on_pin1_clicked()
{
 if (pins[0])
 {
     pins[0] = false;
     points--; score--;
 }
     else
 {
     pins[0] = true;
     points++; score++;
 }

 RedrawGUI();
}

void ChangerWindow::on_pin2_clicked()
{  
 if (pins[1])
 {
     pins[1] = false;
     points--; score--;
 }
     else
 {
     pins[1] = true;
     points++; score++;
 }

 RedrawGUI();
}

void ChangerWindow::on_pin3_clicked()
{
 if (pins[2])
 {
     pins[2] = false;
     points--; score--;
 }
     else
 {
     pins[2] = true;
     points++; score++;
 }

 RedrawGUI();
}

void ChangerWindow::on_pin4_clicked()
{
 if (pins[3])
 {
     pins[3] = false;
     points--; score--;
 }
     else
 {
     pins[3] = true;
     points++; score++;
 }

 RedrawGUI();
}

void ChangerWindow::on_pin5_clicked()
{
 if (pins[4])
 {
     pins[4] = false;
     points--; score--;
 }
     else
 {
     pins[4] = true;
     points++; score++;
 }

 RedrawGUI();
}

void ChangerWindow::on_pin6_clicked()
{
 if (pins[5])
 {
     pins[5] = false;
     points--; score--;
 }
     else
 {
     pins[5] = true;
     points++; score++;
 }

 RedrawGUI();
}

void ChangerWindow::on_pin7_clicked()
{
 if (pins[6])
 {
     pins[6] = false;
     points--; score--;
 }
     else
 {
     pins[6] = true;
     points++; score++;
 }

 RedrawGUI();
}

void ChangerWindow::on_pin8_clicked()
{
 if (pins[7])
 {
     pins[7] = false;
     points--; score--;
 }
     else
 {
     pins[7] = true;
     points++; score++;
 }

 RedrawGUI();
}

void ChangerWindow::on_pin9_clicked()
{
 if (pins[8])
 {
     pins[8] = false;
     points--; score--;
 }
     else
 {
     pins[8] = true;
     points++; score++;
 }

 RedrawGUI();
}

void ChangerWindow::RedrawGUI()
{
    ui->scoreLCD->display(score);
    ui->roundsLCD->display(rounds);
    ui->pointsLCD->display(points);

    if (!pins[0]) ui->pin1->setIcon(QIcon(":/images/kolok1.png")); else ui->pin1->setIcon(QIcon(":/images/kolok2.png"));
    if (!pins[1]) ui->pin2->setIcon(QIcon(":/images/kolok1.png")); else ui->pin2->setIcon(QIcon(":/images/kolok2.png"));
    if (!pins[2]) ui->pin3->setIcon(QIcon(":/images/kolok1.png")); else ui->pin3->setIcon(QIcon(":/images/kolok2.png"));
    if (!pins[3]) ui->pin4->setIcon(QIcon(":/images/kolok1.png")); else ui->pin4->setIcon(QIcon(":/images/kolok2.png"));
    if (!pins[4]) ui->pin5->setIcon(QIcon(":/images/kolok1.png")); else ui->pin5->setIcon(QIcon(":/images/kolok2.png"));
    if (!pins[5]) ui->pin6->setIcon(QIcon(":/images/kolok1.png")); else ui->pin6->setIcon(QIcon(":/images/kolok2.png"));
    if (!pins[6]) ui->pin7->setIcon(QIcon(":/images/kolok1.png")); else ui->pin7->setIcon(QIcon(":/images/kolok2.png"));
    if (!pins[7]) ui->pin8->setIcon(QIcon(":/images/kolok1.png")); else ui->pin8->setIcon(QIcon(":/images/kolok2.png"));
    if (!pins[8]) ui->pin9->setIcon(QIcon(":/images/kolok1.png")); else ui->pin9->setIcon(QIcon(":/images/kolok2.png"));


}
