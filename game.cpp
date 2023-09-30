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

#include "./headers/editor/explorer.h"
#include "./headers/editor/heirarchy.h"
#include "./headers/editor/inspector.h"

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

const int UI_AREA = 256+64;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

const int BUTTON_WIDTH = 96;
const int BUTTON_HEIGHT = 32;

bool init();
bool loadMedia();
void close();

SDL_Window*     window 	 = NULL;
SDL_Renderer*   renderer = NULL;

Canvas canvas;

Image mainMenuImg{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

Text FPSText{"", 0, SCREEN_HEIGHT - 28};

Explorer explorer{ "", 0, SCREEN_HEIGHT+32, SCREEN_WIDTH/2, UI_AREA };
Heirarchy heirarchy{ SCREEN_WIDTH/2, SCREEN_HEIGHT+32, SCREEN_WIDTH/2, UI_AREA };
Inspector inspector{SCREEN_WIDTH, 0};

Scene* testScene = new Scene{};

Button saveButton{"Save", 0, SCREEN_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
Button cdButton{"cd ..", BUTTON_WIDTH*2+16*2, SCREEN_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
Button createButton{"Create Empty Obj", BUTTON_WIDTH*3+16*3, SCREEN_HEIGHT};

InputField field{BUTTON_WIDTH+16,  SCREEN_HEIGHT, 96, 28, 8};

GameObject a{};

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

		window = SDL_CreateWindow( "gentle game engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH + UI_AREA, SCREEN_HEIGHT + UI_AREA, SDL_WINDOW_SHOWN );
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

	testScene->load("./test.scene");
	testScene->loadTextures(renderer);

	heirarchy.loadTextures(renderer);
	heirarchy.setActiveScene(testScene, renderer);

	inspector.setActiveObj(&a);
	inspector.loadFont(renderer);

	explorer.folderTexture.loadFromFile( "./resources/folderSheet.png", renderer );
	explorer.fileTexture.loadFromFile( "./resources/fileSheet.png", renderer );
	explorer.texture.loadFromFile( "./resources/explorer.png", renderer );

	mainMenuImg.getTexture().loadFromFile( "./resources/main-menu-background.png", renderer );
	canvas.addObj(&mainMenuImg);

	saveButton.getText().loadFont( "./resources/font.ttf", 28 );
	saveButton.loadSpriteSheet( "./resources/buttonsheet.png", renderer );
	canvas.addObj(&saveButton);

	createButton.getText().loadFont( "./resources/font.ttf", 28 );
	createButton.loadSpriteSheet( "./resources/buttonsheet.png", renderer );
	canvas.addObj(&createButton);

	cdButton.getText().loadFont( "./resources/font.ttf", 28 );
	cdButton.loadSpriteSheet( "./resources/buttonsheet.png", renderer );
	canvas.addObj(&cdButton);

	field.getText().loadFont( "./resources/font.ttf", 28 );
	field.getTexture().loadFromFile( "./resources/buttonsheet.png", renderer );
	for( int i = 0; i < 4; ++i )
	{
		field.getSpriteClip( i ).x = 0;
		field.getSpriteClip( i ).y = i * BUTTON_HEIGHT;
        field.getSpriteClip( i ).w = BUTTON_WIDTH;
        field.getSpriteClip( i ).h = BUTTON_HEIGHT;
	}
	canvas.addObj(&field);

    success = !FPSText.loadFont( "./resources/font.ttf", 28 );
    success = !FPSText.loadTexture( renderer );
	canvas.addObj(&FPSText);

	return success;
}

void close()
{
	//gScratch = NULL; <------ TODO
	//gHigh = NULL;
	//gMedium = NULL;
	//gLow = NULL;
	
	// gMusic = NULL;

	canvas.freeTextures();

	//TTF_CloseFont( globalFont );
	// globalFont = NULL;

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

			int currentTile = 0;

			int countedFrames = 0;
			fpsTimer.start();

			SDL_Color textColor = { 0, 0, 0, 0xFF };
			
			field.setText("sample.scene");
			field.loadTextTexture(renderer);
			
			SDL_StartTextInput();

			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			// Game loop
			while( !quit )
			{
				capTimer.start();

				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_KEYDOWN )
					{
						// For heirarchy
						switch( e.key.keysym.sym )
						{
							case SDLK_UP: 
								heirarchy.decreaseIndex();
								inspector.setActiveObj(testScene->getObjFromName(heirarchy.getActiveObjName()));
							break;
							case SDLK_DOWN: 
								heirarchy.increaseIndex();
								inspector.setActiveObj(testScene->getObjFromName(heirarchy.getActiveObjName()));
							break;
						}

						if( e.key.keysym.sym == SDLK_BACKSPACE)
						{
							field.removeChar();
						}
						else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
						{
							SDL_SetClipboardText( field.getContent().c_str() );
						}
						else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
						{
							field.updateText(SDL_GetClipboardText());
						}
					}
					else if( e.type == SDL_TEXTINPUT )
					{
						if(!(SDL_GetModState() & KMOD_CTRL))
						{
							field.appendToText(e.text.text);
						}
					}

					canvas.handleEvent( &e );
					explorer.handleEvent( &e );

				}

				if(saveButton.isToggled())
				{
					testScene->save("");
					saveButton.setToggle(false);
				}
				if(createButton.isToggled())
				{
					testScene->createEmptyObject();
					heirarchy.setActiveScene(testScene, renderer);
					createButton.setToggle(false);
				}
				if(cdButton.isToggled())
				{
					explorer.goBack();
					cdButton.setToggle(false);
				}

				if(explorer.fileHasChanged())
				{
					testScene->load(explorer.currentFile); // TODO: Check if scene
					testScene->loadTextures(renderer);
					heirarchy.setActiveScene(testScene, renderer);
					explorer.toggleFileChanged();
				}

				float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
				if( avgFPS > 2000000 )
				{
					avgFPS = 0;
				}

				timeText.str( "" );
				timeText << "Average FPS: " << avgFPS;
                FPSText.updateContent(timeText.str());
                FPSText.loadTexture(renderer);

				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderer );

                canvas.render(renderer);

				testScene->render(renderer);
				explorer.render(renderer);
				heirarchy.render(renderer);
				inspector.render(renderer);

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