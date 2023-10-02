#ifndef GAME_FURNITURE_H
#define GAME_FURNITURE_H

#include <SDL2/SDL_render.h>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <vector>
#include <string>
#include <map>

#include "../physics_object.h"
#include "./box.h"

namespace game
{
    class Box;

    class Furniture : public PhysicsObject
    {
    public:
        Furniture(std::string name, std::string texPath, float mass, std::vector<std::string> roomNames);
        
        bool compatableWith(std::string roomName);
        
        bool isDragging = false;
        std::vector<std::string> compatableRooms;
    };

    struct FurnitureMeta {
        int maxAmount;
        float weight;
        std::vector<std::string> compatableRooms;

        FurnitureMeta(int maxAmount, float weight, const std::vector<std::string>& rooms);
    };

    class FurnitureLoader
    {
    public:
        FurnitureLoader();

        void loadFurnitureData(const std::string& filename);
        std::vector<Box*> loadBoxes(SDL_Renderer* renderer, std::string dir);

    private:
        std::map<std::string, FurnitureMeta> furniture;
    };
}

#endif
