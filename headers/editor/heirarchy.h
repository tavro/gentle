// TODO: Implement
// * List GameObjects in active scene
// * Select GameObject to show in inspector

#ifndef HEIRARCHY_H
#define HEIRARCHY_H

#include <vector>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../text.h"
#include "../scene.h"
#include "../ui_panel.h"
#include "../game_object.h"

class Heirarchy
{
    public:
        Heirarchy(int x, int y, int w, int h);

        void setActiveScene(Scene* scene, SDL_Renderer* renderer);

        void render(SDL_Renderer* renderer);

    private:
        UIPanel panel{0, 0, 256, 256};

        Scene* activeScene;

        std::vector<std::string> names;
        int index = 0;
        int maxIndex;
};

#endif // HEIRARCHY_H