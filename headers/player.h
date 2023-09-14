#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include "./tile.h"
#include "./particle.h"

class Player
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

		void render( SDL_Rect& camera, SDL_Rect* currentClip );

    private:
		Particle* particles[ TOTAL_PARTICLES ];

		void renderParticles();

		SDL_Rect mBox;

		int mVelX, mVelY;
};

#endif // PLAYER_H
