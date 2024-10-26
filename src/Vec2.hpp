#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>


template <typename T>
class Vec2 
{
public:
    T x = 0;
    T y = 0;

    Vec2() = default; // this mark when creating a vector it will use this constructor as the default method of creating it.

    Vec2(T xIn, T yIn) : x(xIn), y(yIn) { }

    // constructor to convert from sf::Vector2
    Vec2(const sf::Vector2<T>& vec): x(vec.x), y(vec.y) { }

    // allow automatic conversion to sf::Vector2
    // this lets us pass our Vec2 into sfml functions
    operator sf::Vector2<T>()
    {
        return sf::Vector2<T>(x, y);
    }

    Vec2 operator + (const Vec2& rhs) const
    {
        return Vec2(x+rhs.x, y+rhs.y);
    }

    Vec2 operator - (const Vec2& rhs) const
    {
        return Vec2(x-rhs.x, y-rhs.y);
    }

    Vec2 operator / (const T val) const
    {
        if (val == 0)
        {
            throw std::runtime_error("Division by zero in Vec2");
        }

        return Vec2(x/val, y/val);
    }

    Vec2 operator * (const T val) const
    {
        return Vec2(x*val, y*val);
    }

    bool operator == (const Vec2& rhs) const
    {
        if (x == rhs.x && y == rhs.y)
        {
            return true;
        }

        return false;
    }

    bool operator != (const Vec2& rhs) const
    {

        return !(*this==rhs);
    }

    void operator += (const Vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
    }

    void operator -= (const Vec2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
    }

    void operator *= (const T val)
    {
        x *= val;
        y *= val;
    }

    void operator /= (const T val)
    {
        if (val == 0)
        {
            throw std::runtime_error("Division by zero in Vec2");
        }
        x /= val;
        y /= val;
    }

    float dist(const Vec2& rhs) const
    {
        Vec2 delta = rhs - *this;
        return sqrt((delta.x * delta.x) + (delta.y * delta.y));
    }

    float magnitude()
    {
        return sqrt((x*x) + (y*y));
    }

    Vec2 normalize()
    {
        if (magnitude() == 0)
        {
            return Vec2(0, 0);
        }
        return Vec2(x / magnitude(), y / magnitude());
    }

};

using Vec2f = Vec2<float>;  // similar to a macro a way to aliases a Vec2f float easily.
