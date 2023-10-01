#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <cmath>
#include <iostream>
#include "./game_object.h"
#include "./utils/constants.h"
#include <random>
#include <functional>

class PhysicsObject : public GameObject
{
    public:
        PhysicsObject(float m);
        PhysicsObject(int x, int y, int w, int h, float m);
        PhysicsObject(Vector2D pos, Vector2D s, Vector2D vel, std::string n, std::string path, float m);

        void rotate() override
        {
            if(rotationDirection != RotDir::NONE)
            {
                float localVel = rotationSpeed*rotationSpeed;
                float friction = localVel * 0.0025;

                rotationSpeed-=friction;

                if(rotationSpeed <= 0.25 && rotationSpeed >= -0.25)
                {
                    rotationSpeed = 0;
                    setRotationDirection(RotDir::NONE);
                }

                switch(rotationDirection)
                {
                    case RotDir::RIGHT:
                        increaseRotation(rotationSpeed);
                    break;
                    case RotDir::LEFT:
                        decreaseRotation(rotationSpeed);
                    break;
                }
            }
        }

        void move() override
        {
            if(hasFriction) 
            {
                float localVel = getVelocity().getX()*getVelocity().getX() + getVelocity().getY()*getVelocity().getY();
                Vector2D friction = (getVelocity().getNormalizedVector() * -1) * (float)(localVel * 0.0025);

                acceleration.set(friction.getX()*mass, friction.getY()*mass);

                getVelocity().increaseX(acceleration.getX());
                getVelocity().increaseY(acceleration.getY());
            }

            getPosition().increaseX(getVelocity().getX());
            getPosition().increaseY(getVelocity().getY());

            if(getPosition().getX() >= SCREEN_WIDTH-getSize().getX())
            {
                getVelocity().setX(getVelocity().getX()*-1);
            }

            if(getPosition().getX() <= 0)
            {
                getVelocity().setX(getVelocity().getX()*-1);
            }

            if(getPosition().getY() >= SCREEN_HEIGHT-getSize().getY())
            {
                getVelocity().setY(getVelocity().getY()*-1);
            }

            if(getPosition().getY() <= 0)
            {
                getVelocity().setY(getVelocity().getY()*-1);
            }

            if(getVelocity().getX() <= 0.1 && getVelocity().getX() >= -0.1)
            {
                getVelocity().setX(0);
            }

            if(getVelocity().getY() <= 0.1 && getVelocity().getY() >= -0.1) 
            {
                getVelocity().setY(0);
            }
        }

        void handleCollisions(std::vector<GameObject*> others) override
        {
            if(isMoving())
            {
                for (auto* other: others)
                {
                    if(other != this)
                    {
                        if(hasCollision(other->getCorners()))
                        {
                            other->getVelocity().set(getVelocity().getX(), getVelocity().getY());
                            auto gen = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());
                            bool b = gen();

                            if(b)
                            {
                                other->setRotationDirection(RotDir::LEFT);
                                other->setRotationSpeed(1);
                            }
                            else
                            {
                                other->setRotationDirection(RotDir::RIGHT);
                                other->setRotationSpeed(1);
                            }
                            
                            getVelocity()*=-1;
                        }
                    }
                }
            }
        }

        void setHasFriction(bool state);

    private:
        Vector2D acceleration{0, 0};

        bool hasFriction = true;
        
        float mass;
};

#endif