#include "circle.h"

void Circle::render(QPainter &painter, const Camera &cam)
{
    painter.setPen(strokeColor);
    painter.setBrush(QBrush(fillColor));
    Vec screenpos = cam.convertToCameraCoords(pos);
    painter.drawEllipse(screenpos.x - radius*cam.zoom, screenpos.y - radius*cam.zoom,
                        2*radius*cam.zoom, 2*radius*cam.zoom);
}

bool Circle::isInternal(WPos p)
{
    return (Vec(p.x, p.y)).length() <= radius;
}

bool Circle::intersecting(Circle &c)
{
    double dist = (pos - c.pos).length();
    if (dist < radius + c.radius) return true;
    return false;
}

bool Circle::colliding(Circle &c)
{
    double dist = (pos - c.pos).length();
    double diam = radius + c.radius;

    if (fabs(dist - diam) < EPSILON) return true;

    return false;
}


WPos Circle::collisionPoint(Circle &c) {
    Vec uvec = (c.pos - pos).unit();

    return pos.wpos() + uvec*radius;
}


void Circle::collide(Circle &c, double dt) {
    if (!this->intersecting(c)) return;

    double col_time = dt;
    int max_depth = 20;
    int depth = 1;
    double s_dt = dt;

    while (!this->colliding(c) && depth < max_depth)
    {
        s_dt = dt/pow(2.0, depth);
        ++depth;

        if (this->intersecting(c)) {
            col_time -= s_dt;
            this->integrate(-s_dt);
            c.integrate(-s_dt);
        }
        else if (!this->colliding(c)) {
            col_time += s_dt;
            this->integrate(s_dt);
            c.integrate(s_dt);
        }
    }

    // unit normal vector at collision point (extends between centres)
    Vec col_normal = (c.pos - this->pos).unit();

    // Components of relative velocity along the collision normal
    Vec col_rel_vel = col_normal * (c.vel - this->vel).dot(col_normal);

    // magnitude of momentum change to apply
    double dm = -2 * (col_rel_vel).dot(col_normal) / (1/this->mass + 1/c.mass);

    // update velocities
    this->vel = this->vel - col_normal*(dm/this->mass);
    c.vel = c.vel + col_normal*(dm/c.mass);

    this->integrate(dt - s_dt);
    c.integrate(dt - s_dt);

}
