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

enum class TileType {
    DIRT,
    WALL_TOP_LEFT,
    WALL_TOP,
    WALL_TOP_RIGHT,
    GRASS,
    WALL_LEFT,
    WALL_CENTER,
    WALL_RIGHT,
    STONE,
    WALL_BOTTOM_LEFT,
    WALL_BOTTOM,
    WALL_BOTTOM_RIGHT,
    GOLD,
    AMOUNT
};

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
	    static const int TOTAL_TILE_TYPES   = (int)TileType::AMOUNT;
        static const int TOTAL_TILES        = 20 /* WIDTH OF MAP */ * 15 /* HEIGHT OF MAP */;
        static const int TOTAL_PIXEL_WIDTH  = 20 * TILE_WIDTH; 
        
        Tile* tiles[ TOTAL_TILES ];
        SDL_Rect clips[ TOTAL_TILE_TYPES ];
        Texture sheetTexture;
};

#endif // TILEMAP_H