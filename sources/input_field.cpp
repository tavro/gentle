#include "../headers/input_field.h"
#include <iostream>

InputField::InputField( int x, int y, int w, int h, int max ) : UIStateObject( x, y, w, h )
{
    maxCharacters = max;
    text.setPosition(x, y);
    textStr = "Sample Text";
    addSpriteClip( { 0, 0, 96, 32 } );  // TODO: Hardcoded values, TODO: Update state object with automatic sizing
    addSpriteClip( { 0, 0, 96, 32 } );
    addSpriteClip( { 0, 0, 96, 32 } );
    addSpriteClip( { 0, 0, 96, 32 } );
}

void InputField::setText(std::string content)
{
    text.updateContent(content);
}

void InputField::loadTextTexture(SDL_Renderer* renderer)
{
    text.loadTexture(renderer);
}

void InputField::appendToText(std::string content)
{
    if(isToggled()) {
        if( !( content[ 0 ] == 'c' || content[ 0 ] == 'C' || content[ 0 ] == 'v' || content[ 0 ] == 'V' ) )
        {
            if( content != "" )
            {
                textStr += content;
                std::string tmp = textStr;
                if(textStr.length() > maxCharacters) {
                    tmp = textStr.substr(textStr.length() - maxCharacters, textStr.length());
                }
                text.updateContent(tmp);
                hasUpdate = true;
            }
        }
    }
}

void InputField::updateText(std::string content)
{
    if( content != "" )
    {
        textStr = content;
        std::string tmp = textStr;
        if(textStr.length() > maxCharacters) {
            tmp = textStr.substr(textStr.length() - maxCharacters, textStr.length());
        }
        text.updateContent(tmp);
        hasUpdate = true;
    }
}

void InputField::removeChar()
{
    if(textStr.length() > 0)
    {
	    textStr.pop_back();
        std::string tmp = textStr;
        if(textStr.length() > maxCharacters) {
            tmp = textStr.substr(textStr.length() - maxCharacters, textStr.length());
        }
        text.updateContent(tmp);
        hasUpdate = true;
    }
}

Text& InputField::getText()
{
    return text;
}

std::string InputField::getContent()
{
    return textStr;
}