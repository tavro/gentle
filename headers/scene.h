#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <vector>
#include "./game_object.h"

class Scene
{
	public:
		Scene();

        void render(SDL_Renderer* renderer);

        void setActive(bool state);

        void handleEvent(SDL_Event* e);

        void addObj(GameObject* obj);

    private:
        std::vector<GameObject*> objs;
        bool isActive = true;
};

#endif // SCENE_H