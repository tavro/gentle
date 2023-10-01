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
        void update(float avgFPS, SDL_Event &event);
        void render();
    private:
        void onMouseDown(int mouseX, int mouseY);
        void onMouseUp(int mouseX, int mouseY);

        SDL_Renderer *renderer = nullptr;
        
        Text fpsText;
        Cursor cursor;

        Box *hoveredBox = nullptr;
        std::vector<Box *> boxes;
        Furniture *currFurniture = nullptr;
        std::vector<Furniture *> placedFurniture;
    };
}

#endif
