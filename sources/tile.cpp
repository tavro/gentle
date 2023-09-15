#include "../headers/tile.h"
#include <iostream>

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    return true;
}

Tile::Tile( int x, int y, int tileType )
{
    box.x = x;
    box.y = y;

    box.w = TILE_WIDTH;
    box.h = TILE_HEIGHT;

    type = tileType;
}

void Tile::render( SDL_Rect& camera, SDL_Rect (&gTileClips)[12], SDL_Renderer* gRenderer, Texture& gTileTexture )
{
    if( checkCollision( camera, box ) )
    {
        gTileTexture.render( box.x - camera.x, box.y - camera.y, &gTileClips[ type ], 0.0, NULL, SDL_FLIP_NONE, gRenderer );
    }
}

int Tile::getType()
{
    return type;
}

SDL_Rect Tile::getBox()
{
    return box;
}