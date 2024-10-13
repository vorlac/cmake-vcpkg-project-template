#include <iostream>

#include <SDL2/SDL_ttf.h>

#include "game.h"
#include "player.h"

struct app {
    Player* player;
};

enum Level {
    LOW,
    MEDIUM,
    HIGH
};

// game constructor
Game::Game()
    : window(nullptr)
    , renderer(nullptr)
    , gameloop(true)
    , fullscreen(false)
    , frameCount(0)
    , currentFPS(0)
    , lastFrame(0)
    , player(nullptr) {
}

// game loop cleanup
Game::~Game() {
    delete player;

    SDL_FreeSurface(m_text_surface);
    SDL_DestroyTexture(m_text_texture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
}

// init event
void Game::init() {
    Level myLevelVariable = LOW;
    Level myLevelVariable1 = MEDIUM;
    Level myLevelVariable2 = HIGH;

    std::cout << myLevelVariable << std::endl;
    std::cout << myLevelVariable1 << std::endl;
    std::cout << myLevelVariable2 << std::endl;

    // Validate SDL initialization was successful
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        std::cout << "Error at SDL_Init()" << std::endl;

    // Validate Window & Renderer were created
    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0)
        std::cout << "Error at SDL_CreateWindowAndRenderer()" << std::endl;

    // Init SDL Window Properties
    SDL_SetWindowTitle(window, "2d engine");
    SDL_ShowCursor(1);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    TTF_Init();

    player = new Player(20, 20, 16, 16, "player.png", renderer, 100);

    TTF_Font* font = TTF_OpenFont("assets/fonts/pico-8.ttf", 24);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        // Handle error (e.g., exit or set a default font)
    }

    SDL_Color color{ .r = 0, .g = 255, .b = 0, .a = 255 };
    m_text_surface = TTF_RenderText_Solid(font, "Hello World", color);
    m_text_texture = SDL_CreateTextureFromSurface(renderer, m_text_surface);
}

// update event
void Game::update(const float delta_time) {
    // updateFPS();
    player->Update(delta_time);

    // fullscreen toggler functionality
    if (fullscreen)
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    else
        SDL_SetWindowFullscreen(window, 0);
}

// input event
void Game::input(const float delta_time) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        player->Input(e);
        if (e.type == SDL_QUIT)
            gameloop = false;

        // key states to control window
        const Uint8* keystates = SDL_GetKeyboardState(nullptr);
        if (keystates[SDL_SCANCODE_ESCAPE])
            gameloop = false;
        if (keystates[SDL_SCANCODE_F11])
            fullscreen = !fullscreen;
    }
}

// draw event
void Game::draw(const float delta_time) {
    // fill background with a blue color
    SDL_SetRenderDrawColor(renderer, 40, 43, 200, 255);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = WIDTH;
    rect.h = HEIGHT;
    SDL_RenderFillRect(renderer, &rect);

    // draw_sprite(*player, renderer);
    draw_text("Hello World :)", 20, 30, 0, 255, 0, 24);

    player->Draw();

    SDL_RenderPresent(renderer);
}

// draw text
void Game::draw_text(const char* msg, int x, int y, uint8_t r, uint8_t g, uint8_t b, int size) {
    SDL_Rect rect{
        .x = x,
        .y = y,
        .w = m_text_surface->w,
        .h = m_text_surface->h,
    };

    SDL_RenderCopy(renderer, m_text_texture, nullptr, &rect);
}

// update fps
void Game::updateFPS() {
    frameCount++;
    int timerFPS = SDL_GetTicks() - lastFrame;
    if (timerFPS < (1000 / FPS))
        SDL_Delay((1000 / FPS) - timerFPS);
}

void Game::print_loop_stats(const float delta_time) {
    const float elapsed_time{ m_timer.elapsed() };
    const uint64_t iterations{ m_timer.tick_count() };
    if (iterations % 60 != 0)
        return;

    printf(" %14.6f s || %10llu u ][ %10.4f ms | %10.4f fps ][ %10.4f avg fps ]\n",
           elapsed_time,                                    // elapsed time (seconds)
           iterations,                                      // loop iterations
           delta_time * 1000.0f,                            // delta time (ms)
           1.0f / delta_time,                               // current fps
           static_cast<float>(iterations) / elapsed_time);  // avg fps
}

// run , calls all the methods
void Game::run() {
    static int lastTime = 0;

    // Main Game Loop
    while (gameloop) {
        lastFrame = SDL_GetTicks();

        m_timer.tick([&] {
            const float delta_time{ m_timer.delta() };
            this->input(delta_time);
            this->update(delta_time);
            this->draw(delta_time);
            print_loop_stats(delta_time);
        });
    }
}
