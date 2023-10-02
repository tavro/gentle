#include "../../headers/game/game.h"

#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>

#include "../../headers/game_object.h"
#include "../../headers/scene.h"
#include "../../headers/utils/constants.h"

#include "../../headers/game/house_generator.h"
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
        , fpsText({"", 0, SCREEN_HEIGHT - 28*2})
    {
        scoreText = new Text{"Score:0", 0, SCREEN_HEIGHT - 28*3};
        placedFurnText = new Text{"Placed:0/0", 0, SCREEN_HEIGHT - 28*4};
        currentFurnText = new Text{"Placeholder", 0, 0};
        tutorialText = new Text{"Press [E] to place furniture. Use [Mouse Wheel] to rotate furniture.", 0, SCREEN_HEIGHT - 28};
        audioSource.addMusic( "./resources/Gamejam.wav" );
        audioSource.addSound( "./resources/scratch.wav" );
        audioSource.addSound( "./resources/high.wav" );

        std::map<std::string, FurnitureMeta> furniture {
            {"bed",  			{1 , 12, {"Bedroom"}}}, 
            {"sofa", 			{1 , 10, {"Living Room"}}}, 
            {"piano",			{1 , 20, {"Bedroom", "Living Room"}}}, 
            //{"plant",			{21, 3 , {"Bedroom", "Living Room", "Bathroom"}}},
            {"oven", 			{1 , 15, {"Kitchen"}}},
            {"bathtub", 		{1 , 10, {"Bathroom"}}}, 
            //{"bedsidetable",	{2 , 6 , {"Bedroom"}}},
            //{"bookshelf", 		{2 , 7 , {"Bedroom", "Living Room"}}}, 
            //{"chair", 			{12, 5 , {"Bedroom", "Living Room", "Kitchen"}}},
            //{"coffeetable", 	{2 , 7 , {"Bedroom", "Living Room", "Kitchen"}}},
            //{"dinnertable", 	{2 , 10, {"Living Room", "Kitchen"}}},
            {"dishwasher", 		{1 , 13, {"Kitchen"}}},
            //{"dresser", 		{2 , 9 , {"Bedroom", "Living Room"}}},
            {"fridge", 			{1 , 14, {"Kitchen"}}}, 
            //{"lamp", 			{14, 3 , {"Bedroom", "Living Room", "Kitchen", "Bathroom"}}}, 
            //{"sink", 			{2 , 12, {"Kitchen", "Bathroom"}}}, 
            {"toilet", 			{1 , 7 , {"Bathroom"}}}, 
            {"washingmachine", 	{1 , 13, {"Bathroom"}}},
            {"washingstation", 	{1 , 7 , {"Bathroom"}}}
        };

        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<int> xPositionDistribution(240, SCREEN_WIDTH-240);
        std::uniform_int_distribution<int> yPositionDistribution(0+32, 160-32);
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
                furnitureAmount++;
            }
        }

        HouseGenerator houseGenerator{};
        rooms = houseGenerator.generateRooms();
        testObjs = houseGenerator.generateWalls();
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
            room->loadFloorImage(renderer);
            room->loadNameText(renderer);
        }

        for (auto* testObj : testObjs)
        {
            testObj->loadTexture(renderer);
        }

        fpsText.loadFont("./resources/fonts/bebasneue-regular.ttf", 28);
        fpsText.loadTexture(renderer);
        canvas.addObj(&fpsText);

        scoreText->loadFont("./resources/fonts/bebasneue-regular.ttf", 28);
        scoreText->loadTexture(renderer);
        canvas.addObj(scoreText);

        placedFurnText->loadFont("./resources/fonts/bebasneue-regular.ttf", 28);
        placedFurnText->loadTexture(renderer);
        canvas.addObj(placedFurnText);

        currentFurnText->loadFont("./resources/fonts/bebasneue-regular.ttf", 14);
        currentFurnText->loadTexture(renderer);

        tutorialText->loadFont("./resources/fonts/bebasneue-regular.ttf", 28);
        tutorialText->loadTexture(renderer);
        canvas.addObj(tutorialText);

        cursor.loadTexture(renderer);

        for (auto box : boxes)
        {
            box->loadTexture(renderer);
            box->furniture->loadTexture(renderer);
        }

        if( Mix_PlayingMusic() == 0 )
		{
			Mix_PlayMusic( audioSource.getMusic(0), -1 );
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

        for (auto* testObj : testObjs)
        {
            testObj->render(renderer);
        }

        for (auto box : boxes)
            box->render(renderer);
        
        if (currFurn != nullptr) 
        {
            currFurn->render(renderer);
            currentFurnText->render(renderer);
        }
        
        for (auto furniture : placedFurn)
            furniture->render(renderer);

        for (auto* checkpoint : checkpoints)
        {
            checkpoint->render(renderer);
        }

        fpsText.render(renderer);
        scoreText->render(renderer);
        placedFurnText->render(renderer);
        tutorialText->render(renderer);

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
                score += 10;
                activeRoom->setColor(0, 255, 0);
				Mix_PlayChannel( -1, audioSource.getSound(1), 0 );
            }
            else
            {
                score -= 10;
                activeRoom->setColor(255, 0, 0);
				Mix_PlayChannel( -1, audioSource.getSound(0), 0 );
            }

            scoreText->updateContent("Score:" + std::to_string(score));
            scoreText->loadTexture(renderer);

            placedFurnText->updateContent("Placed:" + std::to_string(placedFurn.size()) + "/" + std::to_string(furnitureAmount));
            placedFurnText->loadTexture(renderer);

            if(boxes.size() == 0) // Done furnishing
            {
                furnished = true;

                // Pick furniture randomly from placedFurn.
                size_t amount = placedFurn.size()/2;
                std::sample(
                    placedFurn.begin(),
                    placedFurn.end(),
                    std::back_inserter(furnToVisit),
                    amount,
                    std::mt19937{std::random_device{}()}
                );

                for(auto* furn : furnToVisit)
                {
                    GameObject* tmp = new GameObject{{furn->getPosition().getX()-32+furn->getSize().getX()/2, furn->getPosition().getY()-32+furn->getSize().getY()/2}, {64, 64}, {0, 0}, "checkpoint", "./resources/ring.png"};
                    tmp->loadTexture(renderer);
                    checkpoints.push_back(tmp);
                }
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
            if (event->key.keysym.sym == SDLK_SPACE) 
            {
                int indexToRemove = -1;
                for(int i = 0; i < checkpoints.size(); i++)
                {
                    if(checkpoints[i]->isInside(cursor.getPosition().getX(), cursor.getPosition().getY()))
                    {
                        indexToRemove = i;
                        break;
                    }
                }
                if(indexToRemove >= 0)
                {
                    checkpoints.erase(checkpoints.begin() + indexToRemove);
                }
            }
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
            currentFurnText->setPosition(currFurn->getPosition().getX(), currFurn->getPosition().getY()-currFurn->getSize().getY());
            currentFurnText->updateContent(currFurn->getName());
            currentFurnText->loadTexture(renderer);
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

        if(furnished && checkpoints.size() == 0) // "GAME OVER"
        {
            std::cout << "YOU WON: SAVE HIGHSCORE TO FILE!" << std::endl;
        }
    }
}
