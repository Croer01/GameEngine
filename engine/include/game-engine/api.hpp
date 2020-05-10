//
// Created by adria on 21/01/2019.
//

#ifndef SPACEINVADERS_API_HPP
#define SPACEINVADERS_API_HPP

#ifdef WIN32
#  define PUBLICAPI __declspec( dllexport )
#else
#  define PUBLICAPI
#endif

#include <cmath>
#include <cassert>

namespace GameEngine {

//TODO: Move Vec2D into math header
struct PUBLICAPI Vec2D {
    float x;
    float y;

    Vec2D() : x(0), y(0){};
    Vec2D(float x, float y) : x(x), y(y){};

    float sqrMagnitude() const{
        return x*x + y*y;
    }


    float magnitude() const{
        return std::sqrt(sqrMagnitude());
    }

    Vec2D operator+(const Vec2D &other) const {
        return Vec2D(x + other.x, y + other.y);
    };

    Vec2D operator+=(const Vec2D &other){
        x += other.x;
        y += other.y;
        return *this;
    };

    Vec2D operator-(const Vec2D &other) const {
        return Vec2D(x - other.x, y - other.y);
    };

    Vec2D operator-=(const Vec2D &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    };

    Vec2D operator*(const Vec2D &other) const {
        return Vec2D(x * other.x, y * other.y);
    };

    Vec2D operator*=(const Vec2D &other) {
        x *= other.x;
        y *= other.y;
        return *this;
    };

    Vec2D operator*(const int &other) const {
        return Vec2D(x * other, y * other);
    };

    Vec2D operator*=(const int &other) {
        x *= other;
        y *= other;
        return *this;
    };

    Vec2D operator*(const float &other) const {
        return Vec2D(x * other, y * other);
    };

    Vec2D operator*=(const float &other) {
        x *= other;
        y *= other;
        return *this;
    };

    bool operator==(const Vec2D &rhs) const {
        return x == rhs.x &&
               y == rhs.y;
    }

    bool operator!=(const Vec2D &rhs) const {
        return !(rhs == *this);
    }

    void normalize() {
        if(x != 0 || y != 0) {
            float m = magnitude();
            x /= m;
            y /= m;
        }
    }
};

struct PUBLICAPI geColor{
    float r;
    float g;
    float b;
    geColor() : r(0),g(0),b(0){};
    geColor(float greyValue) : r(greyValue),g(greyValue),b(greyValue){};
    geColor(float rValue, float gValue, float bValue) : r(rValue),g(gValue),b(bValue){};
};

struct PUBLICAPI Range{
    float min;
    float max;

    Range() : min(0.f), max(1.f){};
    Range(float min, float max) : min(min), max(max) {};

    float normalizedToValue(float normalized)
    {
        assert(min <= max);
        assert(0 <= normalized && normalized <= 1);
        return min + (max - min) * normalized;
    }

    float valueToNormalized(float value)
    {
        assert(min <= max);
        assert(min <= value && value <= max);
        return (value - min) / (max - min);
    }
};
}

#endif //SPACEINVADERS_API_HPP
