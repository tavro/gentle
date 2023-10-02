#include "../../headers/house-generator/RoomHandler.h"

RoomHandler::RoomHandler(std::vector<RoomId> roomTree, Rectangle houseSize) {
	this->roomTree = roomTree;
	this->houseSize = houseSize;
}

void RoomHandler::createRooms() {
	processNodes(this->roomTree, {});
	findRoomNeighbors();
}

void RoomHandler::processNodes(std::vector<RoomId>& nodes, std::optional<GeneratedRoom> parent)
{
	Rectangle container;
	if (parent.has_value()) {
		GeneratedRoom* room = &parent.value();
		container = Rectangle{ (float)(room->width), (float)(room->height), (float)(room->x), (float)(room->y) };
	}
	else {
		container = houseSize;
	}
	std::vector<std::pair<GeneratedRoom, std::vector<RoomId>>> roomIdsWithChildren;
	std::vector<RoomId> childrenToVisit;

	// Process root nodes
	Array<Number> dataPoints;
	for (auto& node : nodes) {
		if (parent.has_value()) {
			node.area = parent.value().roomId.name;
		}

		dataPoints.emplace_back((float)node.size);

		if (node.hasChildren) {
			int idx = 0;
			for (auto& child : node.children) {
				childrenToVisit.emplace_back(child);
				idx++;
			}
			areas.emplace_back(node);
		}
	}
	// datapoints
	// container
	auto result = SolveSquarifiedTreemap(dataPoints, container);
	for (int i = 0; i < result.size(); ++i) {
		auto& rect = result[i];
		auto& childRoomId = nodes[i];
		GeneratedRoom room = GeneratedRoom(rect.X, rect.Y, rect.Width, rect.Height, childRoomId);
		this->rooms.emplace_back(room);

		if (childRoomId.hasChildren) {
			std::pair<GeneratedRoom, std::vector<RoomId>> roomChildren = std::make_pair(room, childrenToVisit);
			roomIdsWithChildren.emplace_back(roomChildren);
		}
	}

	if (!roomIdsWithChildren.empty()) {
		for (auto& room : roomIdsWithChildren) {
			std::vector<RoomId> children = room.second;
			GeneratedRoom parent = room.first;
			processNodes(children, std::make_optional(parent));
		}
	}
}

void RoomHandler::findRoomNeighbors()
{
	this->rooms[0].isConnectedToStart = true;
	for (auto& room : this->rooms) {
		room.isVisited = true;
		if (room.roomId.hasChildren) {
			continue;
		}
		for (auto& other : this->rooms) {
			if (&room == &other || other.roomId.hasChildren || other.isVisited) {
				continue;
			}

			bool xOverlap = (room.x <= (other.x + other.width)) && ((room.x + room.width) >= other.x);
			bool yOverlap = (room.y <= (other.y + other.height)) && ((room.y + room.height) >= other.y);

			if (!xOverlap || !yOverlap) {
				continue;
			}
			room.addNeighbor(other);

			// Create wall segments
			float xStart = std::max(room.x, other.x);
			float xEnd = std::min(room.x + room.width, other.x + other.width);

			float yStart = std::max(room.y, other.y);
			float yEnd = std::min(room.y + room.height, other.y + other.height);

			bool isDoor = !other.isConnectedToStart;
			
			bool hasArea = room.roomId.area != "Default";
			if (room.roomId.area == other.roomId.area) {
				if (hasArea) {
					isDoor = true;
				}
			}
			// Doors to areas: only make a door to the largest child
			else {
				RoomId currArea;
				for (auto& area : this->areas) {
					if (area.name == other.roomId.area) {
						currArea = area;
					}
				}
				if (&currArea != nullptr) {
					auto largestChild = findLargestRoom(currArea.children);
					if (largestChild.has_value()) {
						if (other.roomId.name == largestChild.value().roomId.name) {
							isDoor = true;
						}
						else {
							isDoor = false;
						}
					}
				}				
			}
			walls.emplace_back(Wall(xStart, yStart, xEnd, yEnd, isDoor));
			other.isConnectedToStart = true;
		}
	}
}

std::vector<GeneratedRoom> RoomHandler::getRooms()
{
	return this->rooms;
}

std::optional<GeneratedRoom> RoomHandler::findLargestRoom(std::vector<RoomId>& rooms) {
	if (rooms.empty()) {
		return {};
	}

	RoomId largestRoomId = rooms[0];

	for (RoomId& roomId : rooms) {
		if (roomId.size > largestRoomId.size) {
			largestRoomId = roomId;
		}
	}
	std::optional<GeneratedRoom> optionalLargestRoom = findRoomWithId(largestRoomId);

	if (optionalLargestRoom.has_value()) {
		GeneratedRoom largestRoom = optionalLargestRoom.value();
		return largestRoom;
	}
	return {};
}

std::optional<GeneratedRoom> RoomHandler::findRoomWithId(RoomId id)
{
	for (auto& room : this->rooms) {
		if (room.roomId.name == id.name) {
			return std::make_optional(room);
		}
	}
	return {};
}
