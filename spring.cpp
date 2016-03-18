#include "spring.h"

void Spring::apply()
{
    Vec ab = b->ego_to_world(b_attach) - a->ego_to_world(a_attach);
    double strength = elasticity*(ab.length() - length);
    Vec force = ab.unit()*strength;

    a->apply_force(force, a_attach);
    b->apply_force(force*-1, b_attach);
}


void Spring::render(QPainter &painter, const Camera &cam)
{
    painter.setPen(stroke_color);
    Vec s1 = cam.to_screen_space(a->ego_to_world(a_attach));
    Vec s2 = cam.to_screen_space(b->ego_to_world(b_attach));
    painter.drawLine(s1.x, s1.y, s2.x, s2.y);
}
