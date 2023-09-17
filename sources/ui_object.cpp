#include "../headers/ui_object.h"

UIObject::UIObject( int x, int y, int w, int h )
{
    setPosition(x, y);
    size.setX(w);
    size.setY(h);
	currentState = UI_MOUSE_OUT;
}

void UIObject::setPosition( int x, int y )
{
    position.setX(x);
    position.setY(y);
}

void UIObject::handleEvent( SDL_Event* e )
{
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		int x, y;
		SDL_GetMouseState( &x, &y );

		bool inside = true;

		// Mouse is to the left
		if( x < position.getX() )
		{
			inside = false;
		}
		// Mouse is to the right
		else if( x > position.getX() + size.getX() )
		{
			inside = false;
		}
		// Mouse above
		else if( y < position.getY() )
		{
			inside = false;
		}
		// Mouse below
		else if( y > position.getY() + size.getY() )
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
				break;
				
				case SDL_MOUSEBUTTONUP:
				currentState = UI_MOUSE_UP;
				break;
			}
		}
	}
}
	
void UIObject::render(SDL_Renderer* renderer)
{
	texture.render( position.getX(), position.getY(), &spriteClips[ currentState ], 0.0, NULL, SDL_FLIP_NONE, renderer );
}

void UIObject::addSpriteClip( SDL_Rect clip )
{
    spriteClips.push_back(clip);
}

void UIObject::setCurrentState( UIState state )
{
    currentState = state;
}

Texture& UIObject::getTexture()
{
    return texture;
}

SDL_Rect& UIObject::getSpriteClip( int index )
{
    return spriteClips[ index ];
}