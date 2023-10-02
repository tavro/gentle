#include "../../headers/game/harold.h"

namespace game
{
    Harold::Harold(Vector2D pos)
        : GameObject(pos, {32, 32}, {0, 0}, "Harold", "./resources/harold/haroldstill.png")
    {
        // TODO: init walk and idle animation
    }

    void Harold::updateTexture(SDL_Renderer *renderer)
    {
        // TODO: change textures if walking or idling
        loadTexture(renderer);
    }

    void Harold::handleEvent(SDL_Event* event)
    {
        if (canControl)
        {
            switch (event->type)
            {
            case SDL_KEYDOWN:
                if (event->key.repeat == 0)
                    onKeyDown(event->key.keysym.sym);
                break;
            case SDL_KEYUP:
                onKeyUp(event->key.keysym.sym);
                break;
            }
        }
    }

    void Harold::onKeyDown(SDL_Keycode keyCode)
    {
        // TODO: basic movement, should be improved
        switch (keyCode)
        {
        case SDLK_w:
            getVelocity().increaseY(-1.0f);
            break;
        case SDLK_a:
            getVelocity().increaseX(-1.0f);
            break;
        case SDLK_s:
            getVelocity().increaseY(1.0f);
            break;
        case SDLK_d:
            getVelocity().increaseX(1.0f);
            break;
        }
    }

    void Harold::onKeyUp(SDL_Keycode keyCode)
    {
        switch (keyCode)
        {
        case SDLK_w:
            getVelocity().increaseY(1.0f);
            break;
        case SDLK_a:
            getVelocity().increaseX(1.0f);
            break;
        case SDLK_s:
            getVelocity().increaseY(-1.0f);
            break;
        case SDLK_d:
            getVelocity().increaseX(-1.0f);
            break;
        }
    }

    void Harold::move()
    {
        Vector2D normalVel = getVelocity().getNormalizedVector();
        getPosition() += normalVel;
    }
}
