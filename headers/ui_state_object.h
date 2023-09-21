#ifndef UI_STATE_OBJECT_H
#define UI_STATE_OBJECT_H

#include <SDL2/SDL.h>
#include <vector>
#include "./utils/vector2d.h"
#include "./ui_object.h"
#include "./texture.h"

enum UIState
{
    UI_MOUSE_OUT         = 0,
    UI_MOUSE_OVER_MOTION = 1,
    UI_MOUSE_DOWN        = 2,
    UI_MOUSE_UP          = 3,
    UI_TOTAL             = 4
}; 

class UIStateObject : public UIObject
{
	public:
        UIStateObject( int x, int y, int w, int h );

        void addSpriteClip( SDL_Rect clip );

		void handleEvent( SDL_Event* e );

        void setCurrentState( UIState state );

        SDL_Rect& getSpriteClip( int index );

        UIState getCurrentState();

        void render(SDL_Renderer* renderer) override
        {
            getTexture().render( getPosition().getX(), getPosition().getY(), &spriteClips[ currentState ], 0.0, NULL, SDL_FLIP_NONE, renderer );
        }

        bool isToggled();

	private:
        std::vector<SDL_Rect> spriteClips;
		UIState currentState;
        bool toggle;
};

#endif // UI_STATE_OBJECT_H