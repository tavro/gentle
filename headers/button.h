#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>

enum ButtonSprite 
{
	BS_MOUSE_OUT         = 0,
	BS_MOUSE_OVER_MOTION = 1,
	BS_MOUSE_DOWN        = 2,
	BS_MOUSE_UP          = 3,
	BS_TOTAL             = 4
};

class Button
{
	public:
		Button();

		void setPosition( int x, int y );

		void handleEvent( SDL_Event* e );

		void render();

	private:
		SDL_Point position;

		ButtonSprite currentSprite;
};

#endif // BUTTON_H
