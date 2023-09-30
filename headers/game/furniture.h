#ifndef GAME_FURNITURE_H
#define GAME_FURNITURE_H

#include <SDL2/SDL_render.h>

#include "../game_object.h"

namespace game
{
    class Furniture : public GameObject
    {
    public:
        Furniture(std::string name, std::string texPath);
        bool isDragging = false;
    };
}

#endif
