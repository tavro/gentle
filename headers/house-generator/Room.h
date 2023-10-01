#ifndef ROOM
#define ROOM

#include <string>
#include <vector>
#include <iostream>
#include <ostream>
#include <cmath>

struct LineSegment
{
public:
	LineSegment();
	LineSegment(float startX, float startY, float endX, float endY);
	float startX, endX;
	float startY, endY;
};

struct Wall
{
public:
	Wall(float startX, float startY, float endX, float endY, bool hasDoor);
	float startX, startY;
	float endX, endY;
	bool hasDoor;

	std::vector<LineSegment> lineSegments;
	void generateLineSegments();

	bool isHorizontal = false;
	float doorSize = 50;
private:
	float length();
};

struct RoomId
{
public:
	RoomId();
	RoomId(std::string name, int size);
	RoomId(std::string name, int size, std::vector<RoomId> children);

	std::string name = "";
	int size = 0;
	std::vector<RoomId> children = {};
	bool hasChildren = false;

	std::string area = "Default";

	bool operator==(const RoomId& other) const;
};

struct GeneratedRoom
{	
	GeneratedRoom(float x, float y, float width, float height, RoomId roomId);
	
	float x, y;
	float width, height;
	RoomId roomId;

	void addNeighbor(GeneratedRoom &neighbor);
	std::vector<GeneratedRoom> getNeighbors() const;

	void addConnectedNeighbor(const GeneratedRoom& neighbor);
	std::vector<GeneratedRoom> getConnectedNeighbors() const;
	bool isConnectedToStart = false;

	friend std::ostream& operator<<(std::ostream& os, const GeneratedRoom& room);

	bool isVisited = false; // Internal check

private:
	std::vector<GeneratedRoom> neighbors;
	std::vector<GeneratedRoom> connectedNeighbors; // Neighbors with open door to room !! TODO !!
	
	std::vector<LineSegment> walls;
};

#endif