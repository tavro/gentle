#include "../../headers/game/game.h"

#include "../../headers/game/cursor.h"
#include "../../headers/game/furniture.h"

#include "../../headers/game_object.h"
#include "../../headers/scene.h"
#include "../../headers/text.h"
#include "../../headers/utils/constants.h"

Text fpsText{"", 0, SCREEN_HEIGHT - 28};

game::Cursor cursor = game::Cursor("Cursor");
game::Furniture box = game::Furniture({0, 0}, {32, 32}, {0, 0}, "Box", "./resources/gameobject.png");

namespace game
{
    static void onMouseDown(int mouseX, int mouseY, SDL_Renderer *renderer)
    {
        cursor.isClosed = true;
        box.isDragging = cursor.isHovering;
    }

    static void onMouseUp(int mouseX, int mouseY, SDL_Renderer *renderer)
    {
        cursor.isClosed = false;
        box.isDragging = false;
    }

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

    void update(SDL_Renderer *renderer, float avgFPS, SDL_Event *event)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        cursor.getPosition().set(mouseX - 32/2, mouseY - 32/2);

        cursor.isHovering = box.isInside(cursor.getPosition().getX()+32/2, cursor.getPosition().getY()+32/2);

        switch (event->type)
        {
        case SDL_MOUSEBUTTONDOWN:
            onMouseDown(mouseX, mouseY, renderer);
            break;
        case SDL_MOUSEBUTTONUP:
            onMouseUp(mouseX, mouseY, renderer);
            break;
        }
        
        cursor.updateTexture(renderer);

        std::stringstream timeText;
        timeText.str("");
        timeText << "Average FPS: " << avgFPS;

        fpsText.updateContent(timeText.str());
        fpsText.loadTexture(renderer);

        if (box.isDragging)
        {
            Vector2D &cursorPos = cursor.getPosition();
            box.setPosition(cursorPos);
        }
    }

    void render(SDL_Renderer *renderer)
    {
        fpsText.render(renderer);

        box.render(renderer);
        cursor.render(renderer);
    }
}
