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
        void loadAnimation(SDL_Renderer *renderer);
        void renderAnimation(SDL_Renderer *renderer);
        void updateTexture(SDL_Renderer *renderer);
        void handleEvent(SDL_Event* event) override;
        void move();
        
        bool canControl = false;
    private:
        void onKeyDown(SDL_Keycode keyCode);
        void onKeyUp(SDL_Keycode keyCode);
        
        Animation* activeAnimation;
		bool playAnimation = false;

        Texture idleTexture;
    };
}

#endif
