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
            if( checkCollision2( box, tiles[ i ]->getBox() ) )
            {
                return true;
            }
        }
    }

    return false;
}

Player::Player()
{
    mBox.x = 0;
    mBox.y = 0;
	mBox.w = WIDTH;
	mBox.h = HEIGHT;

    mVelX = 0;
    mVelY = 0;

    for( int i = 0; i < TOTAL_PARTICLES; ++i )
    {
        particles[ i ] = new Particle( mBox.x, mBox.y );
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
            case SDLK_UP: mVelY -= VELOCITY; break;
            case SDLK_DOWN: mVelY += VELOCITY; break;
            case SDLK_LEFT: mVelX -= VELOCITY; break;
            case SDLK_RIGHT: mVelX += VELOCITY; break;
        }
    }
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += VELOCITY; break;
            case SDLK_DOWN: mVelY -= VELOCITY; break;
            case SDLK_LEFT: mVelX += VELOCITY; break;
            case SDLK_RIGHT: mVelX -= VELOCITY; break;
        }
    }
}

void Player::move( Tile *tiles[] )
{
    mBox.x += mVelX;

    // If the dot went too far to the left or right or touched a wall
    if( ( mBox.x < 0 ) || ( mBox.x + WIDTH > LEVEL_WIDTH ) || touchesWall( mBox, tiles ) )
    {
        // move back
        mBox.x -= mVelX;
    }

    mBox.y += mVelY;

    // If the dot went too far up or down or touched a wall
    if( ( mBox.y < 0 ) || ( mBox.y + HEIGHT > LEVEL_HEIGHT ) || touchesWall( mBox, tiles ) )
    {
        // move back
        mBox.y -= mVelY;
    }
}

void Player::setCamera( SDL_Rect& camera )
{
	camera.x = ( mBox.x + WIDTH / 2 ) - SCREEN_WIDTH / 2;
	camera.y = ( mBox.y + HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

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

void Player::render( SDL_Rect& camera, SDL_Rect* currentClip, SDL_Renderer* gRenderer )
{
	texture.render( mBox.x - camera.x, mBox.y - camera.y, NULL, 0.0, NULL, SDL_FLIP_NONE, gRenderer);// add for animation => , currentClip );
	renderParticles(gRenderer);
}

void Player::renderParticles(SDL_Renderer* gRenderer)
{
    for( int i = 0; i < TOTAL_PARTICLES; ++i )
    {
        if( particles[ i ]->isDead() )
        {
            delete particles[ i ];
            particles[ i ] = new Particle( mBox.x, mBox.y );
        }
    }

    for( int i = 0; i < TOTAL_PARTICLES; ++i )
    {
        particles[ i ]->render(gRenderer);
    }
}