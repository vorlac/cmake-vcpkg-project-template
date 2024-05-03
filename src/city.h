#ifndef HEIST_HEADER
#define HEIST_HEADER

#include <cstdlib>

#include "jewel.h"
using namespace std;

constexpr static int ROWS = 11;
constexpr static int COLS = 13;

class City
{
private:
    std::string m_grid[ROWS][COLS]{};
    int m_jewelCount{ 0 };

public:
    City();
    void initializeGrid();
    void printGrid();
    void scatterJewels();
    std::string getGridCell(int x, int y) const;
    void setGridCell(int x, int y, std::string value);
    int getJewelCount();
};

#endif
