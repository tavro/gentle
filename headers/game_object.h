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

        virtual void handleEvent(SDL_Event* e)
        {

        }

        void setTexture(Texture texture);

        bool loadTexture(SDL_Renderer* renderer, std::string path);

        void freeTexture();

        void setPosition(Vector2D position);
        void setVelocity(Vector2D velocity);
        void setSize(Vector2D scale);

        void render(SDL_Renderer* renderer);

        SDL_Rect toBox();

        bool hasCollision(GameObject& other);

        std::string getName();

    protected:

    private:
        Texture texture;
        std::string texturePath;

        std::string name;

        Vector2D position;
        Vector2D velocity;
        Vector2D size;
};

#endif // GAMEOBJECT_H