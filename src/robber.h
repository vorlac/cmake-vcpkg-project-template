#ifndef ROBBER_HEADER
#define ROBBER_HEADER

#include <cstdlib>
#include <iostream>
#include "city.h"
#include "jewel.h"
using namespace std;

const int MAX_CAPACITY = 20;

template <class Loot>
class Robber
{
private:
    int id;
    int xPos;
    int yPos;
    Loot bag[MAX_CAPACITY];
    int bagSize;
    static int totalWorth;
    bool isActive;
    bool isGreedy;
    bool hasCopy;

public:
    Robber();
    Robber(int id, int xPos, int yPos);
    Robber(const Robber &other);
    void pickUpLoot(const Loot &loot, const City &city, int xPos, int yPos);
    Loot operator--();
    void move(City &city);
    static int getTotalWorth();
    int getxPos();
    int getyPos();
    void setCopy(bool copy);
    bool getCopy();
};

template <class Loot>
Robber<Loot>::Robber() : id(0), xPos(0), yPos(0), hasCopy(0), bagSize(0){};

template <class Loot>
Robber<Loot>::Robber(int id, int xPos, int yPos) : id(id), xPos(xPos), yPos(yPos), hasCopy(0), bagSize(0){};

template <class Loot>
Robber<Loot>::Robber(const Robber &other) : id(other.id), xPos(other.xPos), yPos(other.yPos), hasCopy(0), bagSize(0){};

template <class Loot>
int Robber<Loot>::totalWorth = 0;

template <class Loot>
void Robber<Loot>::pickUpLoot(const City &city, int xPos, int yPos)
{

    if (bagSize >= 20)
    {
        cout << "The robber's bag is full! Cannot pick up loot.\n";
        return;
    }

    Loot newLoot;
    if (city.getGridCell(xPos, yPos) == 'J')
    {
        int x = ROWS - xPos;
        int y = yPos + 1;
        newLoot = Loot(x, y);
        bag[bagSize++] = newLoot;

        totalWorth += newLoot.getValue();

        cout << "Robber " << id << "picked up a loot worth $" << newLoot.getValue() << " at position (" << y << ", " << x << "). " << endl;
    }
}

template <class Loot>
int Robber<Loot>::getTotalWorth()
{
    return totalWorth;
}

template <class Loot>
Loot Robber<Loot>::operator--()
{
    if (bagSize > 0)
    {
        bagSize -= 1;

        Loot temp = bag[0];

        for (int i = 0; i < bagSize; i++)
        {
            bag[i] = bag[i + 1];
        }
    }

    return bag[0];
}

template <class Loot>
void Robber<Loot>::move(City &city)
{

    Robber<Loot> robber;
    bool robberMoved = false;
    bool isActive = true;

    int xPos = rand() % 11;
    int yPos = rand() % 13;

    int direction = rand() % 8;
    while (!robberMoved && isActive)
    {
        switch (direction)
        {
        case 0:
            if (yPos < ROWS - 1 && xPos > 0)
            {
                cout << "Robber moved NE." << endl;
                city.setGridCell(xPos, yPos, '.');
                ++yPos;
                --xPos;
                robberMoved = true;
            }
            break;
        case 1:
            if (yPos < ROWS - 1)
            {
                cout << "Robber moved E." << endl;
                city.setGridCell(xPos, yPos, '.');
                ++yPos;
                robberMoved = true;
            }
            break;
        case 2:
            if (yPos < ROWS - 1 && xPos < COLS - 1)
            {
                cout << "Robber moved SE." << endl;
                city.setGridCell(xPos, yPos, '.');
                ++yPos;
                ++xPos;
                robberMoved = true;
            }
            break;
        case 3:
            if (xPos > 0)
            {
                cout << "Robber moved N." << endl;
                city.setGridCell(xPos, yPos, '.');
                --xPos;
                robberMoved = true;
            }
            break;
        case 4:
            if (xPos < COLS - 1)
            {
                cout << "Robber moved S." << endl;
                city.setGridCell(xPos, yPos, '.');
                ++xPos;
                robberMoved = true;
            }
            break;
        case 5:
            if (yPos > 0 && xPos > 0)
            {
                cout << "Robber moved NW." << endl;
                city.setGridCell(xPos, yPos, '.');
                --yPos;
                --xPos;
                robberMoved = true;
            }
            break;
        case 6:
            if (yPos > 0)
            {
                cout << "Robber moved W." << endl;
                city.setGridCell(xPos, yPos, '.');
                --yPos;
                robberMoved = true;
            }
            break;
        case 7:
            if (yPos > 0 && xPos < COLS - 1)
            {
                cout << "Robber moved SW." << endl;
                city.setGridCell(xPos, yPos, '.');
                --yPos;
                ++xPos;
                robberMoved = true;
            }
            break;
        default:
            break;
        }
    }

    if (robberMoved)
    {
        if (city.getGridCell(xPos, yPos) == 'J')
        {
            robber.pickUpLoot(Loot(xPos, yPos), city, xPos, yPos);
        }
        city.setGridCell(xPos, yPos, 'R');
    }

    if (robberMoved && rand() % 100 < 60 && bagSize > 0)
    {

        cout << "The robber tripped!!!" << endl;
        Loot droppedLoot = --robber;

        if (city.getGridCell(xPos, yPos) == '.')
        {
            city.setGridCell(xPos, yPos, 'J');
            return;
        }
        else
        {
            for (int i = xPos - 1; i <= xPos + 1; i++)
            {
                for (int j = yPos - 1; j < yPos + 1; j++)
                {
                    if (city.getGridCell(i, j) == '.')
                    {
                        city.setGridCell(i, j, 'J');
                        return;
                    }
                }
            }
        }
    }
}

template <class Loot>
int Robber<Loot>::getxPos()
{
    return xPos;
}

template <class Loot>
int Robber<Loot>::getyPos()
{
    return yPos;
}

template <class Loot>
void Robber<Loot>::setCopy(bool copy)
{
    hasCopy = copy;
}

template <class Loot>
bool Robber<Loot>::getCopy()
{
    return hasCopy;
}

#endif