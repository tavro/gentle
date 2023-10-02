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
            RoomId("Kitchen", sizeDistribution3(generator)), 
            RoomId("Laundry", sizeDistribution1(generator)),
            RoomId("DiningRoom", sizeDistribution2(generator)),
            RoomId("Bedroom", sizeDistribution2(generator)),
            RoomId("Bathroom", sizeDistribution1(generator))
        };

        std::shuffle(roomOrder.begin(), roomOrder.end(), generator);

        auto boolgen = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());
        bool b = boolgen();

        if(b)
        {
            roomOrder.insert(roomOrder.begin() + 1, RoomId("LivingRoom", sizeDistribution2(generator)));
        }
        else
        {
            roomOrder.push_back(RoomId("LivingRoom", sizeDistribution2(generator)));
        }

        roomHandler = new RoomHandler{roomOrder, container};
        roomHandler->createRooms();

        dir = roomHandler->dir;
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
                walls.push_back(genWall(static_cast<int>(floor(lineSegment.startX)), static_cast<int>(floor(lineSegment.startY)), static_cast<int>(floor(lineSegment.endX)), static_cast<int>(floor(lineSegment.endY)), xOff, yOff, wallThickness));
            }
        }
        return walls;
    }

    std::vector<Room*> HouseGenerator::generateRooms()
    {
        std::vector<Room*> rooms;
        for(const auto& room : roomHandler->getRooms())
        {
            rooms.push_back(new Room{(int)room.x, (int)room.y, (int)room.width, (int)room.height, room.roomId.name, xOff, yOff});
        }
        return rooms;
    }
}
