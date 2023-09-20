#include "../headers/input_field.h"
#include <iostream>

InputField::InputField( int x, int y, int w, int h ) : UIStateObject( x, y, w, h )
{
    text.setPosition(x, y);
}

void InputField::setText(std::string content)
{
    text.updateContent(content);
}

void InputField::loadTextTexture(SDL_Renderer* renderer)
{
    text.loadTexture(renderer);
}

void InputField::updateText(std::string content)
{
    if( !( content[ 0 ] == 'c' || content[ 0 ] == 'C' || content[ 0 ] == 'v' || content[ 0 ] == 'V' ) )
    {
        if( content != "" )
        {
            text.appendContent(content);
            hasUpdate = true;
        }
    }
}

Text& InputField::getText()
{
    return text;
}