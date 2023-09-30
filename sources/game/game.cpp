#include "../../headers/game/game.h"

#include <vector>

#include "../../headers/game/cursor.h"
#include "../../headers/game/furniture.h"

#include "../../headers/game_object.h"
#include "../../headers/scene.h"
#include "../../headers/text.h"
#include "../../headers/utils/constants.h"

Text fpsText{"", 0, SCREEN_HEIGHT - 28};

game::Cursor cursor = game::Cursor("Cursor");

std::vector<game::Furniture *> furnitureList{
    new game::Furniture({0, 0}, {32, 32}, {0, 0}, "Box", "./resources/gameobject.png"),
    new game::Furniture({32, 0}, {32, 32}, {0, 0}, "Box 2", "./resources/gameobject.png")
};

namespace game
{
    static void onMouseDown(int mouseX, int mouseY, SDL_Renderer *renderer)
    {
        cursor.isClosed = true;
        cursor.draggedFurniture = cursor.hoveredFurniture;
    }

    static void onMouseUp(int mouseX, int mouseY, SDL_Renderer *renderer)
    {
        cursor.isClosed = false;
        cursor.draggedFurniture = nullptr;
    }

    bool loadMedia(SDL_Renderer *renderer, Canvas *canvas)
    {
        bool success = true;
        
        success = !fpsText.loadFont("./resources/font.ttf", 28);
        success = !fpsText.loadTexture(renderer);
	    canvas->addObj(&fpsText);
        
        for (auto furniture : furnitureList)
        {
            furniture->loadTexture(renderer);
        }
        cursor.loadTexture(renderer);

        return success;
    }

    void update(SDL_Renderer *renderer, float avgFPS, SDL_Event *event)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        cursor.getPosition().set(mouseX - 32/2, mouseY - 32/2);

        if (cursor.draggedFurniture == nullptr)
        {
            cursor.hoveredFurniture = nullptr;
            for (auto furniture : furnitureList)
            {
                if (furniture->isInside(cursor.getPosition().getX()+32/2, cursor.getPosition().getY()+32/2)) {
                    cursor.hoveredFurniture = furniture;
                    break;
                }
            }
        }

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

        if (cursor.draggedFurniture != nullptr)
        {
            Vector2D &cursorPos = cursor.getPosition();
            cursor.draggedFurniture->setPosition(cursorPos);
        }
    }

    void render(SDL_Renderer *renderer)
    {
        fpsText.render(renderer);

        for (auto furniture : furnitureList)
        {
            furniture->render(renderer);
        }
        cursor.render(renderer);
    }
}
