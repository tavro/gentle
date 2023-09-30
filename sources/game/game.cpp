#include "../../headers/game/game.h"

#include <vector>

#include "../../headers/game_object.h"
#include "../../headers/scene.h"
#include "../../headers/utils/constants.h"

namespace game
{
    Game::Game(SDL_Renderer *renderer) : renderer(renderer), cursor(Cursor("Cursor")), fpsText({"", 0, SCREEN_HEIGHT - 28})
    {
        furnitureList = {
            new Furniture({0, 0}, {32, 32}, {0, 0}, "Box", "./resources/gameobject.png"),
            new Furniture({32, 0}, {32, 32}, {0, 0}, "Box 2", "./resources/gameobject.png")
        };
    }

    Game::~Game()
    {
        for (auto furniture : furnitureList)
        {
            free(furniture);
        }
    }

    bool Game::loadMedia(Canvas &canvas)
    {
        bool success = true;
        
        success = !fpsText.loadFont("./resources/font.ttf", 28);
        success = !fpsText.loadTexture(renderer);
	    canvas.addObj(&fpsText);
        
        for (auto furniture : furnitureList)
        {
            furniture->loadTexture(renderer);
        }
        cursor.loadTexture(renderer);

        return success;
    }

    void Game::update(float avgFPS, SDL_Event &event)
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

        switch (event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            onMouseDown(mouseX, mouseY);
            break;
        case SDL_MOUSEBUTTONUP:
            onMouseUp(mouseX, mouseY);
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

    void Game::render()
    {
        fpsText.render(renderer);

        for (auto furniture : furnitureList)
        {
            furniture->render(renderer);
        }
        cursor.render(renderer);
    }

    void Game::onMouseDown(int mouseX, int mouseY)
    {
        cursor.isClosed = true;
        cursor.draggedFurniture = cursor.hoveredFurniture;
    }

    void Game::onMouseUp(int mouseX, int mouseY)
    {
        cursor.isClosed = false;
        cursor.draggedFurniture = nullptr;
    }
}
