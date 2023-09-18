#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include "./texture.h"
#include "./utils/vector2d.h"

class GameObject
{
    public:
        GameObject();

        Texture& getTexture();

        Vector2D& getPosition();
        Vector2D& getSize();
        Vector2D& getVelocity();

        void setTexture(Texture texture);
        bool loadTexture(SDL_Renderer* renderer, std::string path);

        void setPosition(Vector2D position);
        void setVelocity(Vector2D velocity);
        void setSize(Vector2D scale);

        void render(SDL_Renderer* renderer);

        SDL_Rect toBox();

        bool hasCollision(GameObject& other);

    protected:

    private:
        Texture texture;
        std::string texturePath;

        Vector2D position;
        Vector2D velocity;
        Vector2D size;
};

#endif // GAMEOBJECT_H