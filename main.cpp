#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>

#include "./headers/texture.h"
#include "./headers/particle.h"
#include "./headers/player.h"
#include "./headers/timer.h"
#include "./headers/tile.h"
#include "./headers/button.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int LEVEL_WIDTH = 512 + 128;
const int LEVEL_HEIGHT = 384 + 96;

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int TOTAL_TILES = 192 + 48 + 60;
const int TOTAL_TILE_SPRITES = 12;

const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

const int BUTTON_WIDTH = 96;
const int BUTTON_HEIGHT = 32;
const int TOTAL_BUTTONS = 4;

const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

bool init();

bool loadMedia( Tile* tiles[] );

void close( Tile* tiles[] );

bool checkCollision( SDL_Rect a, SDL_Rect b );
bool touchesWall( SDL_Rect box, Tile* tiles[] );

bool setTiles( Tile *tiles[] );

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

Texture gPromptTextTexture;
Texture gInputTextTexture;
Texture gTileTexture;
Texture gFPSTextTexture;
Texture gRedTexture;
Texture gGreenTexture;
Texture gBlueTexture;
Texture gShimmerTexture;

SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];

const int DOT_ANIMATION_FRAMES = 4;
SDL_Rect gDotSpriteClips[ DOT_ANIMATION_FRAMES ];
Texture gDotTexture;

SDL_Rect gSpriteClips[ 4 ];
Texture gButtonSpriteSheetTexture;

Button gButtons[ TOTAL_BUTTONS ]; 

Mix_Music *gMusic = NULL;

Mix_Chunk *gScratch = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gLow = NULL;

Texture::Texture()
{
	texture = NULL;
	width = 0;
	height = 0;
}

Texture::~Texture()
{
	free();
}

bool Texture::loadFromFile( std::string path )
{
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			width = loadedSurface->w;
			height = loadedSurface->h;
		}

		SDL_FreeSurface( loadedSurface );
	}

	texture = newTexture;
	return texture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool Texture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	free();

	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
        texture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( texture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			width = textSurface->w;
			height = textSurface->h;
		}

		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	return texture != NULL;
}
#endif

void Texture::free()
{
	if( texture != NULL )
	{
		SDL_DestroyTexture( texture );
		texture = NULL;
		width = 0;
		height = 0;
	}
}

void Texture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	SDL_SetTextureColorMod( texture, red, green, blue );
}

void Texture::setBlendMode( SDL_BlendMode blending )
{
	SDL_SetTextureBlendMode( texture, blending );
}
		
void Texture::setAlpha( Uint8 alpha )
{
	SDL_SetTextureAlphaMod( texture, alpha );
}

void Texture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	SDL_Rect renderQuad = { x, y, width, height };

	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx( gRenderer, texture, clip, &renderQuad, angle, center, flip );
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}


Timer::Timer()
{
    startTicks = 0;
    pausedTicks = 0;

    paused = false;
    started = false;
}

void Timer::start()
{
    started = true;
    paused = false;

    startTicks = SDL_GetTicks();
	pausedTicks = 0;
}

void Timer::stop()
{
    started = false;
    paused = false;

	startTicks = 0;
	pausedTicks = 0;
}

void Timer::pause()
{
    if( started && !paused )
    {
        paused = true;

        pausedTicks = SDL_GetTicks() - startTicks;
		startTicks = 0;
    }
}

void Timer::unpause()
{
    if( started && paused )
    {
        paused = false;

        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

Uint32 Timer::getTicks()
{
	Uint32 time = 0;

    if( started )
    {
        if( paused )
        {
            time = pausedTicks;
        }
        else
        {
            time = SDL_GetTicks() - startTicks;
        }
    }

    return time;
}

bool Timer::isStarted()
{
    return started;
}

bool Timer::isPaused()
{
    return paused && started;
}

Button::Button()
{
	position.x = 0;
	position.y = 0;

	currentSprite = BS_MOUSE_OUT;
}

void Button::setPosition( int x, int y )
{
	position.x = x;
	position.y = y;
}

void Button::handleEvent( SDL_Event* e )
{
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		int x, y;
		SDL_GetMouseState( &x, &y );

		bool inside = true;

		// Mouse is left of the button
		if( x < position.x )
		{
			inside = false;
		}
		// Mouse is right of the button
		else if( x > position.x + BUTTON_WIDTH )
		{
			inside = false;
		}
		// Mouse above the button
		else if( y < position.y )
		{
			inside = false;
		}
		// Mouse below the button
		else if( y > position.y + BUTTON_HEIGHT )
		{
			inside = false;
		}

		// Mouse is outside button
		if( !inside )
		{
			currentSprite = BS_MOUSE_OUT;
		}
		// Mouse is inside button
		else
		{
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				currentSprite = BS_MOUSE_OVER_MOTION;
				break;
			
				case SDL_MOUSEBUTTONDOWN:
				currentSprite = BS_MOUSE_DOWN;
				break;
				
				case SDL_MOUSEBUTTONUP:
				currentSprite = BS_MOUSE_UP;
				break;
			}
		}
	}
}
	
void Button::render()
{
	gButtonSpriteSheetTexture.render( position.x, position.y, &gSpriteClips[ currentSprite ] );
}

Tile::Tile( int x, int y, int tileType )
{
    box.x = x;
    box.y = y;

    box.w = TILE_WIDTH;
    box.h = TILE_HEIGHT;

    type = tileType;
}

void Tile::render( SDL_Rect& camera )
{
    if( checkCollision( camera, box ) )
    {
        gTileTexture.render( box.x - camera.x, box.y - camera.y, &gTileClips[ type ] );
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

Particle::Particle( int x, int y )
{
    posX = x - 5 + ( rand() % 25 );
    posY = y - 5 + ( rand() % 25 );

    frame = rand() % 5;

    switch( rand() % 3 )
    {
        case 0: texture = &gRedTexture; break;
        case 1: texture = &gGreenTexture; break;
        case 2: texture = &gBlueTexture; break;
    }
}

void Particle::render()
{
	texture->render( posX, posY );

    if( frame % 2 == 0 )
    {
		gShimmerTexture.render( posX, posY );
    }

    frame++;
}

bool Particle::isDead()
{
    return frame > 10;
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

void Player::render( SDL_Rect& camera, SDL_Rect* currentClip )
{
	gDotTexture.render( mBox.x - camera.x, mBox.y - camera.y);// add for animation => , currentClip );
	// add for particles => renderParticles();
}

void Player::renderParticles()
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
        particles[ i ]->render();
    }
}

bool init()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		gWindow = SDL_CreateWindow( "SDL Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
                
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}

			if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
			{
				printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
				success = false;
			}
		}
	}

	return success;
}

bool loadMedia( Tile* tiles[] )
{
	bool success = true;

	if( !gDotTexture.loadFromFile( "./resources/dotanim.png" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}
    else
	{
		gDotSpriteClips[ 0 ].x = 0;
		gDotSpriteClips[ 0 ].y = 0;
		gDotSpriteClips[ 0 ].w = 20;
		gDotSpriteClips[ 0 ].h = 20;

		gDotSpriteClips[ 1 ].x = 20;
		gDotSpriteClips[ 1 ].y = 0;
		gDotSpriteClips[ 1 ].w = 20;
		gDotSpriteClips[ 1 ].h = 20;
		
		gDotSpriteClips[ 2 ].x = 40;
		gDotSpriteClips[ 2 ].y = 0;
		gDotSpriteClips[ 2 ].w = 20;
		gDotSpriteClips[ 2 ].h = 20;

		gDotSpriteClips[ 3 ].x = 60;
		gDotSpriteClips[ 3 ].y = 0;
		gDotSpriteClips[ 3 ].w = 20;
		gDotSpriteClips[ 3 ].h = 20;
	}

	if( !gTileTexture.loadFromFile( "./resources/tilesheet.png" ) )
	{
		printf( "Failed to load tile set texture!\n" );
		success = false;
	}

	if( !setTiles( tiles ) )
	{
		printf( "Failed to load tile set!\n" );
		success = false;
	}

	gFont = TTF_OpenFont( "./resources/lazy.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		SDL_Color textColor = { 0, 0, 0, 0xFF };
		if( !gPromptTextTexture.loadFromRenderedText( "Enter Text:", textColor ) )
		{
			printf( "Failed to render prompt text!\n" );
			success = false;
		}
	}

	if( !gButtonSpriteSheetTexture.loadFromFile( "./resources/buttonsheet.png" ) )
	{
		printf( "Failed to load button sprite texture!\n" );
		success = false;
	}
	else
	{
		for( int i = 0; i < 4; ++i )
		{
			gSpriteClips[ i ].x = 0;
			gSpriteClips[ i ].y = i * BUTTON_HEIGHT;
			gSpriteClips[ i ].w = BUTTON_WIDTH;
			gSpriteClips[ i ].h = BUTTON_HEIGHT;
		}

		gButtons[ 0 ].setPosition( 0, 0 );
		gButtons[ 1 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, 0 );
		gButtons[ 2 ].setPosition( 0, SCREEN_HEIGHT - BUTTON_HEIGHT );
		gButtons[ 3 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT );
	}

	gMusic = Mix_LoadMUS( "./resources/beat.wav" );
	if( gMusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	
	gScratch = Mix_LoadWAV( "./resources/scratch.wav" );
	if( gScratch == NULL )
	{
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	
	gHigh = Mix_LoadWAV( "./resources/high.wav" );
	if( gHigh == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	gMedium = Mix_LoadWAV( "./resources/medium.wav" );
	if( gMedium == NULL )
	{
		printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	gLow = Mix_LoadWAV( "./resources/low.wav" );
	if( gLow == NULL )
	{
		printf( "Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	if( !gDotTexture.loadFromFile( "./resources/dot.bmp" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	if( !gRedTexture.loadFromFile( "./resources/red.bmp" ) )
	{
		printf( "Failed to load red texture!\n" );
		success = false;
	}

	if( !gGreenTexture.loadFromFile( "./resources/green.bmp" ) )
	{
		printf( "Failed to load green texture!\n" );
		success = false;
	}

	if( !gBlueTexture.loadFromFile( "./resources/blue.bmp" ) )
	{
		printf( "Failed to load blue texture!\n" );
		success = false;
	}

	if( !gShimmerTexture.loadFromFile( "./resources/shimmer.bmp" ) )
	{
		printf( "Failed to load shimmer texture!\n" );
		success = false;
	}
	
	gRedTexture.setAlpha( 192 );
	gGreenTexture.setAlpha( 192 );
	gBlueTexture.setAlpha( 192 );
	gShimmerTexture.setAlpha( 192 );

	return success;
}

void close( Tile* tiles[] )
{
	for( int i = 0; i < TOTAL_TILES; ++i )
	{
		 if( tiles[ i ] != NULL )
		 {
			delete tiles[ i ];
			tiles[ i ] = NULL;
		 }
	}

	Mix_FreeChunk( gScratch );
	Mix_FreeChunk( gHigh );
	Mix_FreeChunk( gMedium );
	Mix_FreeChunk( gLow );
	gScratch = NULL;
	gHigh = NULL;
	gMedium = NULL;
	gLow = NULL;
	
	Mix_FreeMusic( gMusic );
	gMusic = NULL;

	gDotTexture.free();
	gRedTexture.free();
	gGreenTexture.free();
	gBlueTexture.free();
	gShimmerTexture.free();
	gFPSTextTexture.free();
	gPromptTextTexture.free();
	gInputTextTexture.free();
	gDotTexture.free();
	gTileTexture.free();
	gButtonSpriteSheetTexture.free();

	TTF_CloseFont( gFont );
	gFont = NULL;

	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

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

bool setTiles( Tile* tiles[] )
{
	bool tilesLoaded = true;

    int x = 0, y = 0;

    std::ifstream map( "./resources/lazy.map" );

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

			if( x >= LEVEL_WIDTH )
			{
				x = 0;

				y += TILE_HEIGHT;
			}
		}
		
		if( tilesLoaded )
		{
			gTileClips[ TILE_RED ].x = 0;
			gTileClips[ TILE_RED ].y = 0;
			gTileClips[ TILE_RED ].w = TILE_WIDTH;
			gTileClips[ TILE_RED ].h = TILE_HEIGHT;

			gTileClips[ TILE_GREEN ].x = 0;
			gTileClips[ TILE_GREEN ].y = TILE_HEIGHT;
			gTileClips[ TILE_GREEN ].w = TILE_WIDTH;
			gTileClips[ TILE_GREEN ].h = TILE_HEIGHT;

			gTileClips[ TILE_BLUE ].x = 0;
			gTileClips[ TILE_BLUE ].y = TILE_HEIGHT * 2;
			gTileClips[ TILE_BLUE ].w = TILE_WIDTH;
			gTileClips[ TILE_BLUE ].h = TILE_HEIGHT;

			gTileClips[ TILE_TOPLEFT ].x = TILE_WIDTH;
			gTileClips[ TILE_TOPLEFT ].y = 0;
			gTileClips[ TILE_TOPLEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_TOPLEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_LEFT ].x = TILE_WIDTH;
			gTileClips[ TILE_LEFT ].y = TILE_HEIGHT;
			gTileClips[ TILE_LEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_LEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_BOTTOMLEFT ].x = TILE_WIDTH;
			gTileClips[ TILE_BOTTOMLEFT ].y = TILE_HEIGHT * 2;
			gTileClips[ TILE_BOTTOMLEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_BOTTOMLEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_TOP ].x = TILE_WIDTH * 2;
			gTileClips[ TILE_TOP ].y = 0;
			gTileClips[ TILE_TOP ].w = TILE_WIDTH;
			gTileClips[ TILE_TOP ].h = TILE_HEIGHT;

			gTileClips[ TILE_CENTER ].x = TILE_WIDTH * 2;
			gTileClips[ TILE_CENTER ].y = TILE_HEIGHT;
			gTileClips[ TILE_CENTER ].w = TILE_WIDTH;
			gTileClips[ TILE_CENTER ].h = TILE_HEIGHT;

			gTileClips[ TILE_BOTTOM ].x = TILE_WIDTH * 2;
			gTileClips[ TILE_BOTTOM ].y = TILE_HEIGHT * 2;
			gTileClips[ TILE_BOTTOM ].w = TILE_WIDTH;
			gTileClips[ TILE_BOTTOM ].h = TILE_HEIGHT;

			gTileClips[ TILE_TOPRIGHT ].x = TILE_WIDTH * 3;
			gTileClips[ TILE_TOPRIGHT ].y = 0;
			gTileClips[ TILE_TOPRIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_TOPRIGHT ].h = TILE_HEIGHT;

			gTileClips[ TILE_RIGHT ].x = TILE_WIDTH * 3;
			gTileClips[ TILE_RIGHT ].y = TILE_HEIGHT;
			gTileClips[ TILE_RIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_RIGHT ].h = TILE_HEIGHT;

			gTileClips[ TILE_BOTTOMRIGHT ].x = TILE_WIDTH * 3;
			gTileClips[ TILE_BOTTOMRIGHT ].y = TILE_HEIGHT * 2;
			gTileClips[ TILE_BOTTOMRIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_BOTTOMRIGHT ].h = TILE_HEIGHT;
		}
	}

    map.close();

    return tilesLoaded;
}

bool touchesWall( SDL_Rect box, Tile* tiles[] )
{
    for( int i = 0; i < TOTAL_TILES; ++i )
    {
        if( ( tiles[ i ]->getType() >= TILE_CENTER ) && ( tiles[ i ]->getType() <= TILE_TOPLEFT ) )
        {
            if( checkCollision( box, tiles[ i ]->getBox() ) )
            {
                return true;
            }
        }
    }

    return false;
}

int main( int argc, char* args[] )
{
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		Tile* tileSet[ TOTAL_TILES ];

		if( !loadMedia( tileSet ) )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			bool quit = false;

			SDL_Event e;

			Timer fpsTimer;
			Timer capTimer;

			std::stringstream timeText;

			int countedFrames = 0;
			fpsTimer.start();

            // Current animation frame
			int frame = 0;

			SDL_Color textColor = { 0, 0, 0, 0xFF };

			std::string inputText = "Sample Text";
			gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );

			SDL_StartTextInput();

			Player dot;

			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			// Game loop
			while( !quit )
			{
				capTimer.start();

				bool renderText = false;

				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_KEYDOWN )
					{
						if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
						{
							inputText.pop_back();
							renderText = true;
						}
						else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
						{
							SDL_SetClipboardText( inputText.c_str() );
						}
						else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
						{
							inputText = SDL_GetClipboardText();
							renderText = true;
						}

						switch( e.key.keysym.sym )
						{
							case SDLK_1:
							Mix_PlayChannel( -1, gHigh, 0 );
							break;
							
							case SDLK_2:
							Mix_PlayChannel( -1, gMedium, 0 );
							break;
							
							case SDLK_3:
							Mix_PlayChannel( -1, gLow, 0 );
							break;
							
							case SDLK_4:
							Mix_PlayChannel( -1, gScratch, 0 );
							break;
							
							case SDLK_9:
							if( Mix_PlayingMusic() == 0 )
							{
								Mix_PlayMusic( gMusic, -1 );
							}
							else
							{
								if( Mix_PausedMusic() == 1 )
								{
									Mix_ResumeMusic();
								}
								else
								{
									Mix_PauseMusic();
								}
							}
							break;
							
							case SDLK_0:
							Mix_HaltMusic();
							break;
						}
					}
					else if( e.type == SDL_TEXTINPUT )
					{
						if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) )
						{
							inputText += e.text.text;
							renderText = true;
						}
					}

					for( int i = 0; i < TOTAL_BUTTONS; ++i )
					{
						gButtons[ i ].handleEvent( &e );
					}

					dot.handleEvent( e );
				}

				float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
				if( avgFPS > 2000000 )
				{
					avgFPS = 0;
				}

				timeText.str( "" );
				timeText << "Average Frames Per Second (With Cap) " << avgFPS;

				if( !gFPSTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
				{
					printf( "Unable to render FPS texture!\n" );
				}

				if( renderText )
				{
					if( inputText != "" )
					{
						gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );
					}
					else
					{
						gInputTextTexture.loadFromRenderedText( " ", textColor );
					}
				}

				dot.move( tileSet );
				dot.setCamera( camera );

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				for( int i = 0; i < TOTAL_TILES; ++i )
				{
					tileSet[ i ]->render( camera );
				}

				SDL_Rect* currentClip = &gDotSpriteClips[ frame / 4 ];
				dot.render( camera, currentClip );

				for( int i = 0; i < TOTAL_BUTTONS; ++i )
				{
					gButtons[ i ].render();
				}

				gPromptTextTexture.render( ( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 0 );
				gInputTextTexture.render( ( SCREEN_WIDTH /*- gInputTextTexture.getWidth()*/ ) / 2, gPromptTextTexture.getHeight() );
				gFPSTextTexture.render( ( SCREEN_WIDTH - gFPSTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gFPSTextTexture.getHeight() ) );

				SDL_RenderPresent( gRenderer );

				++frame;

				if( frame / 4 >= DOT_ANIMATION_FRAMES )
				{
					frame = 0;
				}

				++countedFrames;

				int frameTicks = capTimer.getTicks();
				if( frameTicks < SCREEN_TICK_PER_FRAME )
				{
					SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
				}
			}
            
			SDL_StopTextInput();
		}
		
		close( tileSet );
	}

	return 0;
}
