#include "../../headers/game/game.h"

#include "../../headers/scene.h"

Scene *testScene = new Scene();

bool game::loadMedia(SDL_Renderer *renderer)
{
    bool success = true;

    testScene->load("./resources/otherTest.scene");
    testScene->loadTextures(renderer);

    return success;
}

// TODO: add ability to interact with GOs

void game::render(SDL_Renderer *renderer)
{
    testScene->render(renderer);
}