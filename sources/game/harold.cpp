#include "../../headers/game/harold.h"

namespace game
{
    Harold::Harold(Vector2D pos)
        : GameObject(pos, {16, 22}, {0, 0}, "Harold", "./resources/haroldstill.png")
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
            rotation = 0;
            break;
        case SDLK_a:
            getVelocity().increaseX(-1.0f);
            rotation = 270;
            break;
        case SDLK_s:
            getVelocity().increaseY(1.0f);
            rotation = 180;
            break;
        case SDLK_d:
            getVelocity().increaseX(1.0f);
            rotation = 90;
            break;
        }
    }

    void Harold::onKeyUp(SDL_Keycode keyCode)
    {
        switch (keyCode)
        {
        case SDLK_w:
            getVelocity().increaseY(1.0f);
            playAnimation = false;
            break;
        case SDLK_a:
            getVelocity().increaseX(1.0f);
            playAnimation = false;
            break;
        case SDLK_s:
            getVelocity().increaseY(-1.0f);
            playAnimation = false;
            break;
        case SDLK_d:
            getVelocity().increaseX(-1.0f);
            playAnimation = false;
            break;
        }
    }

    void Harold::move()
    {
        Vector2D normalVel = getVelocity().getNormalizedVector();
        getPosition() += normalVel*2;
    }

    void Harold::loadAnimation(SDL_Renderer *renderer)
    {
        Animation* anim = new Animation{};
        anim->loadTexture( renderer, "./resources/harold-anim.png" );
        idleTexture.loadFromFile("./resources/haroldstill.png", renderer);
        activeAnimation = anim;
    }

    void Harold::renderAnimation(SDL_Renderer *renderer)
    {
        if(playAnimation)
        {
            activeAnimation->render({getPosition().getX(), getPosition().getY()}, renderer, rotation);
        }
        else
        {
            idleTexture.render( getPosition().getX(), getPosition().getY(), NULL, rotation, NULL, SDL_FLIP_NONE, renderer);
        }
    }


}
