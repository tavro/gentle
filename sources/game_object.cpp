#include "../headers/game_object.h"

#include <iostream>
#include <string>
#include <random>

std::string getRandomName(int length) {
    static const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string result;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, charset.size() - 1);

    for (int i = 0; i < length; ++i) {
        result += charset[distribution(generator)];
    }

    return result;
}

GameObject::GameObject()
{
    name = getRandomName(8);

    position.setX(0);
    position.setY(0);

    velocity.setX(0);
    velocity.setY(0);

    size.setX(0);
    size.setY(0);
}

GameObject::GameObject(int x, int y, int w, int h)
{
    name = getRandomName(8);

    position.setX(x);
    position.setY(y);

    velocity.setX(0);
    velocity.setY(0);

    size.setX(w);
    size.setY(h);
}

GameObject::GameObject(Vector2D pos, Vector2D s, Vector2D vel, std::string n, std::string path)
{
    position = pos;
    size = s;
    velocity = vel;
    name = n;
    texturePath = path;
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

void GameObject::setTexture(Texture texture)
{
    this->texture = texture;
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

bool GameObject::loadTexture(SDL_Renderer* renderer, std::string path)
{
    return texture.loadFromFile(path, renderer);
}

bool GameObject::loadTexture(SDL_Renderer* renderer)
{
    return texture.loadFromFile(texturePath, renderer);
}

std::string GameObject::getName()
{
    return name;
}

void GameObject::render(SDL_Renderer* renderer)
{
    texture.render( position.getX(), position.getY(), NULL, rotation, NULL, SDL_FLIP_NONE, renderer );
}

void GameObject::setTexturePath(std::string path)
{
    texturePath = path;
}

bool GameObject::isInside(int x, int y)
{
    bool inBoundX = x <= (getPosition().getX() + getSize().getX()) && x >= getPosition().getX();
    bool inBoundY = y <= (getPosition().getY() + getSize().getY()) && y >= getPosition().getY();
    return inBoundX && inBoundY;
}

bool GameObject::isToggled()
{
    return toggle;
}

void GameObject::setToggle(bool state)
{
    toggle = state;
}

void GameObject::setCurrentState( State state )
{
    currentState = state;
}

State GameObject::getCurrentState()
{
    return currentState;
}

void GameObject::setRotation(float value)
{
    rotation = value;
}

void GameObject::increaseRotation(float amount)
{
    rotation+=amount;
    rotation = fmod(rotation, 360.0);
}

void GameObject::decreaseRotation(float amount)
{
    rotation-=amount;
    rotation = fmod(rotation, 360.0);
}