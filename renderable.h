#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <QColor>
#include "camera.h"

class Camera;

class Renderable
{
public:
    virtual void render(QPainter &, const Camera&) = 0;

    QColor stroke_color = QColor(Qt::black);
    QColor fill_color = QColor(Qt::yellow);
};

#endif // RENDERABLE_H
