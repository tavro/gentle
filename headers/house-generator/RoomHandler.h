#pragma once

#ifndef ROOM_HANDLER
#define ROOM_HANDLER

#include <stack>
#include <unordered_map>
#include <optional>

#include "./Room.h"
#include "./SquarifiedTreemap.hpp"

class RoomHandler
{
public:
	RoomHandler(std::vector<RoomId> roomTree, Rectangle houseSize);
	void createRooms();

	std::vector<Wall> walls;

	std::vector<GeneratedRoom> getRooms();

	std::optional<GeneratedRoom> findLargestRoom(std::vector<RoomId>& rooms);
	std::optional<GeneratedRoom> findRoomWithId(RoomId id);

private:
	std::vector<RoomId> roomTree;
	void processNodes(std::vector<RoomId>& nodes, std::optional<GeneratedRoom> parent);
	Rectangle houseSize; // houseSize.Width, houseSize.Height (I think)
	std::vector<GeneratedRoom> rooms;
	std::vector<RoomId> areas;

	void findRoomNeighbors();
};

#endif