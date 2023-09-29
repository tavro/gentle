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

		void handleEvent( SDL_Event* e ) override
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

        void setCurrentState( UIState state );

        SDL_Rect& getSpriteClip( int index );

        UIState getCurrentState();

        void render(SDL_Renderer* renderer) override
        {
            getTexture().render( getPosition().getX(), getPosition().getY(), &spriteClips[ currentState ], 0.0, NULL, SDL_FLIP_NONE, renderer );
        }

        bool isToggled();

        void setToggle(bool state);

	private:
        std::vector<SDL_Rect> spriteClips;
		UIState currentState;
        bool toggle = false;
};

#endif // UI_STATE_OBJECT_H