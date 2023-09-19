#include "../headers/player.h"

bool checkCollision2( SDL_Rect a, SDL_Rect b )
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

bool touchesWall( SDL_Rect box, Tile* tiles[] )
{
    for( int i = 0; i < 300; ++i )
    {
                                    // TILE_CENTER                       // TILE_TOPLEFT
        if( ( tiles[ i ]->getType() >= 3 ) && ( tiles[ i ]->getType() <= 11 ) )
        {
            if( checkCollision2( box, tiles[ i ]->toBox() ) )
            {
                return true;
            }
        }
    }

    return false;
}

Player::Player()
{
    setPosition(Vector2D(0, 0));
    setSize(Vector2D(WIDTH, HEIGHT));
    setVelocity(Vector2D(0, 0));

    for( int i = 0; i < TOTAL_PARTICLES; ++i )
    {
        particles[ i ] = new Particle( getPosition().getX(), getPosition().getY() );
    }
}


Player::~Player()
{
    for( int i = 0; i < TOTAL_PARTICLES; ++i )
    {
        delete particles[ i ];
    }
}

void Player::handleEvent( SDL_Event& e )
{
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: getVelocity().decreaseY(VELOCITY); break;
            case SDLK_DOWN: getVelocity().increaseY(VELOCITY); break;
            case SDLK_LEFT: getVelocity().decreaseX(VELOCITY); break;
            case SDLK_RIGHT: getVelocity().increaseX(VELOCITY); break;
        }
    }
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: getVelocity().increaseY(VELOCITY); break;
            case SDLK_DOWN: getVelocity().decreaseY(VELOCITY); break;
            case SDLK_LEFT: getVelocity().increaseX(VELOCITY); break;
            case SDLK_RIGHT: getVelocity().decreaseX(VELOCITY); break;
        }
    }
}

void Player::move( Tile *tiles[] )
{
    getPosition().increaseX(getVelocity().getX());

    // If the player went too far to the left or right or touched a wall
    if( ( getPosition().getX() < 0 ) || ( getPosition().getX() + WIDTH > LEVEL_WIDTH ) || touchesWall( toBox(), tiles ) )
    {
        // move back
        getPosition().decreaseX(getVelocity().getX());
    }

    getPosition().increaseY(getVelocity().getY());

    // If the player went too far up or down or touched a wall
    if( ( getPosition().getY() < 0 ) || ( getPosition().getY() + HEIGHT > LEVEL_HEIGHT ) || touchesWall( toBox(), tiles ) )
    {
        // move back
        getPosition().decreaseY(getVelocity().getY());
    }
}

void Player::setCamera( SDL_Rect& camera )
{
	camera.x = ( getPosition().getX() + WIDTH / 2 ) - SCREEN_WIDTH / 2;
	camera.y = ( getPosition().getY() + HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

	if( camera.x < 0 )
	{ 
		camera.x = 0;
	}
	if( camera.y < 0 )
	{
		camera.y = 0;
	}
	if( camera.x > LEVEL_WIDTH - camera.w )
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if( camera.y > LEVEL_HEIGHT - camera.h )
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void Player::render( SDL_Rect& camera, SDL_Renderer* gRenderer )
{
    animation.render({getPosition().getX() - camera.x, getPosition().getY() - camera.y}, gRenderer);
	//getTexture().render( getPosition().getX() - camera.x, getPosition().getY() - camera.y, NULL, 0.0, NULL, SDL_FLIP_NONE, gRenderer);
	renderParticles(gRenderer);
}

void Player::renderParticles(SDL_Renderer* gRenderer)
{
    for( int i = 0; i < TOTAL_PARTICLES; ++i )
    {
        if( particles[ i ]->isDead() )
        {
            delete particles[ i ];
            particles[ i ] = new Particle( getPosition().getX(), getPosition().getY() );
        }
    }

    for( int i = 0; i < TOTAL_PARTICLES; ++i )
    {
        particles[ i ]->render(gRenderer);
    }
}

Animation& Player::getAnimation()
{
    return animation;
}