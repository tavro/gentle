#include "../headers/physics_object.h"

PhysicsObject::PhysicsObject(float m) : GameObject()
{
    mass = m;
}

PhysicsObject::PhysicsObject(int x, int y, int w, int h, float m) : GameObject(x, y, w, h)
{
    mass = m;
}

PhysicsObject::PhysicsObject(Vector2D pos, Vector2D s, Vector2D vel, std::string n, std::string path, float m) : GameObject(pos, s, vel, n, path)
{
    mass = m;
}

void PhysicsObject::setHasFriction(bool state)
{
    hasFriction = state;
}