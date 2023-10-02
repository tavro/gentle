#include "../../headers/game/house_generator.h"
#include "../../headers/game/room.h"

namespace game
{
    HouseGenerator::HouseGenerator() {
        std::random_device rd;
        std::mt19937 generator(rd());

        std::uniform_int_distribution<int> sizeDistribution1(2, 5);
        std::uniform_int_distribution<int> sizeDistribution2(6, 12);
        std::uniform_int_distribution<int> sizeDistribution3(2, 8);

        std::vector<RoomId> roomOrder = {
            RoomId("LivingRoom", sizeDistribution2(generator)),
            RoomId("Kitchen", sizeDistribution3(generator)), 
            RoomId("Laundry", sizeDistribution1(generator)),
            RoomId("DiningRoom", sizeDistribution2(generator)),
            RoomId("Bedroom", sizeDistribution2(generator)),
            RoomId("Bathroom", sizeDistribution1(generator))
        };

        std::shuffle(roomOrder.begin(), roomOrder.end(), generator);

        roomHandler = new RoomHandler{roomOrder, container};
        roomHandler->createRooms();
    }

    GameObject* genWall(float x1, float y1, float x2, float y2, int xOffset, int yOffset, float thickness)
    {
        if(x1 == x2)
        {
            return new GameObject{{x1+xOffset, y1+yOffset}, {x2+thickness+xOffset, y2+yOffset}};
        }
        else
        {
            return new GameObject{{x1+xOffset, y1+yOffset}, {x2+xOffset, y2+thickness+yOffset}};
        }
    }

    std::vector<GameObject*> HouseGenerator::generateWalls()
    {
        std::vector<GameObject*> walls;
        for(const auto& wall : roomHandler->walls)
        {
            for(const auto& lineSegment : wall.lineSegments)
            {
                walls.push_back(genWall(lineSegment.startX, lineSegment.startY, lineSegment.endX, lineSegment.endY, xOff, yOff, wallThickness));
            }
        }
        return walls;
    }

    std::vector<Room*> HouseGenerator::generateRooms()
    {
        std::vector<Room*> rooms;
        for(const auto& room : roomHandler->getRooms())
        {
            rooms.push_back(new Room{room.x, room.y, room.width, room.height, room.roomId.name, xOff, yOff});
        }
        return rooms;
    }
}
