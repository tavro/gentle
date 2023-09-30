#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <SDL2/SDL_render.h>

#include "../../headers/canvas.h"

namespace game
{
    bool loadMedia(SDL_Renderer *renderer, Canvas *canvas);
    void update(SDL_Renderer *renderer, float avgFPS, SDL_Event *event);
    void render(SDL_Renderer *renderer);
}

#endif
