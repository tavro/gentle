#include "../../headers/house-generator/Room.h"

// ------------------ LineSegment

LineSegment::LineSegment()
{
}

LineSegment::LineSegment(float startX, float startY, float endX, float endY)
{
	this->startX = startX;
	this->startY = startY;
	this->endX = endX;
	this->endY = endY;
}


// ------------------ Wall

Wall::Wall(float startX, float startY, float endX, float endY, bool hasDoor)
{
	this->startX = startX;
	this->startY = startY;
	this->endX = endX;
	this->endY = endY;
	this->hasDoor = hasDoor;

	isHorizontal = startY == endY;

	if (doorSize > length()) {
		doorSuccess = false;
	}

	// Place random door
	if (openRandomDoors && doorSuccess && !this->hasDoor) {
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		if (r < forceDoorChance) {
			this->hasDoor = true;
		}
	}
	generateLineSegments();
}

void Wall::generateLineSegments()
{
	if (hasDoor && doorSuccess) {
		float center = calculateDoorLocation();

		//float center = length() / 2;
		if (isHorizontal) {
			lineSegments.emplace_back(LineSegment(startX, startY, startX + center - doorSize / 2, endY));
			lineSegments.emplace_back(LineSegment(startX + center + doorSize / 2, startY, endX, endY));
		}
		else {
			lineSegments.emplace_back(LineSegment(startX, startY, endX, startY + center - doorSize / 2));
			lineSegments.emplace_back(LineSegment(startX, startY + center + doorSize / 2, endX, endY));
		}
	}
	else {
		lineSegments.emplace_back(LineSegment(startX, startY, endX, endY));
	}
}

float Wall::length()
{
	return std::sqrt(std::pow(endX - startX, 2) + std::pow(endY - startY, 2));
}

float Wall::calculateDoorLocation()
{
	float len = length();
	// If wall is short: place in center
	if (len < 2 * doorSize) {
		return len / 2;
	}
	// Otherwise, pick a random location along wall
	float random = ((float)rand()) / (float)RAND_MAX;
	float range = (length() - doorSize / 2) - doorSize / 2;
	return (random * range) + doorSize / 2;
}	


// ------------------ GeneratedRoom Id

RoomId::RoomId()
{
}

RoomId::RoomId(std::string name, int size)
{
	this->name = name;
	this->size = size;
}

RoomId::RoomId(std::string name, int size, std::vector<RoomId> children)
{
	this->name = name;
	this->size = size;
	this->children = children;
	this->hasChildren = true;
}

bool RoomId::operator==(const RoomId& other) const
{
	return this->area == other.area && this->name == other.name && this->size == other.size;
}

// ------------------ GeneratedRoom 

GeneratedRoom::GeneratedRoom(float x, float y, float width, float height, RoomId roomId)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->roomId = roomId;
}

void GeneratedRoom::addNeighbor(GeneratedRoom& neighbor)
{
	neighbors.emplace_back(neighbor);
}

std::vector<GeneratedRoom> GeneratedRoom::getNeighbors() const
{
	return neighbors;
}

void GeneratedRoom::addConnectedNeighbor(const GeneratedRoom& neighbor)
{
	connectedNeighbors.emplace_back(neighbor);
}

std::vector<GeneratedRoom> GeneratedRoom::getConnectedNeighbors() const
{
	return connectedNeighbors;
}

std::ostream& operator<<(std::ostream& os, const GeneratedRoom& room)
{
	os << "(x,y): (" << room.x << ", " << room.y << ") ,  (w,h): (" << room.width << ", " << room.height << ") \n";
	os << "Name: " << room.roomId.name << ",  size: " << room.roomId.size << ",  area: " << room.roomId.area;
	return os;
}

