#ifndef HOUSE_GENERATOR_H
#define HOUSE_GENERATOR_H

#include <random>
#include <algorithm>

#include "../game_object.h"
#include "../scene.h"
#include "./room.h"
#include "../utils/vector2d.h"

#include "../house-generator/Room.h"
#include "../house-generator/RoomHandler.h"

namespace game
{
    class HouseGenerator
    {
    public:
        HouseGenerator();

        std::vector<GameObject*> generateWalls();
        std::vector<Room*> generateRooms();

        int houseW = 600;
        int houseH = 400;
        int screenW = 1080;
        int screenH = 720;
        int xOff = screenW/2 - houseW/2;
        int yOff = screenH/2 - houseH/2;
        int wallThickness = 4;

        Rectangle container = { houseW, houseH };

        RoomHandler* roomHandler = nullptr;
    };
}

#endif
