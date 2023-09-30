#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <cmath>
#include <iostream>
#include "./game_object.h"

class PhysicsObject : public GameObject
{
    public:
        PhysicsObject(float m);
        PhysicsObject(int x, int y, int w, int h, float m);
        PhysicsObject(Vector2D pos, Vector2D s, Vector2D vel, std::string n, std::string path, float m);

        void move() override
        {
            if(hasFriction) 
            {
                float localVel = getVelocity().getX()*getVelocity().getX() + getVelocity().getY()*getVelocity().getY();
                Vector2D friction = (getVelocity().getNormalizedVector() * -1) * (float)(localVel * 0.25);

                acceleration.set(friction.getX()/mass, friction.getY()/mass);

                getVelocity().increaseX(acceleration.getX());
                getVelocity().increaseY(acceleration.getY());
            }

            getPosition().increaseX(getVelocity().getX());
            getPosition().increaseY(getVelocity().getY());

            if(getVelocity().getX() <= 0.1)
            {
                getVelocity().setX(0);
            }

            if(getVelocity().getY() <= 0.1) 
            {
                getVelocity().setY(0);
            }
        }

        void setHasFriction(bool state);

    private:
        Vector2D acceleration{0, 0};

        bool hasFriction = true;
        
        float mass;
};

#endif