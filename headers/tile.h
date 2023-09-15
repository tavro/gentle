#ifndef TILE_H
#define TILE_H

#include <SDL2/SDL.h>
#include "./texture.h"

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;

class Tile
{
    public:
		Tile( int x, int y, int tileType );

		void render( SDL_Rect& camera, SDL_Rect (&gTileClips)[12], SDL_Renderer* gRenderer, Texture& gTileTexture );

		int getType();

		SDL_Rect getBox();

        Texture tileSheetTexture;

    private:
		SDL_Rect box;

		int type;
};

#endif // TILE_H
