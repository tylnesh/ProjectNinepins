#ifndef GLOBAL_H
#define GLOBAL_H

extern bool faultSwitch;
extern bool faultConfirmed;
extern bool faultRunning;
extern bool changer;
extern bool changerRunning;

extern int currentRound;

extern int lastPoints;

extern int cmdGF;
extern int cmdOutGF;

extern int pointsGF;
extern int pointsOutGF;
extern int pointsTmp;

extern int roundsGF;
extern int roundsOutGF;


extern int scoreGF;
extern int scoreOutGF;
extern int scoreTmp;


extern bool pinsGF[9];
extern bool pinsOutGF[9];
extern bool pinsTmp[9];

extern bool isPartial;

#endif // GLOBAL_H
