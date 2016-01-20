#include "circle.h"

void Circle::render(QPainter &painter, const Camera &cam)
{
    painter.setPen(strokeColor);
    painter.setBrush(QBrush(fillColor));
    Vec screenpos = cam.convertToCameraCoords(pos);
    painter.drawEllipse(screenpos.x - radius*cam.zoom, screenpos.y - radius*cam.zoom,
                        2*radius*cam.zoom, 2*radius*cam.zoom);
}

