#ifndef GAME_BOX_H
#define GAME_BOX_H

#include "./furniture.h"
#include "../game_object.h"
#include "../utils/vector2d.h"

namespace game
{
    class Furniture;

    class Box : public GameObject
    {
    public:
        Box(Vector2D pos, std::string name, std::string texPath, Furniture* furniturePtr);
        Furniture *furniture = nullptr;
    };
}

#endif
