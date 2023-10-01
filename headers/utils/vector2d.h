#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>

class Vector2D {
    public:
        Vector2D();
        Vector2D(float x, float y);

        float getX() const;
        float getY() const;

        void setX(float x);
        void setY(float y);

        void set(float x, float y);

        void increaseX(float amount);
        void increaseY(float amount);

        void decreaseX(float amount);
        void decreaseY(float amount);

        Vector2D operator+(const Vector2D& v2) const;
        friend Vector2D& operator+=(Vector2D& v1, const Vector2D& v2);

        Vector2D operator-(const Vector2D& v2) const;
        friend Vector2D& operator-=(Vector2D& v1, const Vector2D& v2);

        Vector2D operator*(float scalar);
        Vector2D& operator*=(float scalar);

        Vector2D operator/(float scalar);
        Vector2D& operator/=(float scalar);

        Vector2D getNormalizedVector();

    private:
        float x;
        float y;
};

#endif // VECTOR2D_H