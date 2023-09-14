#ifndef TILE_H
#define TILE_H

#include <SDL2/SDL.h>

class Tile
{
    public:
		Tile( int x, int y, int tileType );

		void render( SDL_Rect& camera );

		int getType();

		SDL_Rect getBox();

    private:
		SDL_Rect box;

		int type;
};

#endif // TILE_H
