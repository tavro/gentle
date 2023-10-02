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
        nameText->color = {136, 8, 8};
        nameText->loadFont( "./resources/fonts/bebasneue-regular.ttf", 20 );
        nameText->loadTexture( renderer );
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
