#include "phys.h"

#include <cmath>
#include <iostream>

#define REVOLUTION 6.283185

void Phys::integrate(double dt)
{
    vel = vel + (acc*dt);
    pos = pos + (vel*dt);

    angVel += angAcc*dt;
    angle = fmod(angle + angVel*dt, REVOLUTION);

    acc.set(0.0,0.0);
    angAcc = 0.0;
}

void Phys::applyForce(Vec f, Vec p)
{
    acc = acc + (f/mass);
    angAcc += p.perpdot(f.rotated(-angle))/mi;
}

Vec Phys::egoPointVel(Vec p)
{
    return (p.perpendicular()*angVel) +  vel;
}

Vec Phys::egoToWorld(Vec p)
{
    return p.rotated(angle) + pos;
}

Vec Phys::worldToEgo(Vec p)
{
    return (p - pos).rotated(-angle);
}
