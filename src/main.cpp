#include <array>
#include <cstdint>

#include "city.h"
#include "jewel.h"
#include "robber.h"

int main()
{
    srand(static_cast<uint32_t>(time(NULL)));

    City city{};
    Jewel jewels[60]{};

    int jewelIndex{ 0 };
    for (int y = 0; y < ROWS; y++)
    {
        for (int x = 0; x < COLS; x++)
        {
            if (city.getGridCell(x, y) == "J ")
            {
                jewels[jewelIndex] = Jewel(x + 1, ROWS - y);
                jewelIndex++;
            }
        }
    }

    cout << "Value of Jewel 1: $" << jewels[7].getValue() << endl;
    cout << "Coordinates of Jewel are: " << jewels[7].getxPos()
         << " " << jewels[7].getyPos() << endl;

    int robberID = 0;
    int turns = 0;
    std::array<Robber<Jewel>, 20> robbers{};
    const auto& curr_robber{ robbers[robberID] };
    city.setGridCell(curr_robber.getxPos(), curr_robber.getyPos(), "00");
    city.printGrid();

    while (turns++ < 5)
    {
        for (auto turn_count = 0; turn_count < 20; ++turn_count)
        {
            if (robbers[robberID].getTotalWorth() > 1600 && !robbers[robberID].hasCopy())
            {
                int x_rob = rand() % COLS;
                int y_rob = rand() % ROWS;

                ++robberID %= robbers.size();
                robbers[robberID] = Robber<Jewel>(robberID, x_rob, y_rob);
                robbers[robberID].move(city);
                city.printGrid();

                cout << "Total Worth of robber " << robberID
                     << ": $" << robbers[robberID + 1].getTotalWorth()
                     << endl;

                robbers[robberID].setCopy(true);
            }
            else
            {
                robbers[robberID].move(city);
                city.printGrid();

                cout << "Total Worth of robber " << robberID
                     << ": $" << robbers[robberID].getTotalWorth()
                     << endl;
            }
        }
    }
}
