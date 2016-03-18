#ifndef LINE_H
#define LINE_H
#include "renderable.h"

class Line: public Renderable
{
public:
    Line(Vec a, Vec b):
        p1(a), p2(b) {}

    virtual ~Line() {}

    void render(QPainter &, const Camera &);
    Vec p1, p2;
};

void Line::render(QPainter &painter, const Camera &cam)
{
    painter.setPen(stroke_color);
    painter.drawLine(cam.to_screen_space(p1).qpointf(), cam.to_screen_space(p2).qpointf());
}

#endif // LINE_H

