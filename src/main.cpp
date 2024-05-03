#include "city.h"
#include "robber.h"
#include "jewel.h"

int main()
{
    srand(time(0));

    City city;
    Jewel jewels[60];

    int jewelIndex = 0;

    city.printGrid();

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (city.getGridCell(i, j) == 'J')
            {
                int x = j + 1;
                int y = ROWS - i;
                jewels[jewelIndex] = Jewel(x, y);
                jewelIndex++;
            }
        }
    }

    cout << "Value of Jewel 1: " << jewels[7].getValue();
    cout << "Coordinates of Jewel are: " << jewels[7].getxPos() << " " << jewels[7].getyPos();

    int num_robbers = 0;
    int robberID = 1;
    int turns = 0;
    Robber<Jewel> robbers[20];
    bool hasCopy = false;

    while (turns < 5)
    {
        for (int i = 0; i < 20; i++)
        {
            if (robbers[i].getTotalWorth() > 1600 && !robbers[i].getCopy())
            {
                int p_rob = rand() % 11;
                int r_rob = rand() % 13;

                robbers[i + 1] = Robber<Jewel>(++robberID, p_rob, r_rob);
                robbers[i + 1].move(city);

                city.printGrid();

                cout << "Total Worth of robber " << i << ": " << robbers[i + 1].getTotalWorth() << endl;

                robbers[i].setCopy(true);
            }
            else
            {

                robbers[i].move(city);

                city.printGrid();

                cout << "Total Worth of robber " << i << ": " << robbers[i].getTotalWorth() << endl;
            }
        }
        turns++;
    }
}