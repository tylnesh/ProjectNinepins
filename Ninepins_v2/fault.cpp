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
#include "fault.h"
#include "ui_fault.h"
#include "global.h"
#include "comm.h"



fault::fault(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fault)
{
    ui->setupUi(this);
    this ->setWindowFlags(Qt::Window);
    this ->showFullScreen();
    faultRunning = true;

}

fault::~fault()
{
    delete ui;
}




void fault::on_faultConfirmedButton_clicked()
{
    cmdOutGF = -2;
    faultSwitch = false;
    faultConfirmed = true;
    faultRunning = false;

   /* pointsGF = pointsTmp;
    scoreGF = scoreTmp;
    for (int i = 0; i<9; i++) pinsGF[i] = pinsTmp[i];*/

    close();
}

void fault::on_faultCancelledButton_clicked()
{
    cmdOutGF = -3;
    faultSwitch = false;
    faultConfirmed = false;
    faultRunning = false;
    close();
}
