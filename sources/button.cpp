#include "../headers/button.h"

Button::Button(): UIStateObject(0, 0, BUTTON_WIDTH, BUTTON_HEIGHT)
{
    setPosition(0, 0);
    setCurrentState( UI_MOUSE_OUT );
    addSpriteClip( { 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT } );
    addSpriteClip( { 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT } );
    addSpriteClip( { 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT } );
    addSpriteClip( { 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT } );
}