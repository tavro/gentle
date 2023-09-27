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
Canvas selectionCanvas;

Button startButton  {"Start",   SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, BUTTON_HEIGHT + 16, 	 96, BUTTON_HEIGHT};
Button optionsButton{"Options", SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, BUTTON_HEIGHT * 2 + 32, 96, BUTTON_HEIGHT};
Button quitButton   {"Quit",    SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, BUTTON_HEIGHT * 3 + 48, 96, BUTTON_HEIGHT};

UIPanel buttonPanel { 0, SCREEN_HEIGHT+BUTTON_HEIGHT, SCREEN_WIDTH, 320 };
std::vector<Button*> tileButtons;
int tileButtonAmount = (int)TileType::AMOUNT;

Button saveButton{"Save", BUTTON_WIDTH + 16, SCREEN_HEIGHT + UI_AREA - BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};

Image mainMenuImg{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

Text FPSText{"", 0, SCREEN_HEIGHT - 28};
Text promtText{"Main Menu", SCREEN_WIDTH / 2 - (8*14) / 2, 0};
InputField field{0,  SCREEN_HEIGHT + UI_AREA - BUTTON_HEIGHT, 96, 28, 20};

Player player;
TileMap tileMap;
AudioSource audioSource;

std::map<TileType, std::string> tileTypeMap;

Explorer explorer{ "", 0, SCREEN_HEIGHT+32, SCREEN_WIDTH/2, UI_AREA };
Heirarchy heirarchy{ SCREEN_WIDTH/2, SCREEN_HEIGHT+32, SCREEN_WIDTH/2, UI_AREA };
Inspector inspector{SCREEN_WIDTH, 0};

Scene testScene{};

GameObject a{};
GameObject b{20, 20, 20, 20};
GameObject c{0, 0, 20, 20};

bool init()
{
	testScene.addObj(&a);
	testScene.addObj(&b);
	testScene.addObj(&c);

	tileTypeMap[TileType::DIRT] = "Dirt";
	tileTypeMap[TileType::GRASS] = "Grass";
	tileTypeMap[TileType::STONE] = "Stone";
	tileTypeMap[TileType::WALL_CENTER] = "W Center";
	tileTypeMap[TileType::WALL_TOP] = "W Top";
	tileTypeMap[TileType::WALL_TOP_RIGHT] = "W Top Right";
	tileTypeMap[TileType::WALL_RIGHT] = "W Right";
	tileTypeMap[TileType::WALL_BOTTOM_RIGHT] = "W Bottom Right";
	tileTypeMap[TileType::WALL_BOTTOM] = "W Bottom";
	tileTypeMap[TileType::WALL_BOTTOM_LEFT] = "W Bottom Left";
	tileTypeMap[TileType::WALL_LEFT] = "W Left";
	tileTypeMap[TileType::WALL_TOP_LEFT] = "W Top Left";
	tileTypeMap[TileType::GOLD] = "Gold";

	for(int i = 0; i < tileButtonAmount; i++)
	{
		Button* b = new Button(tileTypeMap[(TileType)i], 0, 0);
		tileButtons.push_back(b);
	}

	player.setPosition(Vector2D{SCREEN_WIDTH/2, SCREEN_HEIGHT/2});

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

	heirarchy.setActiveScene(&testScene, renderer);

	inspector.setActiveObj(&a);
	inspector.loadFont(renderer);

	explorer.folderTexture.loadFromFile( "./resources/folder.png", renderer );
	explorer.fileTexture.loadFromFile( "./resources/file.png", renderer );
	explorer.texture.loadFromFile( "./resources/explorer.png", renderer );

	mainMenuImg.getTexture().loadFromFile( "./resources/main-menu-background.png", renderer );
	canvas.addObj(&mainMenuImg);

    success = tileMap.loadTexture( renderer, "./resources/tilesheet.png" );
    
	field.getText().loadFont( "./resources/font.ttf", 28 );

	startButton.getText().loadFont( "./resources/font.ttf", 28 );
	optionsButton.getText().loadFont( "./resources/font.ttf", 28 );
	quitButton.getText().loadFont( "./resources/font.ttf", 28 );

	for(int i = 0; i < tileButtonAmount; i++)
	{
		tileButtons[i]->getText().loadFont( "./resources/font.ttf", 28 );
	}

	saveButton.getText().loadFont( "./resources/font.ttf", 28 );

    success = promtText.loadFont( "./resources/font.ttf", 28 );
    success = promtText.loadTexture( renderer );
	canvas.addObj(&promtText);

    success = FPSText.loadFont( "./resources/font.ttf", 28 );
    success = FPSText.loadTexture( renderer );
	canvas.addObj(&FPSText);

	field.getTexture().loadFromFile( "./resources/buttonsheet.png", renderer );

    startButton.loadSpriteSheet( "./resources/buttonsheet.png", renderer );
    optionsButton.loadSpriteSheet( "./resources/buttonsheet.png", renderer );
    quitButton.loadSpriteSheet( "./resources/buttonsheet.png", renderer );
	
	for(int i = 0; i < tileButtonAmount; i++)
	{
		tileButtons[i]->loadSpriteSheet( "./resources/buttonsheet.png", renderer );
	}

	saveButton.loadSpriteSheet( "./resources/buttonsheet.png", renderer );
	
	for( int i = 0; i < 4; ++i )
	{
		field.getSpriteClip( i ).x = 0;
		field.getSpriteClip( i ).y = i * BUTTON_HEIGHT;
        field.getSpriteClip( i ).w = BUTTON_WIDTH;
        field.getSpriteClip( i ).h = BUTTON_HEIGHT;
	}
	selectionCanvas.addObj(&field);

	canvas.addObj(&startButton);
	canvas.addObj(&optionsButton);
	canvas.addObj(&quitButton);

	selectionCanvas.addObj(&saveButton);
	
	for(int i = 0; i < tileButtonAmount; i++)
	{
		buttonPanel.addObj(tileButtons[i]);
	}
	buttonPanel.setMaxHeight();
	buttonPanel.alignObjs();
	selectionCanvas.addObj(&buttonPanel);

	selectionCanvas.setActive(false);

    audioSource.addMusic( "./resources/beat.wav" );

    audioSource.addSound( "./resources/scratch.wav" );
    audioSource.addSound( "./resources/high.wav" );
    audioSource.addSound( "./resources/medium.wav" );
    audioSource.addSound( "./resources/low.wav" );
	
    success = player.loadTexture( renderer, "./resources/player.bmp" );

	player.addAnimation("./resources/playeranim1.png", renderer);
	player.addAnimation("./resources/playeranim2.png", renderer);
	player.addAnimation("./resources/playeranim3.png", renderer);
	player.addAnimation("./resources/playeranim4.png", renderer);

    for(int i = 0; i < player.TOTAL_PARTICLES; i++) {
        if( !player.particles[i]->redTexture.loadFromFile( "./resources/red.bmp", renderer ) )
        {
            printf( "Failed to load red texture!\n" );
            success = false;
        }

        if( !player.particles[i]->greenTexture.loadFromFile( "./resources/green.bmp", renderer ) )
        {
            printf( "Failed to load green texture!\n" );
            success = false;
        }

        if( !player.particles[i]->blueTexture.loadFromFile( "./resources/blue.bmp", renderer ) )
        {
            printf( "Failed to load blue texture!\n" );
            success = false;
        }

        if( !player.particles[i]->shimmerTexture.loadFromFile( "./resources/shimmer.bmp", renderer ) )
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

	canvas.freeTextures();
	selectionCanvas.freeTextures();

	player.getTexture().free();
	tileMap.getTexture().free();

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

			field.setText("Sample Text");
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
								inspector.setActiveObj(testScene.getObjFromName(heirarchy.getActiveObjName()));
							break;
							case SDLK_DOWN: 
								heirarchy.increaseIndex();
								inspector.setActiveObj(testScene.getObjFromName(heirarchy.getActiveObjName()));
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
					else if( e.type == SDL_TEXTINPUT )
					{
						if(!(SDL_GetModState() & KMOD_CTRL))
						{
							field.appendToText(e.text.text);
						}
					}
					else
					{
						int mouseX, mouseY;
						SDL_GetMouseState( &mouseX, &mouseY );
						if(mouseY <= SCREEN_HEIGHT)
						{
							switch( e.type )
							{
								case SDL_MOUSEBUTTONDOWN:
								int tileX = (mouseX / TILE_WIDTH);
								int tileY = (mouseY / TILE_HEIGHT);
								int index = tileMap.getTileFromScreenPosition(mouseX, mouseY);
								Tile* t = new Tile( tileX*TILE_WIDTH, tileY*TILE_HEIGHT, currentTile );
								tileMap.setTile(index, t);
								break;
							}
						}
					}

					canvas.handleEvent( &e );
					selectionCanvas.handleEvent( &e );
					player.handleEvent( e );

				}

				if(startButton.isToggled())
				{
					canvas.setActive(false);
					selectionCanvas.setActive(true);
				}

				if(quitButton.isToggled())
				{
					quit = true;
				}

				for(int i = 0; i < tileButtonAmount; i++)
				{
					if(tileButtons[i]->isToggled())
					{
						for(int j = 0; j < tileButtonAmount; j++)
						{
							tileButtons[j]->setToggle(false);
						}
						
						currentTile = i;
						break;
					}
				}

				if(saveButton.isToggled())
				{
					saveButton.setToggle(false);
					tileMap.saveTilesToFile(field.getContent());
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

				player.move( tileMap.getTiles() );
				player.setCamera( camera );

				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderer );

                tileMap.render( camera, renderer );

				player.render ( camera, renderer );

                canvas.render(renderer);
				selectionCanvas.render(renderer);

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
