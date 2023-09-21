#include "../headers/ui_state_object.h"
#include <iostream>

UIStateObject::UIStateObject( int x, int y, int w, int h ) : UIObject( x, y, w, h )
{
	currentState = UI_MOUSE_OUT;
}

void UIStateObject::handleEvent( SDL_Event* e )
{
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		int x, y;
		SDL_GetMouseState( &x, &y );

		bool inside = true;

		// Mouse is to the left
		if( x < getPosition().getX() )
		{
			inside = false;
		}
		// Mouse is to the right
		else if( x > getPosition().getX() + getSize().getX() )
		{
			inside = false;
		}
		// Mouse above
		else if( y < getPosition().getY() )
		{
			inside = false;
		}
		// Mouse below
		else if( y > getPosition().getY() + getSize().getY() )
		{
			inside = false;
		}

		// Mouse is outside
		if( !inside )
		{
			currentState = UI_MOUSE_OUT;
		}
		// Mouse is inside
		else
		{
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				currentState = UI_MOUSE_OVER_MOTION;
				break;
			
				case SDL_MOUSEBUTTONDOWN:
				currentState = UI_MOUSE_DOWN;
				toggle = !toggle;
				break;
				
				case SDL_MOUSEBUTTONUP:
				currentState = UI_MOUSE_UP;
				break;
			}
		}
	}
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