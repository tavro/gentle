#ifndef UI_OBJECT_H
#define UI_OBJECT_H

#include <SDL2/SDL.h>
#include <vector>
#include "./utils/vector2d.h"
#include "./texture.h"

class UIObject
{
	public:
		UIObject( int x, int y, int w, int h );

		void setPosition( int x, int y );

        void setSize( int w, int h );

        virtual void handleEvent(SDL_Event* e)
        {

        }

        virtual void render(SDL_Renderer* renderer)
        {
            texture.render( position.getX(), position.getY(), NULL, 0.0, NULL, SDL_FLIP_NONE, renderer );
        }

        Vector2D& getPosition();
        Vector2D& getSize();

        Texture& getTexture();

        void freeTexture();

	private:
        Texture texture;

		Vector2D position;
		Vector2D size;
};

#endif // UI_OBJECT_H