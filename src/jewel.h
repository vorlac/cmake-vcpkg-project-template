#ifndef JEWEL_HEADER
#define JEWEL_HEADER

#include <cstdlib>
#include <iostream>
using namespace std;

class Jewel
{
private:
    int m_value;
    int m_xPos;
    int m_yPos;

public:
    Jewel();
    Jewel(const int xPos, const int yPos);
    Jewel operator*(const int multiplier) const;
    void operator*=(const int multiplier);
    int getValue() const;
    void setValue(int cost);
    int getxPos() const;
    void setxPos(int x_coord);
    int getyPos() const;
    void setyPos(int y_coord);
};

#endif
