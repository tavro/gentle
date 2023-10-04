#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <vector>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>

#include "../../headers/editor/cursor.h"

#include "../../headers/canvas.h"
#include "../../headers/text.h"
#include "../../headers/scene.h"
#include "../../headers/image.h"
#include "../../headers/audio_source.h"

namespace game
{
    class Game
    {
    public:
        Game(SDL_Renderer *renderer);
        ~Game();
        bool loadMedia();
        void handleEvent(SDL_Event *event);
        void update(float avgFPS);
        void render();
        void reset();

    private:
        SDL_Renderer *renderer = nullptr;
        
        Text fpsText;
        Cursor cursor;
    };
}

#endif
