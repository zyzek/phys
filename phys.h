#ifndef PHYS_H
#define PHYS_H


#include <string>

#include "vec.h"
#include "camera.h"

class Camera;

enum class PhysType {Phys, Circle, Polygon};

class Phys
{
public:
    Phys() {}
    Phys(Vec p):
        pos(p) {}
    Phys(Vec p, PhysType t):
        pos(p), type(t) {}
    Phys(Vec p, PhysType t, double r):
        pos(p), type(t), radius(r) {}



    void integrate(double dt);
    virtual void applyForce(Vec f, Vec p);
    Vec egoPointVel(Vec p);

    Vec egoToWorld(Vec p);
    Vec worldToEgo(Vec p);

    virtual bool isInternal(WPos) = 0;

    Vec pos = Vec(0,0), vel = Vec(0,0), acc = Vec(0,0);
    double angle = 0.0, angVel = 0.0, angAcc = 0.0;
    double mass = 1.0, mi = 1.0; // mi is the moment of inertia
    double bounciness = 1.0;
    double radius = 1.0;
    bool is_static = false;

    std::string name;
    PhysType type = PhysType::Phys;
};

#endif // PHYS_H
