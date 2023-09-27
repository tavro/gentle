// TODO: Implement
// Show visual representation of fields of selected object from heirarchy

#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <string>

#include "../game_object.h"
#include "../text.h"
#include "../texture.h"

class Inspector
{
    public:
        Inspector(int x, int y);

        void setActiveObj(GameObject* gameObject);

        void render(SDL_Renderer* renderer);
        
        void loadFont(SDL_Renderer* renderer);

    private:
        Vector2D position;

        GameObject* activeObj;

        Texture texture;

        Text nameText{"Name: ''", 0, 0};
        Text posText {"Position: (X:, Y:)", 0, 0};
        Text sizeText{"Size: (X:, Y:)", 0, 0};
        Text velocityText{"Velocity: (X:, Y:)", 0, 0};
};

#endif // INSPECTOR_H