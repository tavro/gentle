#ifndef ROOM_H
#define ROOM_H

#include "./furniture.h"
#include "../game_object.h"
#include "../image.h"
#include "../text.h"
#include "../utils/vector2d.h"
#include <map>

namespace game
{
    class Room
    {
    public:
        Room(int x, int y, int w, int h, std::string n, int xOff, int yOff);

        bool isInside(int x, int y);

        void loadFloorImage(SDL_Renderer* renderer);
        void loadNameText(SDL_Renderer* renderer);

        void render(SDL_Renderer* renderer);

        std::string getName();

        Vector2D position;
        Vector2D size;

        std::string name;

        Text* nameText = nullptr;
        Image* floorImage = nullptr;

        void setColor( Uint8 r, Uint8 g, Uint8 b );

        int xOffset = 0;
        int yOffset = 0;

    private:
        Uint8 red = 255;
        Uint8 green = 255;
        Uint8 blue = 255;
        std::map<std::string, std::string> floorMap {
            {"Living Room", "./resources/floor.png"},
            {"Dining Room", "./resources/floor.png"},
            {"Pantry",      "./resources/floor.png"},
            {"Kitchen",     "./resources/kitchen-floor.png"},
            {"Laundry",     "./resources/floor.png"},
            {"Bedroom",     "./resources/floor.png"},
            {"Bathroom",    "./resources/bathroom-floor.png"},
        };
    };
}

#endif
