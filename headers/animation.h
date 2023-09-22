#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "./texture.h"
#include "./utils/vector2d.h"

class Animation
{
    public:
        Animation();

        bool loadTexture(SDL_Renderer* gRenderer, std::string path);

        Texture& getTexture();

        void addFrame(SDL_Rect frame);

		void render( Vector2D position, SDL_Renderer* gRenderer );

        SDL_Rect& getCurrentFrame();

        void tick();

    protected:

    private:
        Texture sheetTexture;
        
        int frameAmount;
        int skipFrames = 8;
        int frame;
        
        std::vector<SDL_Rect> frames;

        Vector2D size;
};

#endif // ANIMATION_H