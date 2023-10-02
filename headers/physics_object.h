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
                float velSqrMagnitude = getVelocity().getX()*getVelocity().getX() + getVelocity().getY()*getVelocity().getY();
                bool isNotMoving = velSqrMagnitude <= 1; // TODO: decide better value

                if (isNotMoving)
                {
                    acceleration.setX(-getVelocity().getX());
                    acceleration.setY(-getVelocity().getY());
                    getVelocity().increaseX(acceleration.getX());
                    getVelocity().increaseY(acceleration.getY());
                }
                else
                {
                    float fricCoefficient = 0.0025f * mass; // TODO: decide better value
                    float fricMagnitude = fricCoefficient * 9.82f;

                    Vector2D fricDirection = getVelocity().getNormalizedVector() * -1;
                    Vector2D friction = fricDirection * fricMagnitude;
                    acceleration.set(friction.getX(), friction.getY());

                    getVelocity().increaseX(acceleration.getX());
                    getVelocity().increaseY(acceleration.getY());
                }
            }

            Vector2D newPosition = getPosition() + getVelocity();

            if (newPosition.getX() <= 0)
            {
                float magnitude = abs(getVelocity().getX());
                getVelocity().setX(magnitude);
            }
            else if (newPosition.getX() >= SCREEN_WIDTH - getSize().getX())
            {
                float magnitude = abs(getVelocity().getX());
                getVelocity().setX(-magnitude);
            }

            if (newPosition.getY() <= 0)
            {
                float magnitude = abs(getVelocity().getY());
                getVelocity().setY(magnitude);
            }
            else if (newPosition.getY() >= SCREEN_HEIGHT - getSize().getY())
            {
                float magnitude = abs(getVelocity().getY());
                getVelocity().setY(-magnitude);
            }

            // TODO: adjust newPosition so that the object is not out of bounds

            setPosition(newPosition);

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