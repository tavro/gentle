#include <iostream>

#include "../../headers/house-generator/Room.h"
#include "../../headers/house-generator/RoomHandler.h"

int main()
{
    std::vector<RoomId> roomOrder = {
        RoomId("Living room", 11),
        RoomId("Kitchen area", 8, {RoomId("Pantry", 2), RoomId("Kitchen", 3), RoomId("Laundry", 1)}),
        RoomId("Dining room", 6),
        RoomId("Bedroom", 10),
        RoomId("Bathroom", 3)
    };

    int width = 600, height = 400;

    Rectangle container = { width, height };

    RoomHandler roomHandler = RoomHandler(roomOrder, container);
    roomHandler.createRooms();

    for(const auto& room : roomHandler.getRooms())
    {
        std::cout << room.roomId.name << std::endl;
    }

    for(const auto& wall : roomHandler.walls)
    {
        for(const auto& lineSegment : wall.lineSegments)
        {
            std::cout << "Start(" << lineSegment.startX << "," << lineSegment.startY << ")" << std::endl;
            std::cout << "End(" << lineSegment.endX << "," << lineSegment.endY << ")" << std::endl;
        }
    }

    // Line segments:
    // for wall in roomHandler.walls
    //      for lineSegment in wall.lineSegments

    // for room in roomHandler.getRooms()
    //     access room.x, room.y, room.width, room.height
    //     here you can also find room.roomId
    //          roomId contains: room name (roomId.name), input size

    // house size: roomHandler.houseSize (todo: make a getter)

    return 0;
}