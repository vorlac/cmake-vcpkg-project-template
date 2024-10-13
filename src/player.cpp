#include "object.h"
#include "player.h"

Player::Player(int x, int y, int w, int h, std::string sprite, SDL_Renderer* renderer, int health)
    : Object(sprite, renderer)
    , m_health(100) {
    this->setDest(x, y, 100, 100);
    this->setSource(0, 0, 64, 64);
    this->setTexture(sprite, renderer);
    this->Create();
}

void Player::Create() {
    SDL_Log("Create event");
    this->setPosition({ 0.0f, 0.0f });
}

void Player::Input(const SDL_Event& event) {
    const Uint8* key = SDL_GetKeyboardState(nullptr);

    const float input_dir_x{
        key[SDL_SCANCODE_LEFT]    ? -1.0f
        : key[SDL_SCANCODE_RIGHT] ? 1.0f
                                  : 0.0f
    };

    const float input_dir_y{
        key[SDL_SCANCODE_UP]     ? -1.0f
        : key[SDL_SCANCODE_DOWN] ? 1.0f
                                 : 0.0f
    };

    Vector2 input_dir{
        input_dir_x,
        input_dir_y,
    };

    if (input_dir_x != 0.0f && input_dir_y != 0.0f)
        input_dir *= 0.7f;

    this->setDirection(input_dir);

    switch (event.type) {
        case SDL_KEYDOWN:
            // std::cout << ("Key Down");
            break;

        case SDL_KEYUP:
            // std::cout << ("Key Up");
            break;
    }
}

void Player::Update(const float delta_time) {
    constexpr static float friction{ 10.0f };
    const Vector2 movement_velocity{ this->getDirection() * delta_time };

    Vector2 velocity{ this->getVelocity().lerp(
        movement_velocity, friction * delta_time) };
    velocity = velocity.clamp({ -1.0, -1.0 }, { 1.0, 1.0 });

    this->setPosition(this->getPosition() + (velocity * this->getMaxSpeed()));
    this->setVelocity(velocity);

    const Vector2 new_pos{ this->getPosition() };
    std::cout << "Position: ("
              << new_pos.x << ", "
              << new_pos.y << ")\n";

    this->setDest(
        static_cast<int>(new_pos.x),
        static_cast<int>(new_pos.y),
        100, 100);

    this->setSource(0, 0, 64, 64);
}

void Player::Draw() {
    draw_sprite(*this, m_renderer);
}
