#include "../headers/button.h"

Button::Button(std::string content, int x, int y): UIStateObject(x, y, BUTTON_WIDTH, BUTTON_HEIGHT)
{
    setText(content);

    int fontSize = 14;
    int contentLength = (content.length()-1) * fontSize;

    text.setPosition(x + BUTTON_WIDTH / 2 - contentLength / 2, y);

    setPosition(x, y);
    setCurrentState( UI_MOUSE_OUT );
    addSpriteClip( { 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT } );
    addSpriteClip( { 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT } );
    addSpriteClip( { 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT } );
    addSpriteClip( { 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT } );
}

Text& Button::getText()
{
    return text;
}
        
void Button::setText(std::string content)
{
    text.updateContent(content);
}
        
void Button::loadTextTexture(SDL_Renderer* renderer)
{
    text.loadTexture(renderer);
}

void Button::loadSpriteSheet(std::string path, SDL_Renderer* renderer)
{
    getTexture().loadFromFile( path, renderer );
    for( int i = 0; i < 4; ++i )
    {
        getSpriteClip( i ).x = 0;
        getSpriteClip( i ).y = i * BUTTON_HEIGHT;
        getSpriteClip( i ).w = BUTTON_WIDTH;
        getSpriteClip( i ).h = BUTTON_HEIGHT;
    }
}