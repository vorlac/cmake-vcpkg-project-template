#ifndef HEIST_HEADER
#define HEIST_HEADER

#include <cstdlib>
#include <iostream>
#include "jewel.h"
using namespace std;

static const int ROWS = 11;
static const int COLS = 13;

class City
{
private:
    char m_grid[ROWS][COLS];
    int m_jewelCount;

public:
    City();
    void initializeGrid();
    void printGrid();
    void scatterJewels();
    char getGridCell(int row, int col) const;
    void setGridCell(int row, int col, char value);
    int getJewelCount();
};

#endif