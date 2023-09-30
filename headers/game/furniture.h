#ifndef GAME_FURNITURE_H
#define GAME_FURNITURE_H

#include "../game_object.h"

namespace game
{
    class Furniture : GameObject
    {
        Furniture(Vector2D pos, Vector2D size, Vector2D vel, std::string name, std::string texPath);
    };
}


#endif
