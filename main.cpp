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

const int TOTAL_TILES = 300;
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

const int TOTAL_BUTTONS = 4;

const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

bool init();

bool loadMedia( Tile* tiles[] );

void close( Tile* tiles[] );

bool setTiles( Tile *tiles[] );

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

Texture gPromptTextTexture;
Texture gInputTextTexture;
Texture gTileTexture;
Texture gFPSTextTexture;

SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];

const int DOT_ANIMATION_FRAMES = 4;
SDL_Rect gDotSpriteClips[ DOT_ANIMATION_FRAMES ];

Button gButtons[ TOTAL_BUTTONS ]; 

Mix_Music *gMusic = NULL;

Mix_Chunk *gScratch = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gLow = NULL;

Player dot;

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

	if( !dot.texture.loadFromFile( "./resources/dotanim.png", gRenderer ) )
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

	if( !gTileTexture.loadFromFile( "./resources/tilesheet.png", gRenderer ) )
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
		if( !gPromptTextTexture.loadFromRenderedText( "Enter Text:", textColor, gRenderer, gFont ) )
		{
			printf( "Failed to render prompt text!\n" );
			success = false;
		}
	}

    for(int x = 0; x < 4; x++ ) {
        if( !gButtons[x].buttonSpriteSheetTexture.loadFromFile( "./resources/buttonsheet.png", gRenderer ) )
        {
            printf( "Failed to load button sprite texture!\n" );
            success = false;
        }
        else
        {
            for( int i = 0; i < 4; ++i )
            {
                gButtons[x].spriteClips[ i ].x = 0;
                gButtons[x].spriteClips[ i ].y = i * BUTTON_HEIGHT;
                gButtons[x].spriteClips[ i ].w = BUTTON_WIDTH;
                gButtons[x].spriteClips[ i ].h = BUTTON_HEIGHT;
            }
        }
    }
    gButtons[ 0 ].setPosition( 0, 0 );
    gButtons[ 1 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, 0 );
    gButtons[ 2 ].setPosition( 0, SCREEN_HEIGHT - BUTTON_HEIGHT );
    gButtons[ 3 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT );

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

	if( !dot.texture.loadFromFile( "./resources/dot.bmp", gRenderer ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

    for(int i = 0; i < dot.TOTAL_PARTICLES; i++) {
        if( !dot.particles[i]->redTexture.loadFromFile( "./resources/red.bmp", gRenderer ) )
        {
            printf( "Failed to load red texture!\n" );
            success = false;
        }

        if( !dot.particles[i]->greenTexture.loadFromFile( "./resources/green.bmp", gRenderer ) )
        {
            printf( "Failed to load green texture!\n" );
            success = false;
        }

        if( !dot.particles[i]->blueTexture.loadFromFile( "./resources/blue.bmp", gRenderer ) )
        {
            printf( "Failed to load blue texture!\n" );
            success = false;
        }

        if( !dot.particles[i]->shimmerTexture.loadFromFile( "./resources/shimmer.bmp", gRenderer ) )
        {
            printf( "Failed to load shimmer texture!\n" );
            success = false;
        }
	
	    dot.particles[i]->redTexture.setAlpha( 192 );
	    dot.particles[i]->greenTexture.setAlpha( 192 );
	    dot.particles[i]->blueTexture.setAlpha( 192 );
	    dot.particles[i]->shimmerTexture.setAlpha( 192 );
    }

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

    for(int i = 0; i < dot.TOTAL_PARTICLES; i++) {
        dot.particles[i]->redTexture.free();
        dot.particles[i]->greenTexture.free();
        dot.particles[i]->blueTexture.free();
        dot.particles[i]->shimmerTexture.free();
    }

	gFPSTextTexture.free();
	gPromptTextTexture.free();
	gInputTextTexture.free();
	dot.texture.free();
	gTileTexture.free();

    for(int x = 0; x < 4; x++ ) {
	    gButtons[x].buttonSpriteSheetTexture.free();
    }

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
			gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor, gRenderer, gFont );

			SDL_StartTextInput();

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

				if( !gFPSTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor, gRenderer, gFont ) )
				{
					printf( "Unable to render FPS texture!\n" );
				}

				if( renderText )
				{
					if( inputText != "" )
					{
						gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor, gRenderer, gFont );
					}
					else
					{
						gInputTextTexture.loadFromRenderedText( " ", textColor, gRenderer, gFont );
					}
				}

				dot.move( tileSet );
				dot.setCamera( camera );

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				for( int i = 0; i < TOTAL_TILES; ++i )
				{
					tileSet[ i ]->render( camera, gTileClips, gRenderer, gTileTexture);
				}

				SDL_Rect* currentClip = &gDotSpriteClips[ frame / 4 ];
				dot.render( camera, currentClip, gRenderer );

				for( int i = 0; i < TOTAL_BUTTONS; ++i )
				{
					gButtons[ i ].render(gRenderer);
				}

				gPromptTextTexture.render( ( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 0, NULL, 0.0, NULL, SDL_FLIP_NONE, gRenderer );
				gInputTextTexture.render( ( SCREEN_WIDTH /*- gInputTextTexture.getWidth()*/ ) / 2, gPromptTextTexture.getHeight(), NULL, 0.0, NULL, SDL_FLIP_NONE, gRenderer );
				gFPSTextTexture.render( ( SCREEN_WIDTH - gFPSTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gFPSTextTexture.getHeight() ), NULL, 0.0, NULL, SDL_FLIP_NONE, gRenderer );

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
