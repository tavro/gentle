#ifndef ROOM_GENERATOR_H
#define ROOM_GENERATOR_H

#include "../game_object.h"
#include "../scene.h"
#include "./room.h"
#include "../utils/vector2d.h"

namespace game
{
    class RoomGenerator
    {
    public:
        RoomGenerator(int thickness);

        std::vector<Room*> generateRoomScene();

        int wallThickness;

        int houseWidth = 600;
        int houseHeight = 400;

        int xOffset = 1080/2 - 600/2;
        int yOffset = 720/2 - 400/2;
    };
}

#endif
