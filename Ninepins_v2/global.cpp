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
#include "global.h"
#include <iostream>


bool faultSwitch = false;
bool faultConfirmed = false;
bool faultRunning = false;
bool changerRunning=false;

int currentRound = 0;
int lastPoints = 0;

int cmdGF = 0;
int pointsGF=0;
int roundsGF=0;
int scoreGF=0;
bool pinsGF[9] = {false, false, false, false, false, false, false, false, false};

int cmdOutGF = 0;
int pointsOutGF=0;
int roundsOutGF=0;
int scoreOutGF=0;
bool pinsOutGF[9] = {false, false, false, false, false, false, false, false, false};

int pointsTmp=0;
int scoreTmp=0;
bool pinsTmp[9] = {false, false, false, false, false, false, false, false, false};

bool changer = false;

bool isPartial = 0;

