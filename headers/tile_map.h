#ifndef TILEMAP_H
#define TILEMAP_H

#include <SDL2/SDL.h>
#include "./tile.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

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
        void saveTilesToFile(std::string name);

		void render( SDL_Rect& camera, SDL_Renderer* renderer );

        bool loadTexture( SDL_Renderer* renderer, std::string path );

        void deleteTiles();

        Texture& getTexture();
        Tile** getTiles();

        void setTile(int index, Tile* t);
        int getTileFromScreenPosition(int x, int y);

    private:
        Tile* tiles[ TOTAL_TILES ];
        SDL_Rect clips[ TOTAL_TILE_SPRITES ];
        Texture sheetTexture;
};

#endif // TILEMAP_H