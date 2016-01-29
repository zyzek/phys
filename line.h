#ifndef LINE_H
#define LINE_H
#include "renderable.h"

class Line: public Renderable
{
public:
    Line(Vec a, Vec b):
        p1(a), p2(b) {}

    void render(QPainter &, const Camera &);
    Vec p1, p2;
};

void Line::render(QPainter &painter, const Camera &cam)
{
    painter.setPen(strokeColor);
    painter.drawLine(cam.convertToCameraCoords(p1).qpointf(), cam.convertToCameraCoords(p2).qpointf());
}

#endif // LINE_H

