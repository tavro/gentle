#include "../../headers/game/game.h"

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
        , fpsText({"", 0, 96})
    {
        
    }

    Game::~Game()
    {

    }

    bool Game::loadMedia(Canvas &canvas)
    {
        bool success = true;

        return success;
    }

    void Game::render()
    {

    }

    void Game::handleEvent(SDL_Event *event)
    {
        
    }

    void Game::update(float avgFPS)
    {
        
    }
}
