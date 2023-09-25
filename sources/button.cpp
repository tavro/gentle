#include "../headers/button.h"

Button::Button(std::string content, int x, int y, int w, int h): UIStateObject(x, y, w, h)
{
    setText(content);
    setPosition(x, y);
    setCurrentState( UI_MOUSE_OUT );

    // Initialize left, middle, right sprite clips
    for( int i = 0; i < 12; i++ ) // TODO: Create constant for sprite clip amount
    {
        addSpriteClip( { 0, 0, 0, 0 } );
    }
}

Button::Button(std::string content, int x, int y): UIStateObject(x, y, 0, 0)
{
    setText(content);

    int fontSize = 14;  // TODO: Hardcoded value, fetch from text class
    int contentLength = (content.length()-1) * fontSize;

    setSize(contentLength+MINIMUM_WIDTH, BH);

    text.setPosition(x + getSize().getX() / 2 - contentLength / 2, y);

    setPosition(x, y);
    setCurrentState( UI_MOUSE_OUT );

    // Initialize left, middle, right sprite clips
    for( int i = 0; i < 12; i++ ) // TODO
    {
        addSpriteClip( { 0, 0, 0, 0 } );
    }
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
    spriteSheet2.loadFromFile( "./resources/buttonsheet-middle.png", renderer );

    // Update size and position for sprite clips
    for( int j = 0; j < 3; j++ )    // TODO: Hardcoded value
    {
        for( int i = 0; i < 4; ++i )    // TODO: Harcoded values
        {
            getSpriteClip( i + j*4).x = BH*j;
            getSpriteClip( i + j*4).y = i * getSize().getY();
            getSpriteClip( i + j*4).w = getSize().getY();
            getSpriteClip( i + j*4).h = getSize().getY();
        }
    }

}