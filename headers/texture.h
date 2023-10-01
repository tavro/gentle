#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <string>

class Texture
{
	public:
		Texture();
		~Texture();

		bool loadFromFile( std::string path, SDL_Renderer* gRenderer );
		
		#if defined(SDL_TTF_MAJOR_VERSION)
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor, SDL_Renderer* gRenderer, TTF_Font* gFont );
		#endif

		void free();

		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		void setBlendMode( SDL_BlendMode blending );

		void setAlpha( Uint8 alpha );
		
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Renderer* gRenderer = NULL);

		int getWidth();
		int getHeight();

		void setWidth(int w);
		void setHeight(int h);

	private:
		SDL_Texture* texture;

		int width;
		int height;
};

#endif // TEXTURE_H