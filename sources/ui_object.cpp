#include "../headers/ui_object.h"

UIObject::UIObject( int x, int y, int w, int h )
{
    setPosition(x, y);
    setSize(w, h);
}

void UIObject::setSize( int w, int h )
{
    size.set(w, h);
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
