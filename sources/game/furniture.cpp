#include "../../headers/game/furniture.h"

namespace game
{
    Furniture::Furniture(std::string name, std::string texPath, float mass, std::vector<std::string> roomNames)
        : PhysicsObject({0, 0}, {32, 32}, {0, 0}, name, texPath, mass) {
            compatableRooms = roomNames;
        }

    bool Furniture::compatableWith(std::string roomName)
    {
        for (auto name : compatableRooms)
        {
            if(name == roomName)
            {
                return true;
            }
        }
        return false;
    }
        
    FurnitureMeta::FurnitureMeta(int maxAmount, float weight, const std::vector<std::string>& rooms)
        : maxAmount(maxAmount), weight(weight), compatableRooms(rooms) {}
}
