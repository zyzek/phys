#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <QColor>
#include "camera.h"

class Camera;

class Renderable
{
public:    
    virtual void render(QPainter &, const Camera&) = 0;

    QColor strokeColor = QColor(Qt::black);
    QColor fillColor = QColor(Qt::yellow);
};

#endif // RENDERABLE_H
