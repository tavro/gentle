#ifndef PARTICLE_H
#define PARTICLE_H

#include "./texture.h"

class Particle
{
	public:
		Particle( int x, int y );

		void render();

		bool isDead();

	private:
		int posX, posY;
		int frame;

		Texture *texture;
};

#endif // PARTICLE_H