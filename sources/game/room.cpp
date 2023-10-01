#include "../../headers/game/room.h"

namespace game
{
    Room::Room(int x, int y, int w, int h, std::string n, int xOff, int yOff)
    {
        position.set(x, y);
        size.set(w, h);
        name = n;

        xOffset = xOff;
        yOffset = yOff;

        floorImage = new Image(x+xOffset, y+yOffset, w, h);
        floorImage->crop = true;
        nameText = new Text{name, x+xOffset+8, y+yOffset+8};
    }

    void Room::generateWalls(int wallThickness)
    {
        int x1 = position.getX();
        int y1 = position.getY();
        int x2 = size.getX() + x1;
        int y2 = size.getY() + y1;

        Vector2D startUpper{xOffset + x1, yOffset + y1};
        Vector2D endUpper{xOffset + x2, yOffset + y1+wallThickness};

        Vector2D startLower{xOffset + x1, yOffset + y2-wallThickness};
        Vector2D endLower{xOffset + x2, yOffset + y2+wallThickness};

        Vector2D startLeft{xOffset + x1, yOffset + y1};
        Vector2D endLeft{xOffset + x1+wallThickness, yOffset + y2};

        Vector2D startRight{xOffset + x2-wallThickness, yOffset + y1};
        Vector2D endRight{xOffset + x2, yOffset + y2};

        walls.push_back(new GameObject{startUpper, endUpper});
        walls.push_back(new GameObject{startLower, endLower});
        walls.push_back(new GameObject{startLeft , endLeft});
        walls.push_back(new GameObject{startRight, endRight});
    }

    bool Room::isInside(int x, int y)
    {
        bool xInBound = x >= position.getX()+xOffset && x <= position.getX()+xOffset + size.getX();
        bool yInBound = y >= position.getY()+yOffset && y <= position.getY()+yOffset + size.getY();
        return xInBound && yInBound;
    }

    void Room::loadFloorImage(SDL_Renderer* renderer)
    {
        floorImage->getTexture().loadFromFile(floorMap[name], renderer);
    }

    void Room::loadNameText(SDL_Renderer* renderer)
    {
        nameText->loadFont( "./resources/font.ttf", 14 );
        nameText->loadTexture( renderer );
    }

    std::vector<GameObject*> Room::getWalls()
    {
        return walls;
    }

    void Room::render(SDL_Renderer* renderer)
    {
        floorImage->render(renderer);
        floorImage->getTexture().setColor(red, green, blue);
        if(red<255){
            if(red+3 > 255) {
                red = 255;
            }
            else {
                red+=3;
            }
        }
        if(green<255){
            if(green+3 > 255) {
                green = 255;
            }
            else {
                green+=3;
            }
        }
        if(blue<255){
            if(blue+3 > 255) {
                blue = 255;
            }
            else {
                blue+=3;
            }
        }
        nameText->render(renderer);
        for (auto* wall : getWalls())
        {
            wall->render(renderer);
        }
    }

    std::string Room::getName()
    {
        return name;
    }

    void Room::setColor( Uint8 r, Uint8 g, Uint8 b )
    {
        red = r;
        green = g;
        blue = b;
    }

}
