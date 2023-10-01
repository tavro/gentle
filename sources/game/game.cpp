#include "../../headers/game/game.h"

#include <vector>

#include "../../headers/game_object.h"
#include "../../headers/scene.h"
#include "../../headers/utils/constants.h"

static Vector2D getMousePos()
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    return {(float)mouseX, (float)mouseY};
}

namespace game
{
    Game::Game(SDL_Renderer *renderer)
        : renderer(renderer)
        , cursor(Cursor("Cursor"))
        , fpsText({"", 0, SCREEN_HEIGHT - 28})
    {
        boxes = {
            new Box({96, 0}, "Box 1", "./resources/folder.png", "Furn 1", "./resources/gameobject.png", 1.0f),
            new Box({32, 0}, "Box 2", "./resources/folder.png", "Furn 2", "./resources/gameobject.png", 1.0f)
        };
    }

    Game::~Game()
    {
        for (auto box : boxes)
            free(box);
        if (currFurn != nullptr)
            free(currFurn);
        for (auto furn : placedFurn)
            free(furn);
    }

    bool Game::loadMedia(Canvas &canvas)
    {
        bool success = true;

        fpsText.loadFont("./resources/font.ttf", 28);
        fpsText.loadTexture(renderer);
        canvas.addObj(&fpsText);

        cursor.loadTexture(renderer);

        for (auto box : boxes)
        {
            box->loadTexture(renderer);
            box->furniture->loadTexture(renderer);
        }

        return success;
    }

    void Game::render()
    {
        fpsText.render(renderer);

        for (auto box : boxes)
            box->render(renderer);
        if (currFurn != nullptr)
            currFurn->render(renderer);
        for (auto furniture : placedFurn)
            furniture->render(renderer);

        cursor.updateTexture(renderer);
        cursor.render(renderer);
    }

    void Game::handleEvent(SDL_Event *event)
    {
        Box *clickedBox = nullptr;

        for (auto box : boxes)
        {
            box->handleEvent(event);

            switch (box->getCurrentState())
            {
            case State::MOUSE_OVER_MOTION:
                cursor.isHovering = true;
                break;
            case State::MOUSE_OUT:
                cursor.isHovering = false;
                break;
            case State::MOUSE_DOWN:
                if (!currFurn)
                    clickedBox = box;
                break;
            }
        }

        if (currFurn)
        {
            currFurn->handleEvent(event);
            
            switch (currFurn->getCurrentState())
            {
            case State::MOUSE_OVER_MOTION:
                cursor.isHovering = true;
                break;
            case State::MOUSE_OUT:
                cursor.isHovering = false;
                break;
            case State::MOUSE_DOWN:
                currFurn->isDragging = true;
                break;
            }
        }

        if (clickedBox)
        {
            for (auto i = boxes.begin(); i != boxes.end(); i++)
            {
                Box *box = *i;
                if (box == clickedBox)
                {
                    boxes.erase(i);
                    currFurn = box->furniture;
                    currFurn->setPosition(box->getPosition()); // TODO: need to make centered?
                    free(box);
                    break;
                }
            }
        }
            
        switch (event->type)
        {
        case SDL_MOUSEBUTTONDOWN:
            cursor.isClosed = true;
            break;
        case SDL_MOUSEBUTTONUP:
            cursor.isClosed = false;
            if (currFurn)
                currFurn->isDragging = false;
            break;
        }
    }

    void Game::update(float avgFPS)
    {
        std::stringstream fpsTextStream;
        fpsTextStream.str("");
        fpsTextStream << "Average FPS: " << avgFPS;

        fpsText.updateContent(fpsTextStream.str());
        fpsText.loadTexture(renderer);

        if (currFurn)
        {
            cursor.isHovering = currFurn->getCurrentState() != State::MOUSE_OUT;
        }
        else
        {
            cursor.isHovering = false;
            for (auto box : boxes)
            {
                if (box->getCurrentState() != State::MOUSE_OUT)
                {
                    cursor.isHovering = true;
                    break;
                }
            }
        }

        Vector2D mousePos = getMousePos();
        if (currFurn && currFurn->isDragging)
        {
            currFurn->setPosition(mousePos - currFurn->getSize() / 2); // TODO: move smoothly instead
            cursor.setPosition(currFurn->getPosition()); // TODO: need to adjust so it's centered?
        }
        else
        {
            cursor.setPosition(mousePos - cursor.getSize() / 2);
        }
    }
}
