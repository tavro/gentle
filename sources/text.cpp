#include "../headers/text.h"
#include <iostream>

Text::Text(std::string content, int x, int y) : UIObject(x, y, 0, 0)
{
    this->content = content;
    color = { 0, 0, 0, 0xFF }; // TODO: Hardcoded color
}

bool Text::loadFont(std::string path, int size)
{
    font = TTF_OpenFont( path.c_str(), size );
    return ( font == NULL );
}

bool Text::loadTexture(SDL_Renderer* renderer)
{
    return getTexture().loadFromRenderedText( content.c_str(), color, renderer, font );
}

void Text::updateContent(std::string content)
{
    this->content = content;
}

void Text::appendContent(std::string content)
{
    this->content += content;
}

std::string Text::getContent()
{
    return content;
}