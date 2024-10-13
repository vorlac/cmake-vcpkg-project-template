#pragma once

#include <iostream>

#include <SDL2/SDL.h>

#include "vector2.h"

class Object {
protected:
    SDL_Rect m_dest{};
    SDL_Rect m_src{};
    SDL_Texture* m_tex{ nullptr };
    Vector2 m_position{};
    Vector2 m_size{};
    Vector2 m_direction{};
    Vector2 m_velocity{};
    float m_max_speed{ 500.0f };
    std::string m_spriteFile{};

public:
    Object(std::string sprite, SDL_Renderer* renderer);
    virtual ~Object() = default;

    SDL_Rect getDest() const {
        return m_dest;
    }

    SDL_Rect getSource() const {
        return m_src;
    }

    SDL_Texture* getTexture() const {
        return m_tex;
    }

    void setDest(int x, int y, int w, int h);
    void setSource(int x, int y, int w, int h);
    void setTexture(std::string filename, SDL_Renderer* renderer);
    void draw_sprite(Object& obj, SDL_Renderer* renderer);

    const Vector2& getPosition() const {
        return m_position;
    }

    const Vector2& getVelocity() const {
        return m_velocity;
    }

    const Vector2& getDirection() const {
        return m_direction;
    }

    float getMaxSpeed() const {
        return m_max_speed;
    }

    void setPosition(const Vector2 pos) {
        m_position = pos;
    }

    void setVelocity(const Vector2 velocity) {
        m_velocity = velocity;
    }

    void setDirection(const Vector2 dir) {
        m_direction = dir;
    }

    void setMaxSpeed(const float speed) {
        m_max_speed = speed;
    }

protected:
    SDL_Renderer* m_renderer{ nullptr };
};
