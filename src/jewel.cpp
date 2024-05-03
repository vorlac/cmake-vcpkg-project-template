#include "jewel.h"

Jewel::Jewel() : m_value(0), m_xPos(0), m_yPos(){};

Jewel::Jewel(const int xPos, const int yPos) : m_xPos(xPos), m_yPos(yPos)
{
    m_value = (xPos + yPos) * (xPos + yPos);
}

Jewel Jewel::operator*(const int multiplier)
{
    Jewel result(m_xPos, m_yPos);
    result.m_value = m_value * multiplier;
    return result;
}

void Jewel::operator*=(const int multiplier)
{
    m_value *= multiplier;
}

int Jewel::getValue() const
{
    return m_value;
}

void Jewel::setValue(int cost)
{
    m_value = cost;
}

int Jewel::getxPos() const
{
    return m_xPos;
}

void Jewel::setxPos(int x_coord)
{
    m_xPos = x_coord;
}

int Jewel::getyPos() const
{
    return m_yPos;
}

void Jewel::setyPos(int y_coord)
{
    m_yPos = y_coord;
}