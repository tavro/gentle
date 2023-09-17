#include "../headers/particle.h"

Particle::Particle( int x, int y )
{
    getPosition().setX( x - 5 + ( rand() % 25 ) );
    getPosition().setY( y - 5 + ( rand() % 25 ) );

    frame = rand() % 5;

    switch( rand() % 3 )
    {
        case 0: setTexture(redTexture); break; // might have to be reference
        case 1: setTexture(greenTexture); break;
        case 2: setTexture(blueTexture); break;
    }
}

void Particle::render(SDL_Renderer* gRenderer)
{
	getTexture().render( getPosition().getX(), getPosition().getY() );

    if( frame % 2 == 0 )
    {
		shimmerTexture.render( getPosition().getX(), getPosition().getY(), NULL, 0.0, NULL, SDL_FLIP_NONE, gRenderer );
    }

    frame++;
}

bool Particle::isDead()
{
    return frame > 10;
}