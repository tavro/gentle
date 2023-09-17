#ifndef PARTICLE_H
#define PARTICLE_H

#include "./texture.h"
#include "../headers/game_object.h"

class Particle : public GameObject
{
	public:
		Particle( int x, int y );

		void render(SDL_Renderer* gRenderer);

		bool isDead();

        Texture redTexture;
        Texture greenTexture;
        Texture blueTexture;
        Texture shimmerTexture;

	private:
		int frame;
};

#endif // PARTICLE_H