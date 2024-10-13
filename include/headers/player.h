#pragma once

#include "object.h"

class Player : public Object {
public:
    Player(int x, int y, int w, int h, std::string sprite, SDL_Renderer* renderer, int health = 100);

    void Create();
    void Input(const SDL_Event& event);
    void Update(float delta_time);
    void Draw();

private:
    int m_health;
};
