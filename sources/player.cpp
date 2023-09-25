#include "../headers/player.h"
#include "../headers/tile_map.h"

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

bool isSolid(Tile* t)
{
    switch(t->getType())
    {
        case (int)TileType::WALL_BOTTOM:
        case (int)TileType::WALL_BOTTOM_LEFT:
        case (int)TileType::WALL_BOTTOM_RIGHT:
        case (int)TileType::WALL_TOP:
        case (int)TileType::WALL_TOP_LEFT:
        case (int)TileType::WALL_TOP_RIGHT:
        case (int)TileType::WALL_CENTER:
        case (int)TileType::WALL_LEFT:
        case (int)TileType::WALL_RIGHT:
        case (int)TileType::GOLD:
            return true;
    }
    return false;
}

bool touchesWall( SDL_Rect box, Tile* tiles[] )
{
    for( int i = 0; i < 300; ++i )  // TODO: Hardcoded value
    {
        if( isSolid(tiles[ i ]) )
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

void Player::addAnimation(std::string path, SDL_Renderer* renderer)
{
    Animation* anim = new Animation{};
    anim->loadTexture( renderer, path );
    activeAnimation = anim;
    animations.push_back(anim);
}

void Player::handleEvent( SDL_Event& e )
{
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: 
                getVelocity().decreaseY(VELOCITY);
                activeAnimation = animations[2]; 
                playAnimation = true;
            break;
            case SDLK_DOWN: 
                getVelocity().increaseY(VELOCITY); 
                activeAnimation = animations[1]; 
                playAnimation = true;
            break;
            case SDLK_LEFT: 
                getVelocity().decreaseX(VELOCITY); 
                activeAnimation = animations[3]; 
                playAnimation = true;
            break;
            case SDLK_RIGHT: 
                getVelocity().increaseX(VELOCITY); 
                activeAnimation = animations[0]; 
                playAnimation = true;
            break;
        }
    }
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: 
                getVelocity().increaseY(VELOCITY); 
                playAnimation = false;
            break;
            case SDLK_DOWN: 
                getVelocity().decreaseY(VELOCITY); 
                playAnimation = false;
            break;
            case SDLK_LEFT: 
                getVelocity().increaseX(VELOCITY); 
                playAnimation = false;
            break;
            case SDLK_RIGHT: 
                getVelocity().decreaseX(VELOCITY); 
                playAnimation = false;
            break;
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
    if(playAnimation)
    {
        activeAnimation->render({getPosition().getX() - camera.x, getPosition().getY() - camera.y}, gRenderer);
    }
    else
    {
	    getTexture().render( getPosition().getX() - camera.x, getPosition().getY() - camera.y, NULL, 0.0, NULL, SDL_FLIP_NONE, gRenderer);
    }
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

Animation* Player::getAnimation()
{
    return activeAnimation;
}