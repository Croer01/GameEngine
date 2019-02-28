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
namespace GameEngine {

//TODO: Move Vec2D into math header
struct PUBLICAPI Vec2D {
    float x;
    float y;

    Vec2D() : x(0), y(0){};
    Vec2D(float x, float y) : x(x), y(y){};

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
};
}

#endif //SPACEINVADERS_API_HPP
