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

#include "./headers/utils/constants.h"

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
#include "./headers/physics_object.h"

bool init();
bool loadMedia();
void close();

SDL_Window*     window 	 = NULL;
SDL_Renderer*   renderer = NULL;

Canvas canvas;

Image mainMenuImg{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

Text FPSText{"", 0, SCREEN_HEIGHT - 28};

GameObject cursor{{SCREEN_WIDTH, SCREEN_HEIGHT}, {32, 32}, {0, 0}, "Cursor", "./resources/hand-point.png"};

GameObject box{{0, 0}, {32, 16}, {0, 0}, "Box", "./resources/gameobject2.png"};

Scene* wallScene = new Scene{};
Scene* boxScene = new Scene{};
Scene* testScene = new Scene{};

GameObject* activeObj;

GameObject a{};

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

float getRandomAngle()
{
	std::random_device rd;
    std::mt19937 gen(rd());

    float lower_bound = 0.0; // Lower bound of the range
    float upper_bound = 360.0; // Upper bound of the range

    std::uniform_real_distribution<float> distribution(lower_bound, upper_bound);

    float random_float = distribution(gen);
	return random_float;
}

bool loadMedia()
{
	bool success = true;

	testScene->load("./test.scene");

	std::string result;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, SCREEN_HEIGHT);
    std::uniform_int_distribution<int> distribution2(-10, 10);
    std::uniform_int_distribution<int> distribution3(1, 10);

	int xVel = distribution2(generator);
	int yVel = distribution2(generator);

	PhysicsObject* bed = new PhysicsObject{{distribution(generator), distribution(generator)}, {32, 32}, {xVel, yVel}, "bed", "./resources/furniture/0.png", 12};
 	bed->loadTexture(renderer);
	bed->setRotation(getRandomAngle());
	boxScene->addObj(bed);
	PhysicsObject* sofa = new PhysicsObject{{distribution(generator), distribution(generator)}, {32, 32}, {xVel, yVel}, "sofa", "./resources/furniture/1.png", 10};
 	sofa->loadTexture(renderer);
	sofa->setRotation(getRandomAngle());
	boxScene->addObj(sofa);
	PhysicsObject* piano = new PhysicsObject{{distribution(generator), distribution(generator)}, {32, 32}, {xVel, yVel}, "piano", "./resources/furniture/2.png", 20};
 	piano->loadTexture(renderer);
	piano->setRotation(getRandomAngle());
	boxScene->addObj(piano);
	PhysicsObject* plant = new PhysicsObject{{distribution(generator), distribution(generator)}, {32, 32}, {xVel, yVel}, "plant", "./resources/furniture/3.png", 2};
 	plant->loadTexture(renderer);
	plant->setRotation(getRandomAngle());
	boxScene->addObj(plant);
	PhysicsObject* oven = new PhysicsObject{{distribution(generator), distribution(generator)}, {32, 32}, {xVel, yVel}, "oven", "./resources/furniture/4.png", 15};
 	oven->loadTexture(renderer);
	oven->setRotation(getRandomAngle());
	boxScene->addObj(oven);


	wall.loadTexture(renderer);
	wall2.loadTexture(renderer);
	wallScene->addObj(&wall);
	wallScene->addObj(&wall2);

	cursor.loadTexture(renderer);

	box.loadTexture(renderer);

    success = !FPSText.loadFont( "./resources/font.ttf", 28 );
    success = !FPSText.loadTexture( renderer );
	canvas.addObj(&FPSText);

	return success;
}

void close()
{
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

			SDL_StartTextInput();

			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			bool isClosed = false;
			// Game loop
			while( !quit )
			{
				capTimer.start();

				for (auto* obj: boxScene->getObjs())
				{
					if(obj->isInside(cursor.getPosition().getX()+32/2, cursor.getPosition().getY()+32/2))
					{
						if(!isClosed)
						{
							cursor.setTexturePath("./resources/hand.png");
							cursor.loadTexture(renderer);
							activeObj = obj;
							break;
						}
					}
					else
					{
						if(!isClosed)
						{
							cursor.setTexturePath("./resources/hand-point.png");
							cursor.loadTexture(renderer);
						}
					}
				}

				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_KEYDOWN )
					{
						std::string result;
						std::random_device rd;
						std::mt19937 generator(rd());
						std::uniform_int_distribution<int> distribution2(-10, 10);
						int xVel = distribution2(generator);
						int yVel = distribution2(generator);
						switch( e.key.keysym.sym )
						{
							case SDLK_UP:
								activeObj->getVelocity().set(xVel, yVel);
							break;
							case SDLK_DOWN:
							break;
						}
					}
					else
					{
						switch( e.type )
						{
							case SDL_MOUSEBUTTONDOWN:
								cursor.setTexturePath("./resources/hand-closed.png");
								cursor.loadTexture(renderer);
								isClosed = true;
							break;
							
							case SDL_MOUSEBUTTONUP:
								isClosed = false;
							break;

							case SDL_MOUSEWHEEL:
                				int scroll = e.wheel.y * 5;
								//std::cout << "Y:" << scroll << std::endl;
								activeObj->increaseRotation(scroll);
							break;
						}
					}

					canvas.handleEvent( &e );

					int x, y;
                	SDL_GetMouseState( &x, &y );
					cursor.getPosition().set(x-32/2, y-32/2);

					box.handleEvent( &e );
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
				//box.render(renderer);
	
				/*
				if(activeObj != nullptr)
				{
					activeObj->handleCollisions(boxScene->getObjs());
				}
				*/

				for (auto* obj: boxScene->getObjs())
				{
					obj->handleCollisions(boxScene->getObjs());
					obj->handleCollisions(wallScene->getObjs());
					obj->move();
				}

				boxScene->render(renderer);

				wallScene->render(renderer);

				cursor.render(renderer);

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
