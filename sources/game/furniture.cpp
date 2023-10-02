#include "../../headers/game/furniture.h"

namespace game
{
    Furniture::Furniture(std::string name, std::string texPath, float mass, std::vector<std::string> roomNames)
        : PhysicsObject({0, 0}, {32, 32}, {0, 0}, name, texPath, mass) {
            compatableRooms = roomNames;
        }

    bool Furniture::compatableWith(std::string roomName)
    {
        for (auto name : compatableRooms)
        {
            if(name == roomName)
            {
                return true;
            }
        }
        return false;
    }
        
    FurnitureMeta::FurnitureMeta(int maxAmount, float weight, const std::vector<std::string>& rooms)
        : maxAmount(maxAmount), weight(weight), compatableRooms(rooms) {}

    FurnitureLoader::FurnitureLoader() {}
    
    void FurnitureLoader::loadFurnitureData(const std::string& filename) 
    {
        std::map<std::string, FurnitureMeta> furnitureMap;

        std::ifstream file(filename);
        if (file.is_open()) 
        {
            std::string line;
            while (std::getline(file, line)) 
            {
                size_t delimiterPos = line.find(':');

                std::string key = line.substr(0, delimiterPos);
                
                std::string rest = line.substr(delimiterPos + 1);
                rest.erase(std::remove_if(rest.begin(), rest.end(), ::isspace), rest.end());

                size_t amountIndex = rest.find(',');
                size_t massIndex = rest.find(',', amountIndex + 1);

                // TODO: Turn below code into subfunction
                std::string amountStr = rest.substr(0, amountIndex);
                amountStr = amountStr.substr(3, amountStr.length()-1);
                size_t openingParPos = amountStr.find('(');
                size_t closingParPos = amountStr.find(')');
                amountStr = amountStr.substr(openingParPos+1, closingParPos-1);

                std::string massStr = rest.substr(amountIndex + 1, massIndex - amountIndex - 1);
                massStr = massStr.substr(4, massStr.length()-1);
                openingParPos = massStr.find('(');
                closingParPos = massStr.find(')');
                massStr = massStr.substr(openingParPos+1, closingParPos-1);

                std::string compStr = rest.substr(massIndex + 1);
                compStr = compStr.substr(4, compStr.length()-1);
                openingParPos = compStr.find('(');
                closingParPos = compStr.find(')');
                compStr = compStr.substr(openingParPos+1, closingParPos-1);

                std::istringstream ss(compStr);
                std::string token;
                std::vector<std::string> result;

                while (std::getline(ss, token, ',')) 
                {
                    result.push_back(token);
                }

                furnitureMap.insert(std::make_pair(key, FurnitureMeta(std::stoi(amountStr), std::stof(massStr), result)));
            }
            file.close();
        } 
        else 
        {
            std::cerr << "Error: Unable to open the file " << filename << std::endl;
        }

        furniture = furnitureMap;
    }

    std::vector<Box*> FurnitureLoader::loadBoxes(SDL_Renderer* renderer, std::string dir)
    {
        std::cout << "Door dir: " << dir << std::endl;
        
        std::vector<Box*> boxes;

        std::random_device rd;
        std::mt19937 generator(rd());

        int xPosDistStart = 0;
        int xPosDistEnd = 0;
        int yPosDistStart = 0;
        int yPosDistEnd = 0;
        if(dir == "top")
        {
            xPosDistStart=240;
            xPosDistEnd=1080-240;
            yPosDistStart = 0+32;
            yPosDistEnd = 160-32;
        }
        else if(dir == "bottom")
        {
            xPosDistStart=240;
            xPosDistEnd=1080-240;
            yPosDistStart = 720-160+32;
            yPosDistEnd = 720-32;
        }

        std::uniform_int_distribution<int> xPosDist(xPosDistStart, xPosDistEnd);
        std::uniform_int_distribution<int> yPosDist(yPosDistStart, yPosDistEnd);

        for (auto const& [key, val] : furniture)
        {
            std::uniform_int_distribution<int> amountDist(1, val.maxAmount);
            int amount = amountDist(generator);

            for(int i = 0; i < amount; i++)
            {
                Furniture* furniturePtr = new Furniture {
                    key,
                    "./resources/furniture/" + key + ".png", 
                    val.weight,
                    val.compatableRooms
                };
                furniturePtr->loadTexture(renderer);

                boxes.push_back(
                    new Box (
                        {xPosDist(generator), yPosDist(generator)}, 
                        "Box",
                        "./resources/box.png",
                        furniturePtr
                    )
                );
            }
        }

        return boxes;
    }
}
