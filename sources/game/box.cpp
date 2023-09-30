#include "../../headers/game/box.h"

namespace game
{
    Box::Box(Vector2D pos, std::string name, std::string texPath, std::string furnName, std::string furnTexPath) : GameObject(pos, {32, 32}, {0, 0}, name, texPath) {
        furniture = new Furniture(furnName, furnTexPath);
    }
}
