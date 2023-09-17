#ifndef TILEMAP_H
#define TILEMAP_H

#include <SDL2/SDL.h>
#include "./tile.h"

const int TOTAL_TILES = 300;
const int TOTAL_TILE_SPRITES = 12;

const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

const int WIDTH = 512 + 128;

class TileMap
{
    public:
        TileMap();

        bool setTiles(std::string mapPath);

		void render( SDL_Rect& camera, SDL_Renderer* gRenderer );

        void deleteTiles();

        Texture& getTexture();
        Tile** getTiles();

    private:
        Tile* tiles[ TOTAL_TILES ];
        SDL_Rect clips[ TOTAL_TILE_SPRITES ];
        Texture sheetTexture;
};

#endif // TILEMAP_H