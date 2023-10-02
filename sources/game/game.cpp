#include "../../headers/game/game.h"

#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>

#include "../../headers/game_object.h"
#include "../../headers/scene.h"
#include "../../headers/utils/constants.h"

#include "../../headers/game/house_generator.h"
#include "../../headers/game/room.h"

// General TODO: Discuss what should give and remove points
// as well as decide on how many points.

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
        audioSource.addMusic( "./resources/Gamejam.wav" );
        audioSource.addSound( "./resources/scratch.wav" );
        audioSource.addSound( "./resources/high.wav"    );

        scoreText       = new Text{ "Score:0",                                                              0, SCREEN_HEIGHT - 28*3 };
        tutorialText    = new Text{ "Press [E] to place furniture. Use [Mouse Wheel] to rotate furniture.", 0, SCREEN_HEIGHT - 28   };
        placedFurnText  = new Text{ "Placed:0/" + std::to_string(furnitureAmount),                          0, SCREEN_HEIGHT - 28*4 };
        currentFurnText = new Text{ "Placeholder",                                                          0, 0                    };

        HouseGenerator houseGenerator{};
        rooms = houseGenerator.generateRooms();
        walls = houseGenerator.generateWalls();

        FurnitureLoader loader{};
        loader.loadFurnitureData("./resources/furniture/furniture_meta_data.txt");
        boxes = loader.loadBoxes(renderer);
        furnitureAmount = boxes.size();

        harold = new Harold({64, 64}); // TODO: change position
    }

    Game::~Game()
    {
        free(harold);

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

        background.getTexture().loadFromFile(         "./resources/grass.png",      renderer);
        mainMenuBackground.getTexture().loadFromFile( "./resources/mainmenu.png",   renderer);
        highscoreBackground.getTexture().loadFromFile("./resources/scoreboard.png", renderer);

        for (auto* room : rooms)
        {
            room->loadFloorImage(renderer);
            room->loadNameText(renderer);
        }

        for (auto* wall : walls)
        {
            wall->loadTexture(renderer);
        }

        fpsText.loadFont(         "./resources/fonts/bebasneue-regular.ttf", 28);
        scoreText->loadFont(      "./resources/fonts/bebasneue-regular.ttf", 28);
        tutorialText->loadFont(   "./resources/fonts/bebasneue-regular.ttf", 28);
        placedFurnText->loadFont( "./resources/fonts/bebasneue-regular.ttf", 28);
        currentFurnText->loadFont("./resources/fonts/bebasneue-regular.ttf", 14);

        fpsText.loadTexture(         renderer);
        scoreText->loadTexture(      renderer);
        tutorialText->loadTexture(   renderer);
        placedFurnText->loadTexture( renderer);
        currentFurnText->loadTexture(renderer);
        
        canvas.addObj(&fpsText);

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
        if (!gameStarted)
        {
            mainMenuBackground.render(renderer);
        }
        else
        {
            background.render(renderer);

            for (auto* room : rooms)
            {
                room->render(renderer);
            }

            for (auto* wall : walls)
            {
                wall->render(renderer);
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

            harold->updateTexture(renderer);
            harold->render(renderer);

            fpsText.render(renderer);
            scoreText->render(renderer);
            placedFurnText->render(renderer);
            tutorialText->render(renderer);
        }

        if(gameOver)
        {
            highscoreBackground.render(renderer);

            for(auto* highscore : highscores)
            {
                highscore->render(renderer);
            }
            // TODO: Implement reset function and add 'Play Again' button.
        }

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

            if (activeRoom)
            {
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
            }
            else
            {
                score -= 10;
                // TODO: flash background red?
                Mix_PlayChannel( -1, audioSource.getSound(0), 0 );
            }

            scoreText->updateContent("Score:" + std::to_string(score));
            scoreText->loadTexture(renderer);

            placedFurnText->updateContent("Placed:" + std::to_string(placedFurn.size()) + "/" + std::to_string(furnitureAmount));
            placedFurnText->loadTexture(renderer);

            if(boxes.size() == 0) // Done furnishing // TODO: make Harold controlable
            {
                furnished = true;

                // Pick furniture randomly from placedFurn.
                size_t amount = placedFurn.size()/2;
                std::vector<Furniture *> furnToVisit;
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

                harold->canControl = true;
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
                currFurn->setRotationDirection(RotDir::NONE);
                currFurn->setRotationSpeed(0.0f);
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

                    Vector2D furnPos = box->getPosition() + box->getSize() / 2 - currFurn->getSize() / 2;
                    currFurn->setPosition(furnPos);

                    free(box);
                    break;
                }
            }
        }

        harold->handleEvent(event);

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
            if (event->key.keysym.sym == SDLK_p)
                gameStarted = true;
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
        }

        if (currFurn)
        {
            currFurn->move();
            currFurn->rotate();
            std::vector<GameObject *> others;
            for (auto furn : placedFurn)
                others.push_back(furn);
            currFurn->handleCollisions(others);
            currFurn->handleCollisions(walls);
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
            furn->handleCollisions(walls);
        }

        if (currFurn && currFurn->isDragging)
        {
            Vector2D newPos = currFurn->getPosition() + currFurn->getSize() / 2 - cursor.getSize() / 2;
            cursor.setPosition(newPos);
        }
        else
            cursor.setPosition(mousePos - cursor.getSize() / 2);

        harold->move();

        if (!gameOver && furnished)
        {
            if(checkpoints.size() == 0) // "GAME OVER"
            {
                std::ifstream file("./resources/scoreboard.txt");
                std::vector<int> scoreboard;

                int num;
                while (file >> num) {
                    scoreboard.push_back(num);
                }
                file.close();

                scoreboard.push_back(score);

                std::sort(scoreboard.begin(), scoreboard.end(), std::greater<int>());

                std::ofstream out("./resources/scoreboard.txt");
                for (const auto& highscore : scoreboard) {
                    out << highscore << std::endl;
                }
                out.close();

                // Load highscores into vector
                std::ifstream f("./resources/scoreboard.txt");

                std::vector<std::string> lines;
                std::string line;

                int count = 0;
                while (std::getline(f, line) && count < 10) {
                    lines.push_back(line);
                    count++;
                }

                highscores.clear();
                int offset = 0;
                for (const auto& l : lines) {
                    offset++;
                    Text* t = new Text{std::to_string(offset) + ") " + l, SCREEN_WIDTH/2, 28*offset};
                    t->loadFont("./resources/fonts/bebasneue-regular.ttf", 28);
                    t->loadTexture(renderer);
                    highscores.push_back(t);
                }

                f.close(); 

                gameOver = true;
            }
            else // Harold completes task
            {
                int indexToRemove = -1;
                for(int i = 0; i < checkpoints.size(); i++)
                {
                    if(checkpoints[i]->isInside(harold->getPosition().getX(), harold->getPosition().getY()))
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
        }
    }
}
