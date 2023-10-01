#ifndef ROOM_GENERATOR_H
#define ROOM_GENERATOR_H

#include "../game_object.h"
#include "../scene.h"
#include "../utils/vector2d.h"

namespace game
{
    class RoomGenerator
    {
    public:
        RoomGenerator(int thickness);

        Scene* generateRoomScene();

        int wallThickness;
        
        int houseWidth = 600;
        int houseHeight = 400;
    };
}

#endif
