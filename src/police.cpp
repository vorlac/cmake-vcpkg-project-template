#include "police.h"

Police::Police()
    : id(0)
    , xPos(0)
    , yPos(0){};

Police::Police(int id, int xPos, int yPos)
    : id(id)
    , xPos(xPos)
    , yPos(yPos){};

void Police::arrest(const Robber<Jewel>& robber)
{
    totalLootWorth += robber.getTotalWorth();
}

void Police::move(City& city)
{
    int direction = rand() % 8;
    switch (direction)
    {
        case 0:
            if (yPos < ROWS - 1 && xPos > 0)
            {
                city.setGridCell(xPos, yPos, ". ");
                ++yPos;
                --xPos;
            }
            break;
        case 1:
            if (yPos < ROWS - 1)
            {
                city.setGridCell(xPos, yPos, ". ");
                ++yPos;
            }
            break;
        case 2:
            if (yPos < ROWS - 1 && xPos < COLS - 1)
            {
                city.setGridCell(xPos, yPos, ". ");
                ++yPos;
                ++xPos;
            }
            break;
        case 3:
            if (xPos > 0)
            {
                city.setGridCell(xPos, yPos, ". ");
                --xPos;
            }
            break;
        case 4:
            if (xPos < COLS - 1)
            {
                city.setGridCell(xPos, yPos, ". ");
                ++xPos;
            }
            break;
        case 5:
            if (yPos > 0 && xPos > 0)
            {
                city.setGridCell(xPos, yPos, ". ");
                --yPos;
                --xPos;
            }
            break;
        case 6:
            if (yPos > 0)
            {
                city.setGridCell(xPos, yPos, ". ");
                --yPos;
            }
            break;
        case 7:
            if (yPos > 0 && xPos < COLS - 1)
            {
                city.setGridCell(xPos, yPos, ". ");
                --yPos;
                ++xPos;
            }
            break;
        default:
            break;
    }
}
