#include "../../headers/game/room_generator.h"
#include "../../headers/game/room.h"

namespace game
{
    RoomGenerator::RoomGenerator(int thickness) {
        wallThickness = thickness;
    }

    std::vector<Room*> RoomGenerator::generateRoomScene() {
        std::vector<Room*> rooms;
        rooms.push_back(new Room{0, 0, 292, 300, "Living Room", xOffset, yOffset});
        rooms.push_back(new Room{0, 300, 292, 100, "Dining Room", xOffset, yOffset});
        rooms.push_back(new Room{292, 0, 307, 190, "Pantry", xOffset, yOffset});
        rooms.push_back(new Room{292, 190, 223, 209, "Kitchen", xOffset, yOffset});
        rooms.push_back(new Room{516, 190, 83, 209, "Laundry", xOffset, yOffset});
        rooms.push_back(new Room{0, 300, 58, 100, "Bedroom", xOffset, yOffset});
        rooms.push_back(new Room{58, 300, 175, 100, "Bathroom", xOffset, yOffset});
        rooms.push_back(new Room{233, 300, 58, 100, "Living Room", xOffset, yOffset});

        for (auto room : rooms)
        {
            room->generateWalls(wallThickness);
        }
        return rooms;
    }

}
