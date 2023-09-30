#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include "./texture.h"
#include "./utils/vector2d.h"

enum State
{
    MOUSE_OUT         = 0,
    MOUSE_OVER_MOTION = 1,
    MOUSE_DOWN        = 2,
    MOUSE_UP          = 3,
    TOTAL             = 4
}; 

class GameObject
{
    public:
        GameObject();
        GameObject(int x, int y, int w, int h);
        GameObject(Vector2D pos, Vector2D s, Vector2D vel, std::string n, std::string path);

        Texture& getTexture();

        Vector2D& getPosition();
        Vector2D& getSize();
        Vector2D& getVelocity();

        virtual void handleEvent(SDL_Event* e)
        {
            if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
            {
                int x, y;
                SDL_GetMouseState( &x, &y );

                bool inside = true;

                // Mouse is to the left
                if( x < getPosition().getX() )
                {
                    inside = false;
                }
                // Mouse is to the right
                else if( x > getPosition().getX() + getSize().getX() )
                {
                    inside = false;
                }
                // Mouse above
                else if( y < getPosition().getY() )
                {
                    inside = false;
                }
                // Mouse below
                else if( y > getPosition().getY() + getSize().getY() )
                {
                    inside = false;
                }

                // Mouse is outside
                if( !inside )
                {
                    currentState = MOUSE_OUT;
                }
                // Mouse is inside
                else
                {
                    switch( e->type )
                    {
                        case SDL_MOUSEMOTION:
                        currentState = MOUSE_OVER_MOTION;
                        break;
                    
                        case SDL_MOUSEBUTTONDOWN:
                        currentState = MOUSE_DOWN;
                        toggle = !toggle;
                        break;
                        
                        case SDL_MOUSEBUTTONUP:
                        currentState = MOUSE_UP;
                        break;
                    }
                }
            }
        }

        void setTexture(Texture texture);
        void setTexturePath(std::string path);

        bool loadTexture(SDL_Renderer* renderer, std::string path);
        bool loadTexture(SDL_Renderer* renderer);

        void freeTexture();

        void setPosition(Vector2D position);
        void setVelocity(Vector2D velocity);
        void setSize(Vector2D scale);

        void render(SDL_Renderer* renderer);

        SDL_Rect toBox();

        bool hasCollision(GameObject& other);

        bool isInside(int x, int y);
        
        bool isToggled();

        void setToggle(bool state);

        void setCurrentState( State state );

        State getCurrentState();
        
        std::string getName();

        void setRotation(float value);

        void increaseRotation(float amount);

        void decreaseRotation(float amount);

        virtual void move()
        {
            getPosition().increaseX(getVelocity().getX());
            getPosition().increaseY(getVelocity().getY());
        }

        virtual void setHasFriction(bool state)
        {
            
        }

        virtual void handleCollisions(std::vector<GameObject*> others)
        {

        }

        bool isMoving();

    protected:

    private:
		State currentState;
        bool toggle = false;

        Texture texture;
        std::string texturePath;

        std::string name;

        Vector2D position;
        Vector2D velocity;
        Vector2D size;

        float rotation;
};

#endif // GAMEOBJECT_H