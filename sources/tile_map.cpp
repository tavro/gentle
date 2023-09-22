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
			clips[ (int)TileType::DIRT ].x = 0;
			clips[ (int)TileType::DIRT ].y = 0;
			clips[ (int)TileType::DIRT ].w = TILE_WIDTH;
			clips[ (int)TileType::DIRT ].h = TILE_HEIGHT;

			clips[ (int)TileType::GRASS ].x = 0;
			clips[ (int)TileType::GRASS ].y = TILE_HEIGHT;
			clips[ (int)TileType::GRASS ].w = TILE_WIDTH;
			clips[ (int)TileType::GRASS ].h = TILE_HEIGHT;

			clips[ (int)TileType::STONE ].x = 0;
			clips[ (int)TileType::STONE ].y = TILE_HEIGHT * 2;
			clips[ (int)TileType::STONE ].w = TILE_WIDTH;
			clips[ (int)TileType::STONE ].h = TILE_HEIGHT;

			clips[ (int)TileType::WALL_TOP_LEFT ].x = TILE_WIDTH;
			clips[ (int)TileType::WALL_TOP_LEFT ].y = 0;
			clips[ (int)TileType::WALL_TOP_LEFT ].w = TILE_WIDTH;
			clips[ (int)TileType::WALL_TOP_LEFT ].h = TILE_HEIGHT;

			clips[ (int)TileType::WALL_LEFT ].x = TILE_WIDTH;
			clips[ (int)TileType::WALL_LEFT ].y = TILE_HEIGHT;
			clips[ (int)TileType::WALL_LEFT ].w = TILE_WIDTH;
			clips[ (int)TileType::WALL_LEFT ].h = TILE_HEIGHT;

			clips[ (int)TileType::WALL_BOTTOM_LEFT ].x = TILE_WIDTH;
			clips[ (int)TileType::WALL_BOTTOM_LEFT ].y = TILE_HEIGHT * 2;
			clips[ (int)TileType::WALL_BOTTOM_LEFT ].w = TILE_WIDTH;
			clips[ (int)TileType::WALL_BOTTOM_LEFT ].h = TILE_HEIGHT;

			clips[ (int)TileType::WALL_TOP ].x = TILE_WIDTH * 2;
			clips[ (int)TileType::WALL_TOP ].y = 0;
			clips[ (int)TileType::WALL_TOP ].w = TILE_WIDTH;
			clips[ (int)TileType::WALL_TOP ].h = TILE_HEIGHT;

			clips[ (int)TileType::WALL_CENTER ].x = TILE_WIDTH * 2;
			clips[ (int)TileType::WALL_CENTER ].y = TILE_HEIGHT;
			clips[ (int)TileType::WALL_CENTER ].w = TILE_WIDTH;
			clips[ (int)TileType::WALL_CENTER ].h = TILE_HEIGHT;

			clips[ (int)TileType::WALL_BOTTOM ].x = TILE_WIDTH * 2;
			clips[ (int)TileType::WALL_BOTTOM ].y = TILE_HEIGHT * 2;
			clips[ (int)TileType::WALL_BOTTOM ].w = TILE_WIDTH;
			clips[ (int)TileType::WALL_BOTTOM ].h = TILE_HEIGHT;

			clips[ (int)TileType::WALL_TOP_RIGHT ].x = TILE_WIDTH * 3;
			clips[ (int)TileType::WALL_TOP_RIGHT ].y = 0;
			clips[ (int)TileType::WALL_TOP_RIGHT ].w = TILE_WIDTH;
			clips[ (int)TileType::WALL_TOP_RIGHT ].h = TILE_HEIGHT;

			clips[ (int)TileType::WALL_RIGHT ].x = TILE_WIDTH * 3;
			clips[ (int)TileType::WALL_RIGHT ].y = TILE_HEIGHT;
			clips[ (int)TileType::WALL_RIGHT ].w = TILE_WIDTH;
			clips[ (int)TileType::WALL_RIGHT ].h = TILE_HEIGHT;

			clips[ (int)TileType::WALL_BOTTOM_RIGHT ].x = TILE_WIDTH * 3;
			clips[ (int)TileType::WALL_BOTTOM_RIGHT ].y = TILE_HEIGHT * 2;
			clips[ (int)TileType::WALL_BOTTOM_RIGHT ].w = TILE_WIDTH;
			clips[ (int)TileType::WALL_BOTTOM_RIGHT ].h = TILE_HEIGHT;
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