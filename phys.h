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

    virtual ~Phys() {}

    void integrate(double dt);
    virtual void apply_force(Vec f, Vec p);
    Vec ego_point_vel(Vec p);

    Vec ego_to_world(Vec p);
    Vec world_to_ego(Vec p);

    virtual bool is_internal(WPos) const = 0;


    Vec pos = Vec(0,0), vel = Vec(0,0), acc = Vec(0,0);
    double angle = 0.0, ang_vel = 0.0, ang_acc = 0.0;
    double mass = 1.0, mi = 1.0; // mi is the moment of inertia
    double elasticity = 1.0;

    std::string name;
    PhysType type = PhysType::Phys;

    double radius = 1.0;
    bool is_static = false, is_physical = false;
};

#endif // PHYS_H
