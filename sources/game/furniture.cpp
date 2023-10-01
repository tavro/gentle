#include "../../headers/game/furniture.h"

namespace game
{
    Furniture::Furniture(std::string name, std::string texPath, float mass) :
        PhysicsObject({0, 0}, {32, 32}, {0, 0}, name, texPath, mass) {}
}
