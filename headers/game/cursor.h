#ifndef GAME_CURSOR_H
#define GAME_CURSOR_H

#include <SDL2/SDL_render.h>

#include "../game_object.h"
#include "./furniture.h"

namespace game
{
    class Cursor : public GameObject
    {
    public:
        Cursor(std::string name);
        void updateTexture(SDL_Renderer *renderer);
        bool isClosed = false;
        Furniture *hoveredFurniture = nullptr;
        Furniture *draggedFurniture = nullptr;
    };
}

#endif
