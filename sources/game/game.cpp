#include "../../headers/game/game.h"

#include "../../headers/game_object.h"
#include "../../headers/scene.h"
#include "../../headers/text.h"
#include "../../headers/utils/constants.h"

Text fpsText{"", 0, SCREEN_HEIGHT - 28};

GameObject cursor{{SCREEN_WIDTH, SCREEN_HEIGHT}, {32, 32}, {0, 0}, "Cursor", "./resources/hand-point.png"};
GameObject box{{0, 0}, {32, 32}, {0, 0}, "Box", "./resources/gameobject.png"};

namespace game
{
    bool loadMedia(SDL_Renderer *renderer, Canvas *canvas)
    {
        bool success = true;
        
        success = !fpsText.loadFont("./resources/font.ttf", 28);
        success = !fpsText.loadTexture(renderer);
	    canvas->addObj(&fpsText);
        
        box.loadTexture(renderer);
        cursor.loadTexture(renderer);

        return success;
    }

    void update(SDL_Renderer *renderer, float avgFPS) // TODO: implementera deltaTime
    {
        bool cursorIsClosed = false;

        if(box.isInside(cursor.getPosition().getX()+32/2, cursor.getPosition().getY()+32/2))
        {
            if(!cursorIsClosed)
            {
                cursor.setTexturePath("./resources/hand.png");
                cursor.loadTexture(renderer);
            }
        }
        else
        {
            if(!cursorIsClosed)
            {
                cursor.setTexturePath("./resources/hand-point.png");
                cursor.loadTexture(renderer);
            }
        }

        std::stringstream timeText;
        timeText.str("");
        timeText << "Average FPS: " << avgFPS;

        fpsText.updateContent(timeText.str());
        fpsText.loadTexture(renderer);

        int x, y;
        SDL_GetMouseState(&x, &y);
        cursor.getPosition().set(x - 32/2, y - 32/2);
    }

    void render(SDL_Renderer *renderer)
    {
        fpsText.render(renderer);

        box.render(renderer);
        cursor.render(renderer);
    }
}
