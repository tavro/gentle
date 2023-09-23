#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include "./tile.h"
#include "./particle.h"
#include "./texture.h"

#include "./animation.h"

const int LEVEL_WIDTH = 640;// + 128;
const int LEVEL_HEIGHT = 480;// + 128;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Player : public GameObject
{
    public:
		static const int WIDTH = 20;
		static const int HEIGHT = 20;
		static const int VELOCITY = 1;
        static const int TOTAL_PARTICLES = 20;

		Player();
		~Player();

		void handleEvent( SDL_Event& e );

		void move( Tile *tiles[] );

		void setCamera( SDL_Rect& camera );

		void render( SDL_Rect& camera, SDL_Renderer* gRenderer );

		void addAnimation(std::string path, SDL_Renderer* renderer);
        
		Animation* getAnimation();

		Particle* particles[ TOTAL_PARTICLES ];
    private:
		std::vector<Animation*> animations;
        Animation* activeAnimation;

		bool playAnimation = false;

		void renderParticles(SDL_Renderer* gRenderer);
};

#endif // PLAYER_H
