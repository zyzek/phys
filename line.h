#ifndef LINE_H
#define LINE_H
#include "renderable.h"

#include <QColor>

class Line: public Renderable
{
public:
    Line(Vec a, Vec b):
        p1(a), p2(b) {}

    virtual ~Line() {}

    void render(QPainter &, const Camera &);
    Vec p1, p2;

    QColor stroke_color = Qt::white;
};

void Line::render(QPainter &painter, const Camera &cam)
{
    painter.setPen(stroke_color);
    painter.drawLine(cam.to_screen_space(p1).qpointf(), cam.to_screen_space(p2).qpointf());
}

#endif // LINE_H

