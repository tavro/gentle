#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include "./tile.h"
#include "./particle.h"
#include "./texture.h"

const int LEVEL_WIDTH = 512 + 128;
const int LEVEL_HEIGHT = 384 + 96;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Player : public GameObject
{
    public:
		static const int WIDTH = 20;
		static const int HEIGHT = 20;
		static const int VELOCITY = 10;
        static const int TOTAL_PARTICLES = 20;

		Player();
		~Player();

		void handleEvent( SDL_Event& e );

		void move( Tile *tiles[] );

		void setCamera( SDL_Rect& camera );

		void render( SDL_Rect& camera, SDL_Rect* currentClip, SDL_Renderer* gRenderer );

		Particle* particles[ TOTAL_PARTICLES ];
    private:

		void renderParticles(SDL_Renderer* gRenderer);
};

#endif // PLAYER_H
