#ifndef INPUT_FIELD_H
#define INPUT_FIELD_H

#include "./text.h"
#include "./ui_state_object.h"

class InputField : public UIStateObject
{
    public:
        InputField( int x, int y, int w, int h );

        void render(SDL_Renderer* renderer) override
        {
            getTexture().render( getPosition().getX(), getPosition().getY(), &getSpriteClip(getCurrentState()), 0.0, NULL, SDL_FLIP_NONE, renderer );
            if(hasUpdate) 
            {
                text.loadTexture(renderer);
                hasUpdate = false;
            }
            text.render(renderer);
        }

        void setText(std::string content);

        void updateText(std::string content);

        void loadTextTexture(SDL_Renderer* renderer);

        Text& getText();

    private:
        Text text{" ", 0, 0};
        bool isActive;
        bool hasUpdate;
};

#endif // INPUT_FIELD_H