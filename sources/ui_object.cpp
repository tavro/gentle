#include "../headers/ui_object.h"

UIObject::UIObject( int x, int y, int w, int h )
{
    setPosition(x, y);
    size.setX(w);
    size.setY(h);
}

void UIObject::setPosition( int x, int y )
{
    position.setX(x);
    position.setY(y);
}

Texture& UIObject::getTexture()
{
    return texture;
}

Vector2D& UIObject::getPosition()
{
    return position;
}

Vector2D& UIObject::getSize()
{
    return size;
}

void UIObject::freeTexture()
{
    texture.free();   
}
