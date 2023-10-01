#ifndef GAME_FURNITURE_H
#define GAME_FURNITURE_H

#include <SDL2/SDL_render.h>

#include "../physics_object.h"

namespace game
{
    class Furniture : public PhysicsObject
    {
    public:
        Furniture(std::string name, std::string texPath, float mass);
        bool isDragging = false;
    };

    struct FurnitureMeta {
        int maxAmount;
        float weight;
        std::vector<std::string> compatableRooms;

        FurnitureMeta(int maxAmount, float weight, const std::vector<std::string>& rooms);
    };
}

#endif
