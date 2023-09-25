// TODO: Implement
// Show visual representation of fields of selected object from heirarchy

#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "../game_object.h"

class Inspector
{
    public:
        Inspector();

        void setActiveObj(GameObject* gameObject);

        void render(SDL_Renderer* renderer);

    private:
        GameObject* activeObj;
        Canvas canvas;
};

#endif // INSPECTOR_H