#include "../../headers/game/game.h"

#include <vector>

#include "../../headers/game_object.h"
#include "../../headers/scene.h"
#include "../../headers/utils/constants.h"

namespace game
{
    Game::Game(SDL_Renderer *renderer) : renderer(renderer), cursor(Cursor("Cursor")), fpsText({"", 0, SCREEN_HEIGHT - 28})
    {
        boxes = {
            new Box({96, 0}, "Box 1", "./resources/folder.png", "Furn 1", "./resources/gameobject.png"),
            new Box({32, 0}, "Box 2", "./resources/folder.png", "Furn 2", "./resources/gameobject.png")
        };
    }

    Game::~Game()
    {
        for (auto box : boxes)
        {
            free(box);
        }
        if (currFurniture != nullptr)
        {
            free(currFurniture);
        }
        for (auto furniture : placedFurniture)
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
        
        for (auto box : boxes)
        {
            box->loadTexture(renderer);
            box->furniture->loadTexture(renderer);
        }
        cursor.loadTexture(renderer);

        return success;
    }

    void Game::update(float avgFPS, SDL_Event &event)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        cursor.getPosition().set(mouseX - 32/2, mouseY - 32/2);

        hoveredBox = nullptr;
        for (auto box : boxes)
        {
            if (box->isInside(cursor.getPosition().getX() + 32/2, cursor.getPosition().getY() + 32/2))
            {
                hoveredBox = box;
                break;
            }
        }
        cursor.isHoveringBox = hoveredBox != nullptr;

        cursor.isHoveringFurn = currFurniture != nullptr && currFurniture->isInside(cursor.getPosition().getX() + 32/2, cursor.getPosition().getY() + 32/2);

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

        if (currFurniture != nullptr && currFurniture->isDragging)
        {
            Vector2D &cursorPos = cursor.getPosition();
            currFurniture->setPosition(cursorPos);
        }
    }

    void Game::render()
    {
        fpsText.render(renderer);

        for (auto box : boxes)
        {
            box->render(renderer);
        }
        if (currFurniture != nullptr)
        {
            currFurniture->render(renderer);
        }
        for (auto furniture : placedFurniture)
        {
            furniture->render(renderer);
        }
        cursor.render(renderer);
    }

    void Game::onMouseDown(int mouseX, int mouseY)
    {
        cursor.isClosed = true;
        
        if (currFurniture != nullptr && cursor.isHoveringFurn)
        {
            currFurniture->isDragging = true;
        }
        else if (currFurniture == nullptr && hoveredBox != nullptr)
        {
            currFurniture = hoveredBox->furniture;
            for (auto iter = boxes.begin(); iter != boxes.end(); iter++)
            {
                if (*iter == hoveredBox)
                {
                    boxes.erase(iter);
                    break;
                }
            }
            
            currFurniture->setPosition(hoveredBox->getPosition());

            free(hoveredBox);
            hoveredBox = nullptr;
        }
    }

    void Game::onMouseUp(int mouseX, int mouseY)
    {
        cursor.isClosed = false;
        if (currFurniture != nullptr)
        {
            currFurniture->isDragging = false;
        }
    }
}
