#include "../../headers/game/furniture.h"

namespace game
{
    Furniture::Furniture(std::string name, std::string texPath, float mass)
        : PhysicsObject({0, 0}, {32, 32}, {0, 0}, name, texPath, mass) {}

        
    FurnitureMeta::FurnitureMeta(int maxAmount, float weight, const std::vector<std::string>& rooms)
        : maxAmount(maxAmount), weight(weight), compatableRooms(rooms) {}
}
