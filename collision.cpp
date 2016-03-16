#include <iostream>

#include "collision.h"
#include "phys.h"
#include "circle.h"
#include "polygon.h"
#include "wpos.h"


bool collision::intersecting (Phys& p1, Phys& p2) {

    // Check the bounding circles before doing potentially-complicated intersection checks.
    if (cc_intersecting((Circle&)p1, (Circle&)p2)){
        if (p1.type == PhysType::Circle) {
            if (p2.type == PhysType::Circle) {
                return true;
            }
            else {
                return cp_intersecting((Circle&)p1, (Polygon&)p2);
            }
        }
        else {
            if (p2.type == PhysType::Circle) {
                return cp_intersecting((Circle&)p2, (Polygon&)p1);
            }
            else {
                return pp_intersecting((Polygon&)p1, (Polygon&)p2);
            }
        }
    }

    return false;
}

bool collision::colliding (Phys &p1, Phys &p2) {
    if (p1.type == PhysType::Circle) {
        if (p2.type == PhysType::Circle) {
            return cc_colliding((Circle&)p1, (Circle&)p2);
        }
        else {
            return cp_colliding((Circle&)p1, (Polygon&)p2);
        }
    }
    else {
        if (p2.type == PhysType::Circle) {
            return cp_colliding((Circle&)p2, (Polygon&)p1);
        }
        else {
            return pp_colliding((Polygon&)p1, (Polygon&)p2);
        }
    }
}

void collision::collide (Phys &p1, Phys &p2, double dt) {
    if (p1.type == PhysType::Circle) {
        if (p2.type == PhysType::Circle) {
            cc_collide((Circle&)p1, (Circle&)p2, dt);
        }
        else {
            cp_collide((Circle&)p1, (Polygon&)p2, dt);
        }
    }
    else {
        if (p2.type == PhysType::Circle) {
            cp_collide((Circle&)p2, (Polygon&)p1, dt);
        }
        else {
            pp_collide((Polygon&)p1, (Polygon&)p2, dt);
        }
    }
}


bool collision::cc_intersecting (Circle &c1, Circle&c2) {
    return (c1.pos - c2.pos).length() < c1.radius + c2.radius;
}

bool collision::cp_intersecting (Circle &c, Polygon &p) {return false;}
bool collision::pp_intersecting (Polygon &p1, Polygon &p2) {
    for (WPos p : p1.verts) {
        if (p2.isInternal(p)) return true;
    }
    for (WPos p : p2.verts) {
        if (p1.isInternal(p)) return true;
    }
    return false;
}

bool collision::cc_colliding (Circle &c1, Circle &c2) {
    double dist = (c1.pos - c2.pos).length();
    double diam = c1.radius + c2.radius;
    return fabs(dist - diam) < EPSILON;
}

bool collision::cp_colliding (Circle &c, Polygon &p) {return false;}

bool collision::pp_colliding (Polygon &p1, Polygon &p2) {return false;}

void collision::cc_collide(Circle &c1, Circle &c2, double dt) {
    if (!cc_intersecting(c1, c2)) return;

    double col_time = dt;
    int max_depth = 20;
    int depth = 1;
    double s_dt = dt;

    while (!cc_colliding(c1, c2) && depth < max_depth)
    {
        s_dt = dt/pow(2.0, depth);
        ++depth;

        if (cc_intersecting(c1, c2)) {
            col_time -= s_dt;
            c1.integrate(-s_dt);
            c2.integrate(-s_dt);
        }
        else if (!cc_colliding(c1, c2)) {
            col_time += s_dt;
            c1.integrate(s_dt);
            c2.integrate(s_dt);
        }
    }

    // unit normal vector at collision point (extends between centres)
    Vec col_normal = (c2.pos - c1.pos).unit();

    // Components of relative velocity along the collision normal
    Vec col_rel_vel = col_normal * (c2.vel - c1.vel).dot(col_normal);

    // magnitude of momentum change to apply
    double dm = -(1 + c1.elasticity*c2.elasticity) * (col_rel_vel).dot(col_normal) / (1/c1.mass + 1/c2.mass);

    // update velocities
    c1.vel = c1.vel - col_normal*(dm/c1.mass);
    c2.vel = c2.vel + col_normal*(dm/c2.mass);

    c1.integrate(dt - s_dt);
    c2.integrate(dt - s_dt);
}

void collision::cp_collide(Circle &c, Polygon &p, double dt) {return;}
void collision::pp_collide(Polygon &p1, Polygon &p2, double dt) {return;}
