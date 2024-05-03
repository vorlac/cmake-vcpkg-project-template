#include <ranges>
#include <string>

#include "city.h"

City::City()
    : m_jewelCount(60)
{
    initializeGrid();
    scatterJewels();
}

void City::initializeGrid()
{
    for (int y = 0; y < ROWS; y++)
    {
        for (int x = 0; x < COLS; x++)
            this->setGridCell(x, y, ". ");
    }
}

void City::printGrid()
{
    static const std::string separator(COLS * 3, '-');
    cout << separator << endl;
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
            cout << m_grid[row][col] << " ";
        cout << endl;
    }
    cout << separator << endl;
}

void City::scatterJewels()
{
    int jewelsPlaced = 0;

    while (jewelsPlaced < m_jewelCount)
    {
        int x = rand() % COLS;
        int y = rand() % ROWS;

        if (this->getGridCell(x, y) == ". ")
        {
            Jewel jewel(x, y);
            this->setGridCell(x, y, "J ");
            jewelsPlaced++;
        }
    }
}

int City::getJewelCount()
{
    return m_jewelCount;
}

std::string City::getGridCell(int x, int y) const
{
    return m_grid[y][x];
}

void City::setGridCell(int x, int y, std::string value)
{
    m_grid[y][x] = value;
}
