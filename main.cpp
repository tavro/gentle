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

struct FurnitureMeta {
    int maxAmount;
    float weight;
    std::vector<std::string> compatableRooms;

    FurnitureMeta(int maxAmount, float weight, const std::vector<std::string>& rooms)
        : maxAmount(maxAmount), weight(weight), compatableRooms(rooms) {}
};

bool loadMedia()
{
	bool success = true;

	testScene->load("./test.scene");

	std::map<std::string, FurnitureMeta> furniture {
		{"bed",  			{1 , 12, {"bedroom"}}}, 
		{"sofa", 			{1 , 10, {"livingroom"}}}, 
		{"piano",			{1 , 20, {"bedroom", "livingroom"}}}, 
		{"plant",			{21, 3 , {"bedroom", "livingroom", "bathroom"}}},
		{"oven", 			{1 , 15, {"kitchen"}}},
		{"bathtub", 		{1 , 10, {"bathroom"}}}, 
		{"bedsidetable",	{2 , 6 , {"bedroom"}}},
		{"bookshelf", 		{2 , 7 , {"bedroom", "livingroom"}}}, 
		{"chair", 			{12, 5 , {"bedroom", "livingroom", "kitchen"}}},
		{"coffeetable", 	{2 , 7 , {"bedroom", "livingroom", "kitchen"}}},
		{"dinnertable", 	{2 , 10, {"livingroom", "kitchen"}}},
		{"dishwasher", 		{1 , 13, {"kitchen"}}},
		{"dresser", 		{2 , 9 , {"bedroom", "livingroom"}}},
		{"fridge", 			{1 , 14, {"kitchen"}}}, 
		{"lamp", 			{14, 3 , {"bedroom", "livingroom", "kitchen", "bathroom"}}}, 
		{"sink", 			{2 , 12, {"kitchen", "bathroom"}}}, 
		{"toilet", 			{1 , 7 , {"bathroom"}}}, 
		{"washingmachine", 	{1 , 13, {"bathroom"}}},
		{"washingstation", 	{1 , 7 , {"bathroom"}}}
	};

	std::vector<std::vector<int>> rooms;
	rooms.push_back({0, 0, 292, 300}); 
	rooms.push_back({0, 300, 292, 100}); 
	rooms.push_back({292, 0, 307, 190}); 
	rooms.push_back({292, 190, 223, 209}); 
	rooms.push_back({516, 190, 83, 209}); 
	rooms.push_back({0, 300, 58, 100}); 
	rooms.push_back({58, 300, 175, 100}); 
	rooms.push_back({233, 300, 58, 100});
	
	const int WALL_THICKNESS = 4;
	for (auto room : rooms)
	{
		int x1 = room[0];
		int y1 = room[1];
		int x2 = room[2] + room[0];
		int y2 = room[3] + room[1];

		Vector2D startUpper{x1, y1};
		Vector2D endUpper{x2, y1+WALL_THICKNESS};

		Vector2D startLower{x1, y2-WALL_THICKNESS};
		Vector2D endLower{x2, y2+WALL_THICKNESS};

		Vector2D startLeft{x1, y1};
		Vector2D endLeft{x1+WALL_THICKNESS, y2};

		Vector2D startRight{x2-WALL_THICKNESS, y1};
		Vector2D endRight{x2, y2};

		GameObject* upperWall = new GameObject{startUpper, endUpper};
		GameObject* lowerWall = new GameObject{startLower, endLower};
		GameObject* leftWall  = new GameObject{startLeft , endLeft};
		GameObject* rightWall = new GameObject{startRight, endRight};
		
		upperWall->loadTexture(renderer);
		lowerWall->loadTexture(renderer);
		leftWall->loadTexture(renderer);
		rightWall->loadTexture(renderer);

		wallScene->addObj(upperWall);
		wallScene->addObj(lowerWall);
		wallScene->addObj(leftWall);
		wallScene->addObj(rightWall);
	}

	/*
	EX:
		
		x = 0, y = 0, w = 292, h = 300
		{0, 0, 292, 300}, => 	Upper wall: GameObject{x, y, w, y+WALL_THICKNESS}
								Lower wall: GameObject{x, h-WALL_THICKNESS, w, h+WALL_THICKNESS}
								Left wall:  GameObject{x, y, x+WALL_THICKNESS, h}
								Right wall: GameObject{w-WALL_THICKNESS, y, w, h}
	*/

	std::string result;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(64, SCREEN_HEIGHT-64);
    std::uniform_int_distribution<int> distribution2(-10, 10);
    std::uniform_int_distribution<int> distribution3(1, 10);

	int xVel = distribution2(generator);
	int yVel = distribution2(generator);

	for (auto const& [key, val] : furniture)
	{
    	std::uniform_int_distribution<int> distribution4(1, val.maxAmount);
		int amount = distribution4(generator);

		for(int i = 0; i < amount; i++)
		{
			PhysicsObject* pObj = new PhysicsObject{{distribution(generator), distribution(generator)}, {32, 32}, {xVel, yVel}, key, "./resources/furniture/" + key + ".png", val.weight};
			pObj->loadTexture(renderer);
			pObj->setRotation(getRandomAngle());
			//pObj->debugMode = true;
			boxScene->addObj(pObj);
		}
	}

	/*
	wall.loadTexture(renderer);
	wall2.loadTexture(renderer);
	wallScene->addObj(&wall);
	wallScene->addObj(&wall2);
	*/
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
					//obj->handleCollisions(wallScene->getObjs());
					obj->move();
					obj->rotate();
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
