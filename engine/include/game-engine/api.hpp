//
// Created by adria on 21/01/2019.
//

#ifndef SPACEINVADERS_API_HPP
#define SPACEINVADERS_API_HPP

#ifndef _WIN32
#  ifdef GAMEENGINE_EXPORTS
#    define PUBLICAPI __attribute__ ((visibility("default")))
#  else
#    define PUBLICAPI
#  endif
#else //msvc or gcc win32
#  ifdef GAMEENGINE_EXPORTS
#    define PUBLICAPI __declspec(dllexport)
#  else
#    define PUBLICAPI __declspec(dllimport)
#  endif
#endif



#include <cmath>

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

    Vec2D operator/(const int &other) const {
        return Vec2D(x / other, y / other);
    };

    Vec2D operator/=(const int &other) {
        x /= other;
        y /= other;
        return *this;
    };

    Vec2D operator/(const float &other) const {
        return Vec2D(x / other, y / other);
    };

    Vec2D operator/=(const float &other) {
        x /= other;
        y /= other;
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
    explicit geColor(float greyValue) : r(greyValue),g(greyValue),b(greyValue){};
    geColor(float rValue, float gValue, float bValue) : r(rValue),g(gValue),b(bValue){};
};

}

#endif //SPACEINVADERS_API_HPP
