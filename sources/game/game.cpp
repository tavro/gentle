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

/*
std::map<std::string, FurnitureMeta> furniture {
    {"bed",  			{1 , 12, {"bedroom"}}}, 
    {"sofa", 			{1 , 10, {"livingroom"}}}, 
    {"piano",			{1 , 20, {"bedroom", "livingroom"}}}, 
    {"plant",			{21, 3 , {"bedroom", "livingroom", "bathroom"}}},
    {"oven", 			{1 , 15, {"kitchen"}}},
    {"bathtub", 		{1 , 10, {"bathroom"}}}, 
    {"bedsidetable",	{2 , 6 , {"bedroom"}}},
    {"bookshelf", 		{2 , 7 , {"bedroom", "livingroom"}}}, 
    {"chair", 			{12, 5 , {"bedroom", "livingroom", "kitchen"}}},
    {"coffeetable", 	{2 , 7 , {"bedroom", "livingroom", "kitchen"}}},
    {"dinnertable", 	{2 , 10, {"livingroom", "kitchen"}}},
    {"dishwasher", 		{1 , 13, {"kitchen"}}},
    {"dresser", 		{2 , 9 , {"bedroom", "livingroom"}}},
    {"fridge", 			{1 , 14, {"kitchen"}}}, 
    {"lamp", 			{14, 3 , {"bedroom", "livingroom", "kitchen", "bathroom"}}}, 
    {"sink", 			{2 , 12, {"kitchen", "bathroom"}}}, 
    {"toilet", 			{1 , 7 , {"bathroom"}}}, 
    {"washingmachine", 	{1 , 13, {"bathroom"}}},
    {"washingstation", 	{1 , 7 , {"bathroom"}}}
};
*/

namespace game
{
    Game::Game(SDL_Renderer *renderer)
        : renderer(renderer)
        , cursor(Cursor("Cursor"))
        , fpsText({"", 0, SCREEN_HEIGHT - 28})
    {
        boxes = {
            new Box({96, 0}, "Box 1", "./resources/folder.png", "Furn 1", "./resources/gameobject.png", 30.0f),
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
            Vector2D moveDir = mousePos - currFurn->getPosition();

            float deltaTime = 1 / avgFPS; // TODO: pass in as parameter instead
            currFurn->setVelocity(moveDir * deltaTime * 10); // TODO: make heavier objects more sluggish

            // TODO: make unable to move out of bounds
        }

        if (currFurn)
            currFurn->move();
        for (auto furn : placedFurn)
            furn->move();

        if (currFurn && currFurn->isDragging)
            cursor.setPosition(currFurn->getPosition()); // TODO: need to adjust so it's centered?
        else
            cursor.setPosition(mousePos - cursor.getSize() / 2);
    }
}
