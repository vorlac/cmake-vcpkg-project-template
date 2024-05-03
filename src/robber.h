#ifndef ROBBER_HEADER
#define ROBBER_HEADER

#include <cstdlib>
#include <iostream>
#include <string>

#include "city.h"
#include "jewel.h"

using namespace std;

const int MAX_CAPACITY = 20;

enum Direction {
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest
};

template <class Loot>
class Robber
{
public:
    Robber()
        : m_id(Robber<Loot>::curr_robber_id++)
        , m_xPos(rand() % COLS)
        , m_yPos(rand() % ROWS)
        , m_hasCopy(false)
        , m_bagSize(0)
    {
    }

    Robber(const Robber& other)
        : m_id(other.m_id)
        , m_xPos(other.m_xPos)
        , m_yPos(other.m_yPos)
        , m_hasCopy(false)
        , m_bagSize(0)
    {
    }

    Robber(int id, int xPos, int yPos)
        : m_id(id)
        , m_xPos(xPos)
        , m_yPos(yPos)
        , m_hasCopy(false)
        , m_bagSize(0)
    {
    }

    void pickUpLoot(const Loot& loot, const City& city, int xPos, int yPos)
    {
        if (m_bagSize >= 20)
        {
            cout << "The robber's bag is full! Cannot pick up loot."
                 << endl;
            return;
        }

        Loot newLoot;
        if (city.getGridCell(xPos, yPos) == "J ")
        {
            int x = ROWS - xPos;
            int y = yPos + 1;
            newLoot = Loot(x, y);
            m_bag[m_bagSize++] = newLoot;

            m_totalWorth += newLoot.getValue();

            cout << "Robber " << m_id
                 << " picked up a loot worth $" << newLoot.getValue()
                 << " at position (" << y << ", " << x << "). "
                 << endl;
        }
    }

    Loot operator--()
    {
        if (m_bagSize > 0)
        {
            --m_bagSize;

            Loot temp = m_bag[0];
            m_totalWorth -= temp.getValue();

            for (int i = 0; i < m_bagSize && i < MAX_CAPACITY - 1; i++)
                m_bag[i] = m_bag[i + 1];

            return temp;
        }

        return Loot{};
    }

    void move(City& city)
    {
        m_isActive = true;

        int prev_x{ m_xPos };
        int prev_y{ m_yPos };

        bool robberMoved = false;
        Direction dir{ static_cast<Direction>(rand() % 8) };

        while (!robberMoved && m_isActive)
        {
            switch (dir)
            {
                case Direction::NorthEast:
                    if (m_xPos < COLS - 1 && m_yPos > 0)
                    {
                        cout << "Robber moved NE." << endl;
                        city.setGridCell(m_xPos, m_yPos, "x ");
                        ++m_xPos;
                        --m_yPos;
                        robberMoved = true;
                    }
                    break;
                case Direction::East:
                    if (m_xPos < COLS - 1)
                    {
                        cout << "Robber moved E." << endl;
                        city.setGridCell(m_xPos, m_yPos, "x ");
                        ++m_xPos;
                        robberMoved = true;
                    }
                    break;
                case Direction::SouthEast:
                    if (m_yPos < ROWS - 1 && m_xPos < COLS - 1)
                    {
                        cout << "Robber moved SE." << endl;
                        city.setGridCell(m_xPos, m_yPos, "x ");
                        ++m_xPos;
                        ++m_yPos;
                        robberMoved = true;
                    }
                    break;
                case Direction::North:
                    if (m_yPos > 0)
                    {
                        cout << "Robber moved N." << endl;
                        city.setGridCell(m_xPos, m_yPos, "x ");
                        --m_yPos;
                        robberMoved = true;
                    }
                    break;
                case Direction::South:
                    if (m_yPos < ROWS - 1)
                    {
                        cout << "Robber moved S." << endl;
                        city.setGridCell(m_xPos, m_yPos, "x ");
                        ++m_yPos;
                        robberMoved = true;
                    }
                    break;
                case Direction::NorthWest:
                    if (m_yPos > 0 && m_xPos > 0)
                    {
                        cout << "Robber moved NW." << endl;
                        city.setGridCell(m_xPos, m_yPos, "x ");
                        --m_xPos;
                        --m_yPos;
                        robberMoved = true;
                    }
                    break;
                case Direction::West:
                    if (m_xPos > 0)
                    {
                        cout << "Robber moved W." << endl;
                        city.setGridCell(m_xPos, m_yPos, "x ");
                        --m_xPos;
                        robberMoved = true;
                    }
                    break;
                case Direction::SouthWest:
                    if (m_yPos < ROWS - 1 && m_xPos > 0)
                    {
                        cout << "Robber moved SW." << endl;
                        city.setGridCell(m_xPos, m_yPos, "x ");
                        ++m_yPos;
                        --m_xPos;
                        robberMoved = true;
                    }
                    break;
                default:
                    break;
            }
            m_isActive = false;
        }

        if (robberMoved)
        {
            if (city.getGridCell(m_xPos, m_yPos) == "J ")
                this->pickUpLoot(Loot(m_xPos, m_yPos), city, m_xPos, m_yPos);
            city.setGridCell(prev_x, prev_y, "x ");
            auto id_str{ to_string(m_id) };
            if (id_str.size() == 1)
                id_str = "0" + id_str;
            city.setGridCell(m_xPos, m_yPos, id_str);
        }

        const bool robber_tripped{ rand() % 100 < 10 };
        if (robberMoved && robber_tripped && m_bagSize > 0)
        {
            Loot droppedLoot = --(*this);
            cout << "The robber tripped! (lost $"
                 << droppedLoot.getValue() << ")"
                 << endl;

            city.setGridCell(prev_x, prev_y, "J ");
            if (city.getGridCell(m_xPos, m_yPos) == ". ")
            {
                auto id_str{ to_string(m_id) };
                if (id_str.size() == 1)
                    id_str = "0" + id_str;
                city.setGridCell(m_xPos, m_yPos, id_str);
            }
        }
    }

    int getTotalWorth() const
    {
        return m_totalWorth;
    }

    int getxPos() const
    {
        return m_xPos;
    }

    int getyPos() const
    {
        return m_yPos;
    }

    void setCopy(bool copy)
    {
        m_hasCopy = copy;
    }

    bool hasCopy() const
    {
        return m_hasCopy;
    }

private:
    int m_id{};
    int m_xPos{};
    int m_yPos{};
    int m_bagSize{};
    bool m_isActive{};
    bool m_isGreedy{};
    bool m_hasCopy{};
    Loot m_bag[MAX_CAPACITY]{};
    int m_totalWorth{ 0 };
    static inline int curr_robber_id{ 0 };
};

#endif
