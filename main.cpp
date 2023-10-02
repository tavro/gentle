#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <random>

#include "./headers/texture.h"
#include "./headers/particle.h"
#include "./headers/player.h"
#include "./headers/timer.h"
#include "./headers/tile.h"
#include "./headers/button.h"
#include "./headers/tile_map.h"
#include "./headers/audio_source.h"
#include "./headers/text.h"
#include "./headers/input_field.h"
#include "./headers/canvas.h"
#include "./headers/image.h"
#include "./headers/ui_panel.h"
#include "./headers/game_object.h"
#include "./headers/scene.h"
#include "./headers/physics_object.h"
#include "./headers/utils/constants.h"

#include "./headers/game/game.h"

bool init();
bool loadMedia();
void close();

SDL_Window*     window 	 = NULL;
SDL_Renderer*   renderer = NULL;

Canvas canvas;
game::Game *gameInstance = NULL;

GameObject wall{{32, 32}, {SCREEN_WIDTH-32, 32+16}};
GameObject wall2{{32, SCREEN_HEIGHT-32-16}, {SCREEN_WIDTH-32, SCREEN_HEIGHT-32}};

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

		window = SDL_CreateWindow( "gentle game engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( renderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

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

bool loadMedia()
{
	bool success = true;

	success = gameInstance->loadMedia();
	
	return success;
}

void close()
{
	free(gameInstance);

	canvas.freeTextures();

	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		gameInstance = new game::Game(renderer);

		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			bool quit = false;

			SDL_Event e;

			Timer fpsTimer;
			Timer capTimer;

			int currentTile = 0;

			int countedFrames = 0;
			fpsTimer.start();

			SDL_Color textColor = { 0, 0, 0, 0xFF };

			SDL_StartTextInput();

			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			// Game loop
			while( !quit )
			{
				capTimer.start();

				float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
				if( avgFPS > 2000000 )
				{
					avgFPS = 0;
				}

				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					canvas.handleEvent( &e );

					gameInstance->handleEvent(&e);
				}
				gameInstance->update(avgFPS);

				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderer );

                canvas.render(renderer);

				gameInstance->render();

				/* ISAKS TESTGREJOR
				wallScene->render(renderer);
				*/

				SDL_RenderPresent( renderer );

				++countedFrames;
				int frameTicks = capTimer.getTicks();
				if( frameTicks < SCREEN_TICK_PER_FRAME )
				{
					SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
				}
			}
            
			SDL_StopTextInput();
		}
		
		close();
	}

	return 0;
}
