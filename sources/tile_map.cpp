#include "../headers/tile_map.h"

#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

void TileMap::saveTilesToFile(std::string name)
{
    std::string fileName = name + ".map";

    std::ofstream outFile(fileName);

    if (!outFile) {
        std::cerr << "Error: Unable to open the file for writing." << std::endl;
    }

	const int MAX_X = 20;
	int x = 0;
    for (int i = 0; i < TOTAL_TILES; ++i) {
		std::stringstream ss;
		ss << tiles[i]->getType();
		if(ss.str().length() == 1) // num is one digit
		{
        	outFile << "0" + ss.str();
		}
		else
		{
        	outFile << tiles[i]->getType();
		}
		x++;
		if(x >= MAX_X)
		{
			x = 0;
			outFile << std::endl;
		}
		else
		{
			outFile << " ";
		}
    }

    outFile.close();

    std::cout << "Array content written to file: " << fileName << std::endl;
}

TileMap::TileMap()
{
    setTiles("./resources/dkg2.map");
}

bool TileMap::setTiles(std::string mapPath)
{
	bool tilesLoaded = true;

    int x = 0, y = 0;

    std::ifstream map( mapPath );

    if( map.fail() )
    {
		printf( "Unable to load map file!\n" );
		tilesLoaded = false;
    }
	else
	{
		for( int i = 0; i < TOTAL_TILES; ++i )
		{
			int tileType = -1;

			map >> tileType;

			if( map.fail() )
			{
				printf( "Error loading map: Unexpected end of file!\n" );
				tilesLoaded = false;
				break;
			}

			if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_TYPES ) )
			{
				tiles[ i ] = new Tile( x, y, tileType );
			}
			else
			{
				printf( "Error loading map: Invalid tile type at %d!\n", i );
				tilesLoaded = false;
				break;
			}

			x += TILE_WIDTH;

			if( x >= TOTAL_PIXEL_WIDTH )
			{
				x = 0;

				y += TILE_HEIGHT;
			}
		}
		
		if( tilesLoaded )
		{
			for (int i = 0; i < TOTAL_TILE_TYPES; ++i)
			{
				clips[i].x = (i % (128 / TILE_WIDTH)) * TILE_WIDTH;
				clips[i].y = (i / (128 / TILE_WIDTH)) * TILE_HEIGHT;
				clips[i].w = TILE_WIDTH;
				clips[i].h = TILE_HEIGHT;
			}
		}
	}

    map.close();

    return tilesLoaded;
}

void TileMap::render( SDL_Rect& camera, SDL_Renderer* renderer ) {
    for( int i = 0; i < TOTAL_TILES; ++i )
	{
		tiles[ i ]->render( camera, clips, renderer, sheetTexture);
	}
}

Texture& TileMap::getTexture()
{
    return sheetTexture;
}

Tile** TileMap::getTiles()
{
    return tiles;
}

void TileMap::deleteTiles()
{
    for( int i = 0; i < TOTAL_TILES; ++i )
    {
        if( tiles[ i ] != NULL )
        {
            delete tiles[ i ];
            tiles[ i ] = NULL;
        }
    }
}

bool TileMap::loadTexture( SDL_Renderer* renderer, std::string path )
{
    return sheetTexture.loadFromFile( path, renderer );
}

void TileMap::setTile(int index, Tile* t)
{
	tiles[index] = t;
}

int TileMap::getTileFromScreenPosition(int x, int y)
{
	int tileX = (x / TILE_WIDTH);
	int tileY = (y / TILE_HEIGHT);
	return (tileY * (TOTAL_PIXEL_WIDTH / TILE_WIDTH) + tileX);
}