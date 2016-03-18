#include "circle.h"

Circle::Circle(Vec p, double r, double m):
    Phys(p, PhysType::Circle, r)
{
    mass = m;
    mi = mass*r*r/2.0;
}

void Circle::render(QPainter &painter, const Camera &cam) {
    painter.setPen(stroke_color);
    painter.setBrush(QBrush(fill_color));

    Vec screenpos = cam.to_screen_space(pos);
    double scaled_rad = cam.scale_by_zoom(radius);

    // Don't draw if off screen.
    double xl = screenpos.x - scaled_rad;
    if (xl > cam.dim.x) return;
    double yt = screenpos.y - scaled_rad;
    if (yt > cam.dim.y) return;
    if (screenpos.x + scaled_rad < 0) return;
    if (screenpos.y + scaled_rad < 0) return;

    painter.drawEllipse(xl, yt, 2*scaled_rad, 2*scaled_rad);
}

bool Circle::is_internal(WPos p) const {
    return (Vec(p.x, p.y)).length() <= radius;
}
