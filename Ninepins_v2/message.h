#ifndef MESSAGE_H
#define MESSAGE_H


class message
{
public:
    message();

    //getters
    int getCmd();
    int getWire();
    int getPoints();
    int getRounds();
    int getScore();
    void getPins(bool b[]);

    //setters
    void setCmd(int c);
    void setWire(int w);
    void setPoints(int p);
    void setRounds(int r);
    void setScore(int s);
    void setPins(bool b[]);

    //delete msg
    void delMsg();

    void setMsg();
};

#endif // MESSAGE_H
