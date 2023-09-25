#ifndef TILE_H
#define TILE_H

#include <SDL2/SDL.h>
#include "./texture.h"
#include "./game_object.h"

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;

class Tile : public GameObject
{
    public:
		Tile( int x, int y, int tileType );
												// TODO: HARDCODED VALUE
		void render( SDL_Rect& camera, SDL_Rect (&gTileClips)[13], SDL_Renderer* gRenderer, Texture& gTileTexture );

		int getType();

    private:
		int type;
};

#endif // TILE_H
