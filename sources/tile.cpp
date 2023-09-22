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
    setPosition(Vector2D(x, y));
    setSize(Vector2D(TILE_WIDTH, TILE_HEIGHT));

    type = tileType;
}
                                                // HARDCODED VALUE
void Tile::render( SDL_Rect& camera, SDL_Rect (&gTileClips)[13], SDL_Renderer* gRenderer, Texture& gTileTexture )
{
    if( checkCollision( camera, toBox() ) )
    {
        gTileTexture.render( getPosition().getX() - camera.x, getPosition().getY() - camera.y, &gTileClips[ type ], 0.0, NULL, SDL_FLIP_NONE, gRenderer );
    }
}

int Tile::getType()
{
    return type;
}