#include "phys.h"

#include <cmath>

#define REVOLUTION 6.283185

void Phys::integrate(double dt)
{
    vel = vel + (acc*dt);
    pos = pos + (vel*dt);

    ang_vel += ang_acc*dt;
    angle = fmod(angle + ang_vel*dt, REVOLUTION);

    acc.set(0.0,0.0);
    ang_acc = 0.0;
}

void Phys::apply_force(Vec f, Vec p)
{
    if (is_static) {
        return;
    }

    acc = acc + (f/mass);
    ang_acc += p.perp_dot(f.rotated(-angle))/mi;
}

Vec Phys::ego_point_vel(Vec p)
{
    return (p.perpendicular()*ang_vel) +  vel;
}

Vec Phys::ego_to_world(Vec p)
{
    return p.rotated(angle) + pos;
}

Vec Phys::world_to_ego(Vec p)
{
    return (p - pos).rotated(-angle);
}
