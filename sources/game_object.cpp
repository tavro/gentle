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

GameObject::GameObject(Vector2D topLeft, Vector2D bottomRight)
{
    name = getRandomName(8);

    position.setX(topLeft.getX());
    position.setY(topLeft.getY());

    velocity.setX(0);
    velocity.setY(0);

    size.setX(bottomRight.getX()-topLeft.getX());
    size.setY(bottomRight.getY()-topLeft.getY());

    texturePath = "./resources/black.png";
    isStatic = true;
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

std::vector<Vector2D> GameObject::getCorners()
{
    std::vector<Vector2D> corners;

    float halfWidth = size.getX() / 2;
    float halfHeight = size.getY() / 2;

    float radians = rotation * (M_PI / 180.0);
    float cosine = std::cos(radians);
    float sine = std::sin(radians);

    float xOffset = position.getX() + halfWidth;
    float yOffset = position.getY() + halfHeight;

    Vector2D topLeft{
        cosine * (-halfWidth) - sine * (-halfHeight) + xOffset,
        cosine * (-halfHeight) + sine * (-halfWidth) + yOffset
    };

    Vector2D topRight{
        cosine * (halfWidth) - sine * (-halfHeight) + xOffset,
        cosine * (-halfHeight) + sine * (halfWidth) + yOffset
    };

    Vector2D bottomLeft{
        cosine * (-halfWidth) - sine * (halfHeight) + xOffset,
        cosine * (halfHeight) + sine * (-halfWidth) + yOffset
    };

    Vector2D bottomRight{
        cosine * (halfWidth) - sine * (halfHeight) + xOffset,
        cosine * (halfHeight) + sine * (halfWidth) + yOffset
    };

    corners.push_back(topLeft);
    corners.push_back(topRight);
    corners.push_back(bottomLeft);
    corners.push_back(bottomRight);

    return corners;
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

bool GameObject::hasCollision(std::vector<Vector2D> otherCorners)
{
    std::vector<Vector2D> corners = getCorners();

    Vector2D offset = getPosition() + getSize() / 2;

    // 0. Interpolate between points // TODO: add more points betwen otherCorners
    std::vector<Vector2D> topOthers();
    std::vector<Vector2D> bottomOthers();

    for (float i = 0.0f; i <= 1.0f; i += 0.1f)
    {
        Vector2D topLeftOther = otherCorners[0];
        Vector2D topRightOther = otherCorners[1];
    }

    // 1. Remove offset from all corners
    for (int idx = 0; idx < 4; idx++)
    {
        corners[idx] -= offset;
        otherCorners[idx] -= offset;
    } 

    // 2. Rotate all corners around origo by negative this->rotation
    float radians = rotation * (M_PI / 180.0);
    float cosine = std::cos(-radians);
    float sine = std::sin(-radians);

    for (int idx = 0; idx < 4; idx++)
    {
        float cornerX = corners[idx].getX();
        float cornerY = corners[idx].getY();

        corners[idx] = {
            cosine * cornerX - sine * cornerY,
            cosine * cornerY + sine * cornerX
        };

        float otherCornerX = otherCorners[idx].getX();
        float otherCornerY = otherCorners[idx].getY();

        otherCorners[idx] = {
            cosine * otherCornerX - sine * otherCornerY,
            cosine * otherCornerY + sine * otherCornerX
        };
    }

    const float TOP_LEFT = 0;
    const float BOTTOM_RIGHT = 3;
    float minX = corners[TOP_LEFT].getX();
    float minY = corners[TOP_LEFT].getY();
    float maxX = corners[BOTTOM_RIGHT].getX();
    float maxY = corners[BOTTOM_RIGHT].getY();

    for (Vector2D currOther : otherCorners)
    {
        // 3. Check if other corner is within the bound of this object's corners
        if (currOther.getX() < maxX && currOther.getX() > minX && currOther.getY() < maxY && currOther.getY() > minY)
            return true;
    }

    return false;

    /* Original code
    for (int edge = 0; edge < 4; ++edge) {
        Vector2D axis(-corners[edge].getY() + corners[(edge + 1) % 4].getY(), 
                      corners[edge].getX() - corners[(edge + 1) % 4].getX());

        float minA = INFINITY, maxA = -INFINITY;
        float minB = INFINITY, maxB = -INFINITY;

        for (const auto& corner : corners) {
            float projection = corner.getX() * axis.getX() + corner.getY() * axis.getY();
            minA = std::min(minA, projection);
            maxA = std::max(maxA, projection);
        }

        for (const auto& corner : otherCorners) {
            float projection = corner.getX() * axis.getX() + corner.getY() * axis.getY();
            minB = std::min(minB, projection);
            maxB = std::max(maxB, projection);
        }

        if (maxA < minB || maxB < minA) {
            return false;
        }
    }

    return true;
    */
}

bool GameObject::loadTexture(SDL_Renderer* renderer, std::string path)
{
    bool loaded = texture.loadFromFile(path, renderer);
    if(!isStatic)
    {
        size.setX(texture.getWidth());
        size.setY(texture.getHeight());
    }
    return loaded;
}

bool GameObject::loadTexture(SDL_Renderer* renderer)
{
    debugTexture.loadFromFile("./resources/debug.png", renderer);

    bool loaded = texture.loadFromFile(texturePath, renderer);
    if(!isStatic)
    {
        size.setX(texture.getWidth());
        size.setY(texture.getHeight());
    }
    return loaded;
}

std::string GameObject::getName()
{
    return name;
}

void GameObject::render(SDL_Renderer* renderer)
{
    if(isStatic)
    {
        texture.setWidth(size.getX()) ;
        texture.setHeight(size.getY());
    }
    texture.render( position.getX(), position.getY(), NULL, rotation, NULL, SDL_FLIP_NONE, renderer );

    if(debugMode)
    {
        for (auto corner: getCorners())
        {
            debugTexture.render( corner.getX()-2, corner.getY()-2, NULL, 0.0, NULL, SDL_FLIP_NONE, renderer );
        }
    }
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

bool GameObject::isMoving()
{
    return (getVelocity().getX() != 0 || getVelocity().getY() != 0);
}

void GameObject::setRotationDirection(RotDir dir)
{
    rotationDirection = dir;
}

void GameObject::setRotationSpeed(float speed)
{
    rotationSpeed = speed;
}
