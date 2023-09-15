#include "../headers/particle.h"

Particle::Particle( int x, int y )
{
    posX = x - 5 + ( rand() % 25 );
    posY = y - 5 + ( rand() % 25 );

    frame = rand() % 5;

    switch( rand() % 3 )
    {
        case 0: texture = &redTexture; break;
        case 1: texture = &greenTexture; break;
        case 2: texture = &blueTexture; break;
    }
}

void Particle::render(SDL_Renderer* gRenderer)
{
	texture->render( posX, posY );

    if( frame % 2 == 0 )
    {
		shimmerTexture.render( posX, posY, NULL, 0.0, NULL, SDL_FLIP_NONE, gRenderer );
    }

    frame++;
}

bool Particle::isDead()
{
    return frame > 10;
}