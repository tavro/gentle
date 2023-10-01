#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <vector>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>

#include "../../headers/game/box.h"
#include "../../headers/game/cursor.h"
#include "../../headers/game/furniture.h"

#include "../../headers/canvas.h"
#include "../../headers/text.h"

namespace game
{
    class Game
    {
    public:
        Game(SDL_Renderer *renderer);
        ~Game();
        bool loadMedia(Canvas &canvas);
        void handleEvent(SDL_Event *event);
        void update(float avgFPS);
        void render();
    private:
        SDL_Renderer *renderer = nullptr;
        
        Text fpsText;
        Cursor cursor;

        std::vector<Box *> boxes;
        Furniture *currFurn = nullptr;
        std::vector<Furniture *> placedFurn;

        Box *hoveredBox = nullptr;
        Furniture *hoveredFurn = nullptr;
    };
}

#endif
