#ifndef GAME_HAROLD_H
#define GAME_HAROLD_H

#include "../animation.h"
#include "../game_object.h"

namespace game
{
    class Harold : public GameObject
    {
    public:
        Harold(Vector2D pos);
        void updateTexture(SDL_Renderer *renderer);
        
        bool canControl = true; // TODO: should not be true until all furniture are placed;
    private:
        Texture idleTexture;
        Animation walkAnimation;
    };
}

#endif
