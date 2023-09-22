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

enum class TileType {
    DIRT,
    GRASS,
    STONE,
    WALL_CENTER,
    WALL_TOP,
    WALL_TOP_RIGHT,
    WALL_RIGHT,
    WALL_BOTTOM_RIGHT,
    WALL_BOTTOM,
    WALL_BOTTOM_LEFT,
    WALL_LEFT,
    WALL_TOP_LEFT,
    AMOUNT
};

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