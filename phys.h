#ifndef PHYS_H
#define PHYS_H

#define EPSILON 0.001

#include <string>

#include "vec.h"
#include "camera.h"

class Camera;

class Phys
{
public:
    Phys() {}
    Phys(Vec p):
        pos(p) { }

    void integrate(double dt);
    virtual void applyForce(Vec f, Vec p);
    Vec egoPointVel(Vec p);

    Vec egoToWorld(Vec p);
    Vec worldToEgo(Vec p);

    virtual bool isInternal(WPos) = 0;

    Vec pos = Vec(0,0), vel = Vec(0,0), acc = Vec(0,0);
    double angle = 0.0, angVel = 0.0, angAcc = 0.0;
    double mass = 1.0, mi = 1.0;

    std::string name;
};

#endif // PHYS_H
