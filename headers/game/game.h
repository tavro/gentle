#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <SDL2/SDL_render.h>

namespace game
{
    bool loadMedia(SDL_Renderer *renderer);
    void render(SDL_Renderer *renderer);
}

#endif
