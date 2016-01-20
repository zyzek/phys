#include "spring.h"

void Spring::apply()
{
    Vec ab = b->egoToWorld(bAttach) - a->egoToWorld(aAttach);
    double strength = springiness*(ab.length() - length);
    Vec force = ab.unit()*strength;

    a->applyForce(force, aAttach);
    b->applyForce(force*-1, bAttach);
}


void Spring::render(QPainter &painter, const Camera &cam)
{
    painter.setPen(strokeColor);
    Vec s1 = cam.convertToCameraCoords(a->egoToWorld(aAttach));
    Vec s2 = cam.convertToCameraCoords(b->egoToWorld(bAttach));
    painter.drawLine(s1.x, s1.y, s2.x, s2.y);
}
