#ifndef UI_OBJECT_H
#define UI_OBJECT_H

#include <SDL2/SDL.h>
#include <vector>
#include "./utils/vector2d.h"
#include "./texture.h"

enum UIState
{
    UI_MOUSE_OUT         = 0,
    UI_MOUSE_OVER_MOTION = 1,
    UI_MOUSE_DOWN        = 2,
    UI_MOUSE_UP          = 3,
    UI_TOTAL             = 4
}; 

class UIObject
{
	public:
		UIObject( int x, int y, int w, int h );

		void setPosition( int x, int y );

		void handleEvent( SDL_Event* e );

		void render( SDL_Renderer* renderer );

        void addSpriteClip( SDL_Rect clip );

        void setCurrentState( UIState state );

        Texture& getTexture();

        SDL_Rect& getSpriteClip( int index );

	private:
        std::vector<SDL_Rect> spriteClips;
        Texture texture;

        TTF_Font* font;
        Texture textTexture;

		Vector2D position;
		Vector2D size;

		UIState currentState;
};

#endif // UI_OBJECT_H