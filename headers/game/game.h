#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <vector>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>

#include "../../headers/game/box.h"
#include "../../headers/game/cursor.h"
#include "../../headers/game/furniture.h"
#include "../../headers/game/harold.h"
#include "../../headers/game/room.h"

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
        bool loadMedia(Canvas &canvas);
        void handleEvent(SDL_Event *event);
        void update(float avgFPS);
        void render();
    private:
        void placeFurn();

        SDL_Renderer *renderer = nullptr;
        
        Text fpsText;
        Cursor cursor;

        Harold *harold = nullptr;

        std::vector<Box *> boxes;
        Furniture *currFurn = nullptr;
        std::vector<Furniture *> placedFurn;
        std::vector<Furniture *> furnToVisit;

        std::vector<Room*> rooms;
        std::vector<GameObject*> testObjs;
        std::vector<GameObject*> checkpoints;

        Box *hoveredBox = nullptr;
        Furniture *hoveredFurn = nullptr;

        Image background{0, 0, 1080, 720};
        Image floor{240, 160, 600, 400};

        int score = 0;
        Text* scoreText = nullptr;

        int furnitureAmount = 0;
        Text* placedFurnText = nullptr;

        Text* tutorialText = nullptr;
        Text* currentFurnText = nullptr;
        
        Image mainMenuBackground{0, 0, 1080, 720};
        Image highscoreBackground{0, 0, 1080, 720};
        Text* scoreboardTitleText = nullptr;
        std::vector<Text*> highscores;

        AudioSource audioSource;

        bool furnished = false;
        bool gameOver = false;
    };
}

#endif
