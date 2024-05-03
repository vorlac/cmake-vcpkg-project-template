#ifndef POLICE_HEADER
#define POLICE_HEADER

#include <cstdlib>
#include <iostream>
#include "robber.h"
#include "city.h"
using namespace std;

class Police
{
private:
    int id;
    int xPos;
    int yPos;
    int totalLootWorth;
    int robbersCaught;

public:
    Police();
    Police(int id, int xPos, int yPos);
    void arrest(const Robber<Jewel> &robber);
    void move(City& city);
    int getLootWorth();
};

#endif