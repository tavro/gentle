#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include "./texture.h"
#include "./ui_state_object.h"
#include "./text.h"

const int BUTTON_WIDTH = 96;
const int BUTTON_HEIGHT = 32;

class Button : public UIStateObject
{
	public:
		Button(std::string content, int x, int y);
		
		Text& getText();
        
        void setText(std::string content);
        
		void loadTextTexture(SDL_Renderer* renderer);

		void render(SDL_Renderer* renderer) override
        {
			// TODO: Set width based on text amount => getSpriteClip(getCurrentState()).w = BUTTON_WIDTH * 2;
			// Do this for InputField as well
            getTexture().render( getPosition().getX(), getPosition().getY(), &getSpriteClip(getCurrentState()), 0.0, NULL, SDL_FLIP_NONE, renderer );
			text.loadTexture(renderer);
            text.render(renderer);
        }

		void loadSpriteSheet(std::string path, SDL_Renderer* renderer);

	private:
		Text text{" ", 0, 0};
};

#endif // BUTTON_H
