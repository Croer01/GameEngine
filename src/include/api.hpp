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

struct PUBLICAPI Vec3D {
    float x;
    float y;
    float z;

    Vec3D() : x(0), y(0), z(0){};
    Vec3D(float x, float y, float z) : x(x), y(y), z(z){};
};

class PUBLICAPI GameObject {
public:
    virtual ~GameObject() = 0;

    virtual Vec3D position() const = 0;

    virtual void position(const Vec3D &position) = 0;

    virtual Vec3D rotation() const = 0;

    virtual void rotation(const Vec3D &rotation) = 0;

    virtual Vec3D scale() const = 0;

    virtual void scale(const Vec3D &scale) = 0;

};
}

#endif //SPACEINVADERS_API_HPP
