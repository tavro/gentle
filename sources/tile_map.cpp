#include "../headers/tile_map.h"

#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/*
string generateRandomFileName() {
    const string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const int nameLength = 4;
    std::string randomName;

    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < nameLength; ++i) {
        int randomIndex = rand() % charset.length();
        randomName += charset[randomIndex];
    }

    return randomName;
}
*/

void TileMap::saveTilesToFile(std::string name)
{
    std::string fileName = name + ".map";

    std::ofstream outFile(fileName);

    if (!outFile) {
        std::cerr << "Error: Unable to open the file for writing." << std::endl;
    }

    for (int i = 0; i < TOTAL_TILES; ++i) {
        outFile << tiles[i]->getType() << std::endl;
    }

    outFile.close();

    std::cout << "Array content written to file: " << fileName << std::endl;
}

TileMap::TileMap()
{
    setTiles("./resources/empty.map");
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

			if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_SPRITES ) )
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

			if( x >= WIDTH )
			{
				x = 0;

				y += TILE_HEIGHT;
			}
		}
		
		if( tilesLoaded )
		{
			clips[ TILE_RED ].x = 0;
			clips[ TILE_RED ].y = 0;
			clips[ TILE_RED ].w = TILE_WIDTH;
			clips[ TILE_RED ].h = TILE_HEIGHT;

			clips[ TILE_GREEN ].x = 0;
			clips[ TILE_GREEN ].y = TILE_HEIGHT;
			clips[ TILE_GREEN ].w = TILE_WIDTH;
			clips[ TILE_GREEN ].h = TILE_HEIGHT;

			clips[ TILE_BLUE ].x = 0;
			clips[ TILE_BLUE ].y = TILE_HEIGHT * 2;
			clips[ TILE_BLUE ].w = TILE_WIDTH;
			clips[ TILE_BLUE ].h = TILE_HEIGHT;

			clips[ TILE_TOPLEFT ].x = TILE_WIDTH;
			clips[ TILE_TOPLEFT ].y = 0;
			clips[ TILE_TOPLEFT ].w = TILE_WIDTH;
			clips[ TILE_TOPLEFT ].h = TILE_HEIGHT;

			clips[ TILE_LEFT ].x = TILE_WIDTH;
			clips[ TILE_LEFT ].y = TILE_HEIGHT;
			clips[ TILE_LEFT ].w = TILE_WIDTH;
			clips[ TILE_LEFT ].h = TILE_HEIGHT;

			clips[ TILE_BOTTOMLEFT ].x = TILE_WIDTH;
			clips[ TILE_BOTTOMLEFT ].y = TILE_HEIGHT * 2;
			clips[ TILE_BOTTOMLEFT ].w = TILE_WIDTH;
			clips[ TILE_BOTTOMLEFT ].h = TILE_HEIGHT;

			clips[ TILE_TOP ].x = TILE_WIDTH * 2;
			clips[ TILE_TOP ].y = 0;
			clips[ TILE_TOP ].w = TILE_WIDTH;
			clips[ TILE_TOP ].h = TILE_HEIGHT;

			clips[ TILE_CENTER ].x = TILE_WIDTH * 2;
			clips[ TILE_CENTER ].y = TILE_HEIGHT;
			clips[ TILE_CENTER ].w = TILE_WIDTH;
			clips[ TILE_CENTER ].h = TILE_HEIGHT;

			clips[ TILE_BOTTOM ].x = TILE_WIDTH * 2;
			clips[ TILE_BOTTOM ].y = TILE_HEIGHT * 2;
			clips[ TILE_BOTTOM ].w = TILE_WIDTH;
			clips[ TILE_BOTTOM ].h = TILE_HEIGHT;

			clips[ TILE_TOPRIGHT ].x = TILE_WIDTH * 3;
			clips[ TILE_TOPRIGHT ].y = 0;
			clips[ TILE_TOPRIGHT ].w = TILE_WIDTH;
			clips[ TILE_TOPRIGHT ].h = TILE_HEIGHT;

			clips[ TILE_RIGHT ].x = TILE_WIDTH * 3;
			clips[ TILE_RIGHT ].y = TILE_HEIGHT;
			clips[ TILE_RIGHT ].w = TILE_WIDTH;
			clips[ TILE_RIGHT ].h = TILE_HEIGHT;

			clips[ TILE_BOTTOMRIGHT ].x = TILE_WIDTH * 3;
			clips[ TILE_BOTTOMRIGHT ].y = TILE_HEIGHT * 2;
			clips[ TILE_BOTTOMRIGHT ].w = TILE_WIDTH;
			clips[ TILE_BOTTOMRIGHT ].h = TILE_HEIGHT;
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
	return (tileY * (WIDTH / TILE_WIDTH) + tileX);
}