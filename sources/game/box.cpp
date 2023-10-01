#include "../../headers/game/box.h"

namespace game
{
    Box::Box(Vector2D pos, std::string name, std::string texPath, Furniture* furniturePtr) : GameObject(pos, {32, 32}, {0, 0}, name, texPath) {
        furniture = furniturePtr;
    }
}
