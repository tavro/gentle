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
#include "./headers/tile_map.h"
#include "./headers/audio_source.h"
#include "./headers/text.h"

const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

bool init();
bool loadMedia();
void close();

SDL_Window*     gWindow = NULL;
SDL_Renderer*   gRenderer = NULL;

Texture gInputTextTexture;                          // TODO: Implement Input Field class

Button gButtons[ 4 ];

Text FPSText{"", 0, SCREEN_HEIGHT - 28};
Text promtText{"Sample Text", SCREEN_WIDTH / 2, 0};

Player player;
TileMap tileMap;
AudioSource audioSource;

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

bool loadMedia()
{
	bool success = true;

    success = tileMap.loadTexture( gRenderer, "./resources/tilesheet.png" );
    
    success = promtText.loadFont( "./resources/lazy.ttf", 28 );
    success = promtText.loadTexture( gRenderer );

    success = FPSText.loadFont( "./resources/lazy.ttf", 28 );
    success = FPSText.loadTexture( gRenderer );

    for(int x = 0; x < 4; x++ ) {
        if( !gButtons[x].getTexture().loadFromFile( "./resources/buttonsheet.png", gRenderer ) )
        {
            printf( "Failed to load button sprite texture!\n" );
            success = false;
        }
        else
        {
            for( int i = 0; i < 4; ++i )
            {
                gButtons[x].getSpriteClip( i ).x = 0;
                gButtons[x].getSpriteClip( i ).y = i * BUTTON_HEIGHT;
                gButtons[x].getSpriteClip( i ).w = BUTTON_WIDTH;
                gButtons[x].getSpriteClip( i ).h = BUTTON_HEIGHT;
            }
        }
    }
    gButtons[ 0 ].setPosition( 0, 0 );
    gButtons[ 1 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, 0 );
    gButtons[ 2 ].setPosition( 0, SCREEN_HEIGHT - BUTTON_HEIGHT );
    gButtons[ 3 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT );

    audioSource.addMusic( "./resources/beat.wav" );

    audioSource.addSound( "./resources/scratch.wav" );
    audioSource.addSound( "./resources/high.wav" );
    audioSource.addSound( "./resources/medium.wav" );
    audioSource.addSound( "./resources/low.wav" );
	
    success = player.loadTexture( gRenderer, "./resources/dot.bmp" );
    success = player.getAnimation().loadTexture( gRenderer, "./resources/dotanim.png" );

    for(int i = 0; i < player.TOTAL_PARTICLES; i++) {
        if( !player.particles[i]->redTexture.loadFromFile( "./resources/red.bmp", gRenderer ) )
        {
            printf( "Failed to load red texture!\n" );
            success = false;
        }

        if( !player.particles[i]->greenTexture.loadFromFile( "./resources/green.bmp", gRenderer ) )
        {
            printf( "Failed to load green texture!\n" );
            success = false;
        }

        if( !player.particles[i]->blueTexture.loadFromFile( "./resources/blue.bmp", gRenderer ) )
        {
            printf( "Failed to load blue texture!\n" );
            success = false;
        }

        if( !player.particles[i]->shimmerTexture.loadFromFile( "./resources/shimmer.bmp", gRenderer ) )
        {
            printf( "Failed to load shimmer texture!\n" );
            success = false;
        }
	
	    player.particles[i]->redTexture.setAlpha( 192 );
	    player.particles[i]->greenTexture.setAlpha( 192 );
	    player.particles[i]->blueTexture.setAlpha( 192 );
	    player.particles[i]->shimmerTexture.setAlpha( 192 );
    }

	return success;
}

void close()
{
    tileMap.deleteTiles();

	Mix_FreeChunk( audioSource.getSound(0) );
	Mix_FreeChunk( audioSource.getSound(1) );
	Mix_FreeChunk( audioSource.getSound(2) );
	Mix_FreeChunk( audioSource.getSound(3) );
	//gScratch = NULL; <------ TODO
	//gHigh = NULL;
	//gMedium = NULL;
	//gLow = NULL;
	
	Mix_FreeMusic( audioSource.getMusic(0) );
	// gMusic = NULL;

    for(int i = 0; i < player.TOTAL_PARTICLES; i++) {
        player.particles[i]->redTexture.free();
        player.particles[i]->greenTexture.free();
        player.particles[i]->blueTexture.free();
        player.particles[i]->shimmerTexture.free();
    }

	FPSText.getTexture().free();
	promtText.getTexture().free();
	gInputTextTexture.free();
	player.getTexture().free();
	tileMap.getTexture().free();

    for(int x = 0; x < 4; x++ ) {
	    gButtons[x].getTexture().free();
    }

	//TTF_CloseFont( globalFont );
	// globalFont = NULL;

	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

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

			std::stringstream timeText;

			int countedFrames = 0;
			fpsTimer.start();

            // Current animation frame
			int frame = 0;

			SDL_Color textColor = { 0, 0, 0, 0xFF };

			std::string inputText = "Sample Text";
			//gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor, gRenderer, globalFont );

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
							Mix_PlayChannel( -1, audioSource.getSound(0), 0 );
							break;
							
							case SDLK_2:
							Mix_PlayChannel( -1, audioSource.getSound(1), 0 );
							break;
							
							case SDLK_3:
							Mix_PlayChannel( -1, audioSource.getSound(2), 0 );
							break;
							
							case SDLK_4:
							Mix_PlayChannel( -1, audioSource.getSound(3), 0 );
							break;
							
							case SDLK_9:
							if( Mix_PlayingMusic() == 0 )
							{
								Mix_PlayMusic( audioSource.getMusic(0), -1 );
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
                    /*
					else if( e.type == SDL_TEXTINPUT )
					{
						if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) )
						{
							inputText += e.text.text;
							renderText = true;
						}
					}
                    */

					for( int i = 0; i < 4; ++i )
					{
						gButtons[ i ].handleEvent( &e );
					}

					player.handleEvent( e );
				}

				float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
				if( avgFPS > 2000000 )
				{
					avgFPS = 0;
				}

				timeText.str( "" );
				timeText << "Average Frames Per Second (With Cap) " << avgFPS;
                FPSText.updateContent(timeText.str());
                FPSText.loadTexture(gRenderer);

                /*
				if( renderText )
				{
					if( inputText != "" )
					{
						gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor, gRenderer, globalFont );
					}
					else
					{
						gInputTextTexture.loadFromRenderedText( " ", textColor, gRenderer, globalFont );
					}
				}
                */

				player.move( tileMap.getTiles() );
				player.setCamera( camera );

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

                tileMap.render( camera, gRenderer );
				player.render( camera, gRenderer );

				for( int i = 0; i < 4; ++i )
				{
					gButtons[ i ].render(gRenderer);
				}

                promtText.render(gRenderer);
                FPSText.render(gRenderer);

				// gInputTextTexture.render( ( SCREEN_WIDTH /*- gInputTextTexture.getWidth()*/ ) / 2, gPromptTextTexture.getHeight(), NULL, 0.0, NULL, SDL_FLIP_NONE, gRenderer );

				SDL_RenderPresent( gRenderer );

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
