#pragma once

#ifndef ROOM_HANDLER
#define ROOM_HANDLER

#include <stack>
#include <unordered_map>

#include "Room.h"
#include "SquarifiedTreemap.hpp"
#include <optional>

class RoomHandler
{
public:
	RoomHandler(std::vector<RoomId> roomTree, Rectangle houseSize);
	void createRooms();
	void createOuterWalls();

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


	// Return a random wall number. 
	// Left, right, top, bottom = 1, 2, 3, 4 respectively.
	static uint8_t randomWallNumber(std::vector<uint8_t> validWalls); 

	std::string entranceRoomName = "LivingRoom";

	void findRoomNeighbors();
};

#endif