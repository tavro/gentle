#include "../headers/animation.h"
#include <iostream>

Animation::Animation() {
    frame = 0;
    frameAmount = 4;
    size = Vector2D(16, 22);

    for(int i = 0; i < frameAmount; i++) {
        addFrame({ (int)(i * size.getX()), 0, (int)size.getX(), (int)size.getY() });
    }
}

void Animation::tick() {
    frame++;
    if (frame / skipFrames >= frameAmount) {
        frame = 0;
    }
}

void Animation::addFrame(SDL_Rect frame) {
    frames.push_back(frame);
}

SDL_Rect& Animation::getCurrentFrame() {
    return frames[ frame / skipFrames ];
}

bool Animation::loadTexture(SDL_Renderer* gRenderer, std::string path) {
    return sheetTexture.loadFromFile(path, gRenderer);
}

Texture& Animation::getTexture() {
    return sheetTexture;
}

void Animation::render( Vector2D position, SDL_Renderer* gRenderer, float rotation )
{
    tick();
    sheetTexture.render( position.getX(), position.getY(), &getCurrentFrame(), 0.0, NULL, SDL_FLIP_NONE, gRenderer );
}