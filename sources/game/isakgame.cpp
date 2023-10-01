#include "../../headers/game/game.h"

#include <map>
#include <vector>
#include <iostream>

#include "../../headers/game_object.h"
#include "../../headers/scene.h"
#include "../../headers/utils/constants.h"

#include "../../headers/game/room_generator.h"
#include "../../headers/game/room.h"

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
        std::map<std::string, FurnitureMeta> furniture {
            {"bed",  			{1 , 12, {"Bedroom"}}}, 
            {"sofa", 			{1 , 10, {"Living Room"}}}, 
            {"piano",			{1 , 20, {"Bedroom", "Living Room"}}}, 
            {"plant",			{21, 3 , {"Bedroom", "Living Room", "Bathroom"}}},
            {"oven", 			{1 , 15, {"Kitchen"}}},
            {"bathtub", 		{1 , 10, {"Bathroom"}}}, 
            {"bedsidetable",	{2 , 6 , {"Bedroom"}}},
            {"bookshelf", 		{2 , 7 , {"Bedroom", "Living Room"}}}, 
            {"chair", 			{12, 5 , {"Bedroom", "Living Room", "Kitchen"}}},
            {"coffeetable", 	{2 , 7 , {"Bedroom", "Living Room", "Kitchen"}}},
            {"dinnertable", 	{2 , 10, {"Living Room", "Kitchen"}}},
            {"dishwasher", 		{1 , 13, {"Kitchen"}}},
            {"dresser", 		{2 , 9 , {"Bedroom", "Living Room"}}},
            {"fridge", 			{1 , 14, {"Kitchen"}}}, 
            {"lamp", 			{14, 3 , {"Bedroom", "Living Room", "Kitchen", "Bathroom"}}}, 
            {"sink", 			{2 , 12, {"Kitchen", "Bathroom"}}}, 
            {"toilet", 			{1 , 7 , {"Bathroom"}}}, 
            {"washingmachine", 	{1 , 13, {"Bathroom"}}},
            {"washingstation", 	{1 , 7 , {"Bathroom"}}}
        };

        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<int> xPositionDistribution(240, SCREEN_WIDTH-240);
        std::uniform_int_distribution<int> yPositionDistribution(0, 160);
        std::uniform_int_distribution<int> velocityDistribution(-10, 10);

        for (auto const& [key, val] : furniture)
        {
            std::uniform_int_distribution<int> amountDistribution(1, val.maxAmount);
            int amount = amountDistribution(generator);

            for(int i = 0; i < amount; i++)
            {
                Furniture* furniturePtr = new Furniture {
                    key,
                    "./resources/furniture/" + key + ".png", 
                    val.weight,
                    val.compatableRooms
                };
                furniturePtr->loadTexture(renderer);

                boxes.push_back(new Box(
                    {xPositionDistribution(generator), yPositionDistribution(generator)}, 
                    "Box", 
                    "./resources/box.png",
                    furniturePtr));
            }
        }

        RoomGenerator roomGenerator{4};
        rooms = roomGenerator.generateRoomScene();
    }

    Game::~Game()
    {
        for (auto box : boxes)
            free(box);
        if (currFurn != nullptr)
            free(currFurn);
        for (auto furn : placedFurn)
            free(furn);
        
        //free(roomScene); TODO: rooms
    }

    bool Game::loadMedia(Canvas &canvas)
    {
        bool success = true;

        background.getTexture().loadFromFile("./resources/grass.png", renderer);

        for (auto* room : rooms)
        {
            for (auto* wall : room->getWalls())
            {
                wall->loadTexture(renderer);
            }
            room->loadFloorImage(renderer);
            room->loadNameText(renderer);
        }

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
        background.render(renderer);

        for (auto* room : rooms)
        {
            room->render(renderer);
        }

        for (auto box : boxes)
            box->render(renderer);
        if (currFurn != nullptr)
            currFurn->render(renderer);
        for (auto furniture : placedFurn)
            furniture->render(renderer);

        fpsText.render(renderer);

        cursor.updateTexture(renderer);
        cursor.render(renderer);
    }

    void Game::placeFurn()
    {
        if (currFurn)
        {
            placedFurn.push_back(currFurn);

            int x = currFurn->getPosition().getX();
            int y = currFurn->getPosition().getY();

            Room* activeRoom = nullptr;
            for (auto* room : rooms)
            {
                if(room->isInside(x, y))
                {
                    activeRoom = room;
                    break;
                }
            }

            std::string roomName = activeRoom->getName();
            if(currFurn->compatableWith(roomName))
            {
                std::cout << "YOU GET POINTS" << std::endl;
            }
            else
            {
                std::cout << "WTF! NO POINTS FOR YOU!" << std::endl;
            }

            if(boxes.size() == 0)
            {
                // NOTE: ALL FURNITURE PLACED!
            }

            currFurn = nullptr;
        }
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

            bool canRotate = currFurn->isDragging || currFurn->getCurrentState() != State::MOUSE_OUT;
            if (event->type == SDL_MOUSEWHEEL && canRotate)
            {
                float rotationAmount = event->wheel.y * 5;
                currFurn->increaseRotation(rotationAmount);
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
        case SDL_KEYDOWN:
            if (event->key.keysym.sym == SDLK_e)
                placeFurn();
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

            float deltaTime = 1 / avgFPS; // TODO: pass in as parameter instead?
            currFurn->setVelocity(moveDir * deltaTime * 10); // TODO: make heavier objects more sluggish

            // TODO: make unable to move out of bounds
        }

        if (currFurn)
        {
            currFurn->move();
            currFurn->rotate();
            std::vector<GameObject *> others;
            for (auto furn : placedFurn)
                others.push_back(furn);
            currFurn->handleCollisions(others);
        }
        for (auto furn : placedFurn)
        {
            furn->move();
            furn->rotate();
            std::vector<GameObject *> others;
            for (auto otherFurn : placedFurn)
                if (furn != otherFurn)
                    others.push_back(furn);

            if (currFurn)
                others.push_back(currFurn);
            furn->handleCollisions(others);
        }

        if (currFurn && currFurn->isDragging)
            cursor.setPosition(currFurn->getPosition()); // TODO: need to adjust so it's centered?
        else
            cursor.setPosition(mousePos - cursor.getSize() / 2);
    }
}
