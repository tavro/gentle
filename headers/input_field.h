#ifndef INPUT_FIELD_H
#define INPUT_FIELD_H

#include "./text.h"
#include "./ui_state_object.h"

class InputField : public UIStateObject
{
    public:
        InputField( int x, int y, int w, int h, int max );

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

        Text& getText();
        
        void setText(std::string content);

        void appendToText(std::string content);

        void updateText(std::string content);

        void removeChar();

        void loadTextTexture(SDL_Renderer* renderer);

        std::string getContent();

    private:
        Text text{" ", 0, 0};
        
        bool hasUpdate;

        std::string textStr;
        int maxCharacters;
};

#endif // INPUT_FIELD_H