#pragma once

#include <SDL2/SDL.h>

#include "timer.h"

class Object;
class Player;

class Game {
public:
    Game();
    ~Game();

    void init();
    void update(const float delta_time);
    void input(float delta_time);
    void draw(const float delta_time);
    void draw_text(const char* msg, int x, int y, uint8_t r, uint8_t g, uint8_t b, int size);
    void run();

private:
    void print_loop_stats(float delta_time);

    SDL_Window* window;
    SDL_Renderer* renderer;

    bool gameloop;
    bool fullscreen;

    int frameCount;
    int currentFPS;
    int lastFrame;

    static const int WIDTH = 1280;
    static const int HEIGHT = 720;
    static const int FPS = 60;

    Player* player;

    void updateFPS();

    SDL_Texture* m_text_texture{};
    SDL_Surface* m_text_surface{};
    Timer<float> m_timer{};
};
