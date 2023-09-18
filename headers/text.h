#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ui_object.h"

class Text : public UIObject
{
    public:
        Text(std::string content, int x, int y);

        bool loadFont(std::string path, int size);

        bool loadTexture(SDL_Renderer* renderer);

        void updateContent(std::string content);

    private:
        TTF_Font* font;
        SDL_Color color;
        std::string content;
};

#endif // TEXT_H