#include "../../headers/game/game.h"

#include "../../headers/game_object.h"
#include "../../headers/scene.h"
#include "../../headers/text.h"
#include "../../headers/utils/constants.h"

Text fpsText{"", 0, SCREEN_HEIGHT - 28};

GameObject cursor{{SCREEN_WIDTH, SCREEN_HEIGHT}, {32, 32}, {0, 0}, "Cursor", "./resources/hand-point.png"};
GameObject box{{0, 0}, {32, 32}, {0, 0}, "Box", "./resources/gameobject.png"};

bool cursorIsClosed = false; // TODO: implement class Cursor
bool isDraggingBox = false; // TODO: implement class Furniture

namespace game
{
    static void onMouseDown(int mouseX, int mouseY, SDL_Renderer *renderer)
    {
        cursor.setTexturePath("./resources/hand-closed.png");
        cursor.loadTexture(renderer);
        cursorIsClosed = true;
        isDraggingBox = box.isInside(cursor.getPosition().getX()+32/2, cursor.getPosition().getY()+32/2);
    }

    static void onMouseUp(int mouseX, int mouseY, SDL_Renderer *renderer)
    {
        cursorIsClosed = false;
        isDraggingBox = false;
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

        switch (event->type)
        {
        case SDL_MOUSEBUTTONDOWN:
            onMouseDown(mouseX, mouseY, renderer);
            break;
        case SDL_MOUSEBUTTONUP:
            onMouseUp(mouseX, mouseY, renderer);
            break;
        }

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
            else
            {
                cursor.setTexturePath("./resources/hand-closed.png");
                cursor.loadTexture(renderer);
            }
        }

        std::stringstream timeText;
        timeText.str("");
        timeText << "Average FPS: " << avgFPS;

        fpsText.updateContent(timeText.str());
        fpsText.loadTexture(renderer);

        if (isDraggingBox)
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
