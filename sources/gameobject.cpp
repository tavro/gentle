#include "../headers/gameobject.h"

GameObject::GameObject()
{
    position.setX(0);
    position.setY(0);

    velocity.setX(0);
    velocity.setY(0);

    size.setX(0);
    size.setY(0);
}

Vector2D& GameObject::getPosition()
{
    return position;
}

Vector2D& GameObject::getVelocity()
{
    return velocity;
}

Vector2D& GameObject::getSize()
{
    return size;
}

void GameObject::setPosition(Vector2D position)
{
    this->position = position;
}

void GameObject::setVelocity(Vector2D velocity)
{
    this->velocity = velocity;
}

void GameObject::setSize(Vector2D size)
{
    this->size = size;
}

SDL_Rect GameObject::toBox()
{
    SDL_Rect box = { position.getX(), position.getY(), size.getX(), size.getY() };
    return box;
}

Texture& GameObject::getTexture()
{
    return texture;
}

bool GameObject::hasCollision(GameObject& other)
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = getPosition().getX();
    rightA = getPosition().getX() + getSize().getX();
    topA = getPosition().getY();
    bottomA = getPosition().getY() + getSize().getY();

    leftB = other.getPosition().getX();
    rightB = other.getPosition().getX() + other.getSize().getX();
    topB = other.getPosition().getY();
    bottomB = other.getPosition().getY() + other.getSize().getY();

    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    return true;
}