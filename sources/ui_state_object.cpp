#include "../headers/ui_state_object.h"
#include <iostream>

UIStateObject::UIStateObject( int x, int y, int w, int h ) : UIObject( x, y, w, h )
{
	currentState = UI_MOUSE_OUT;
}

void UIStateObject::addSpriteClip( SDL_Rect clip )
{
    spriteClips.push_back(clip);
}

void UIStateObject::setCurrentState( UIState state )
{
    currentState = state;
}

SDL_Rect& UIStateObject::getSpriteClip( int index )
{
    return spriteClips[ index ];
}

UIState UIStateObject::getCurrentState()
{
	return currentState;
}

bool UIStateObject::isToggled()
{
	return toggle;
}

void UIStateObject::setToggle(bool state)
{
	toggle = state;
}