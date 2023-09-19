#include "../headers/animation.h"
#include <iostream>

Animation::Animation() {
    frame = 0;
    frameAmount = 4;
    size = Vector2D(20, 20);

    for(int i = 0; i < frameAmount; i++) {
        addFrame({ (int)(i * size.getX()), 0, (int)size.getX(), (int)size.getY() });
    }
}

void Animation::tick() {
    frame++;
    if (frame / 4 >= frameAmount) {
        frame = 0;
    }
}

void Animation::addFrame(SDL_Rect frame) {
    frames.push_back(frame);
}

SDL_Rect& Animation::getCurrentFrame() {
    return frames[ frame / 4 ];
}

bool Animation::loadTexture(SDL_Renderer* gRenderer, std::string path) {
    return sheetTexture.loadFromFile(path, gRenderer);
}

Texture& Animation::getTexture() {
    return sheetTexture;
}

void Animation::render( Vector2D position, SDL_Renderer* gRenderer )
{
    tick();
    sheetTexture.render( position.getX(), position.getY(), &getCurrentFrame(), 0.0, NULL, SDL_FLIP_NONE, gRenderer );
}