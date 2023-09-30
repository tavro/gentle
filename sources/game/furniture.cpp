#include "../../headers/game/furniture.h"

namespace game
{
    Furniture::Furniture(Vector2D pos, Vector2D size, Vector2D vel, std::string name, std::string texPath) : GameObject(pos, size, vel, name, texPath) {}
}
