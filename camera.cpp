#include "camera.h"

Vec Camera::convertToCameraCoords(Vec p) const {
    return Vec((p.x - x)*zoom + w/2, (y - p.y)*zoom + h/2);
}

Vec Camera::convertToWorldCoords(Vec p) const {
    return Vec((p.x - w/2)/zoom + x, y - (p.y - h/2)/zoom);
}

void Camera::renderScene(QPainter &painter, std::vector<Renderable*>& renderables)
{
    for (Renderable* obj : renderables)
    {
        obj->render(painter, *this);
    }
}
