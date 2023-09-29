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

// TODO: Add empty GameObject

class Heirarchy
{
    public:
        Heirarchy(int x, int y, int w, int h);

        void setActiveScene(Scene* scene, SDL_Renderer* renderer);

        Scene* getActiveScene();

        void render(SDL_Renderer* renderer);

        void increaseIndex();

        void loadTextures(SDL_Renderer* renderer);

        void reset();

        void decreaseIndex();

        std::string getActiveObjName();

    private:
        Texture texture;
        Texture texture2;

        UIPanel panel{0, 0, 256, 256};

        Scene* activeScene;

        std::vector<std::string> names;
        int index = 0;
        int maxIndex;
};

#endif // HEIRARCHY_H