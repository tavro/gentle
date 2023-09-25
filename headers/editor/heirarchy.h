// TODO: Implement
// * List GameObjects in active scene
// * Select GameObject to show in inspector

#ifndef HEIRARCHY_H
#define HEIRARCHY_H

#include "../scene.h"

class Heirarchy
{
    public:
        Heirarchy();

        void setActiveScene(Scene* scene);

        void render(SDL_Renderer* renderer);

    private:
        Scene* activeScene;
        Canvas canvas;
};

#endif // HEIRARCHY_H