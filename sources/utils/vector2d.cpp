#include "../../headers/utils/vector2d.h"

Vector2D::Vector2D()
{
    x = 0;
    y = 0;
}

Vector2D::Vector2D(float x, float y)
{
    this->x = x;
    this->y = y;
}

float Vector2D::getX()
{
    return x;
}

float Vector2D::getY()
{
    return y;
}

void Vector2D::setX(float x)
{
    this->x = x;
}

void Vector2D::setY(float y)
{
    this->y = y;
}

void Vector2D::set(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2D Vector2D::operator+(const Vector2D& v2) const
{
    return Vector2D(x + v2.x, y + v2.y);
}

Vector2D& operator+=(Vector2D& v1, const Vector2D& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;

    return v1;
}

Vector2D Vector2D::operator-(const Vector2D& v2) const
{
    return Vector2D(x - v2.x, y - v2.y);
}

Vector2D& operator-=(Vector2D& v1, const Vector2D& v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;

    return v1;
}

Vector2D Vector2D::operator*(float scalar)
{
    return Vector2D(x * scalar, y * scalar);
}

Vector2D& Vector2D::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;

    return *this;
}

Vector2D Vector2D::operator/(float scalar)
{
    return Vector2D(x / scalar, y / scalar);
}

Vector2D& Vector2D::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;

    return *this;
}

void Vector2D::increaseX(float amount)
{
    x += amount;
}

void Vector2D::increaseY(float amount)
{
    y += amount;
}

void Vector2D::decreaseX(float amount)
{
    x -= amount;
}

void Vector2D::decreaseY(float amount)
{
    y -= amount;
}