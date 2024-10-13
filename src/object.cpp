#include <SDL2/SDL_image.h>

#include "object.h"

Object::Object(std::string sprite, SDL_Renderer* renderer)
    : m_spriteFile(std::move(sprite))
    , m_renderer(renderer) {
    this->setTexture(m_spriteFile, renderer);
}

void Object::setDest(int x, int y, int w, int h) {
    m_dest.x = x;
    m_dest.y = y;
    m_dest.w = w;
    m_dest.h = h;
}

void Object::setSource(int x, int y, int w, int h) {
    m_src.x = x;
    m_src.y = y;
    m_src.w = w;
    m_src.h = h;
}

void Object::setTexture(std::string filename, SDL_Renderer* renderer) {
    SDL_Surface* surf = IMG_Load(("assets/images/" + filename).c_str());
    if (surf == nullptr) {
        std::cout << "Error loading image: " << IMG_GetError() << std::endl;
        // return; // Exit the function if the image fails to load
    }
    m_tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);  // Free the surface after creating the texture
    if (m_tex == nullptr)
        std::cout << "Error creating texture: " << SDL_GetError() << std::endl;

    // After creating the texture, set the filtering
    SDL_SetTextureBlendMode(m_tex, SDL_BLENDMODE_BLEND);
    SDL_SetTextureScaleMode(m_tex, SDL_ScaleMode::SDL_ScaleModeNearest);
}

// draw sprite
void Object::draw_sprite(Object& obj, SDL_Renderer* renderer) {
    SDL_Rect dest = obj.getDest();
    SDL_Rect src = obj.getSource();
    SDL_RenderCopyEx(renderer, obj.getTexture(), &src, &dest, 0, nullptr, SDL_FLIP_NONE);
}
