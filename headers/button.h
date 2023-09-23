#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include "./texture.h"
#include "./ui_state_object.h"
#include "./text.h"

class Button : public UIStateObject
{
	public:
		Button(std::string content, int x, int y, int w, int h);
		Button(std::string content, int x, int y);
		
		Text& getText();
        
        void setText(std::string content);
        
		void loadTextTexture(SDL_Renderer* renderer);

		void render(SDL_Renderer* renderer) override
        {
			if(getSize().getX() <= MINIMUM_WIDTH)
			{
            	getTexture().render( getPosition().getX(), 	getPosition().getY(), &getSpriteClip(getCurrentState()), 0.0, NULL, SDL_FLIP_NONE, renderer );
            	getTexture().render( getPosition().getX()+BH, 	getPosition().getY(), &getSpriteClip(8 + getCurrentState()), 0.0, NULL, SDL_FLIP_NONE, renderer );
			}
			else
			{
				const int MIDDLE_WIDTH = getSize().getX() - MINIMUM_WIDTH;
				getSpriteClip(4 + getCurrentState()).w = MIDDLE_WIDTH;
            	getTexture().render( getPosition().getX(), 	getPosition().getY(), &getSpriteClip(getCurrentState()), 0.0, NULL, SDL_FLIP_NONE, renderer );
            	spriteSheet2.render( getPosition().getX()+BH, 		getPosition().getY(), &getSpriteClip(4 + getCurrentState()), 0.0, NULL, SDL_FLIP_NONE, renderer );
            	getTexture().render( getPosition().getX()+MIDDLE_WIDTH+BH, 	getPosition().getY(), &getSpriteClip(8 + getCurrentState()), 0.0, NULL, SDL_FLIP_NONE, renderer );
			}

			text.loadTexture(renderer);
            text.render(renderer);
        }

		void loadSpriteSheet(std::string path, SDL_Renderer* renderer);

	private:
		static const int BH = 32;
		static const int MINIMUM_WIDTH = BH*2;

		Texture spriteSheet2; // TODO: Fix this. This is just because I'm lazy and want to move foward.

		Text text{" ", 0, 0};
};

#endif // BUTTON_H
