#include "city.h"

City::City() : m_jewelCount(60)
{
    initializeGrid();
    scatterJewels();
}

void City::initializeGrid()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            m_grid[i][j] = '.';
        }
    }
}

void City::printGrid()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            cout << m_grid[i][j] << " ";
        }
        cout << endl;
    }
}

void City::scatterJewels()
{
    int jewelsPlaced = 0;

    while(jewelsPlaced < m_jewelCount){
        int row = rand() % ROWS;
        int col = rand() % COLS;

        if (m_grid[row][col] == '.')
        {
            Jewel jewel(row, col);
            m_grid[row][col] = 'J';

            jewelsPlaced++;
        }
    }

}

int City::getJewelCount()
{
    return m_jewelCount;
}

char City::getGridCell(int row, int col) const
{
    return m_grid[row][col];
}

void City::setGridCell(int row, int col, char value){
    m_grid[row][col] = value;
}