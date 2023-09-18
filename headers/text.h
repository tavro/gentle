#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include "ui_object.h"

class Text : public UIObject
{
    public:
        Text();

    private:
        TTF_Font* font;
        std::string content;
};

#endif // TEXT_H