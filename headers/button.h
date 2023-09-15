#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include "./texture.h"

const int BUTTON_WIDTH = 96;
const int BUTTON_HEIGHT = 32;

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

		void render(SDL_Renderer* gRenderer);

        SDL_Rect spriteClips[ 4 ];          // These should be private, but I'm lazy.
        Texture buttonSpriteSheetTexture;

	private:
		SDL_Point position;

		ButtonSprite currentSprite;
};

#endif // BUTTON_H
