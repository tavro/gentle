#ifndef PARTICLE_H
#define PARTICLE_H

#include "./texture.h"

class Particle
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
		int posX, posY;
		int frame;

		Texture *texture;
};

#endif // PARTICLE_H