#include "../headers/button.h"

Button::Button()
{
	position.x = 0;
	position.y = 0;

	currentSprite = BS_MOUSE_OUT;
}

void Button::setPosition( int x, int y )
{
	position.x = x;
	position.y = y;
}

void Button::handleEvent( SDL_Event* e )
{
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		int x, y;
		SDL_GetMouseState( &x, &y );

		bool inside = true;

		// Mouse is left of the button
		if( x < position.x )
		{
			inside = false;
		}
		// Mouse is right of the button
		else if( x > position.x + BUTTON_WIDTH )
		{
			inside = false;
		}
		// Mouse above the button
		else if( y < position.y )
		{
			inside = false;
		}
		// Mouse below the button
		else if( y > position.y + BUTTON_HEIGHT )
		{
			inside = false;
		}

		// Mouse is outside button
		if( !inside )
		{
			currentSprite = BS_MOUSE_OUT;
		}
		// Mouse is inside button
		else
		{
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				currentSprite = BS_MOUSE_OVER_MOTION;
				break;
			
				case SDL_MOUSEBUTTONDOWN:
				currentSprite = BS_MOUSE_DOWN;
				break;
				
				case SDL_MOUSEBUTTONUP:
				currentSprite = BS_MOUSE_UP;
				break;
			}
		}
	}
}
	
void Button::render(SDL_Renderer* gRenderer)
{
	buttonSpriteSheetTexture.render( position.x, position.y, &spriteClips[ currentSprite ], 0.0, NULL, SDL_FLIP_NONE, gRenderer );
}