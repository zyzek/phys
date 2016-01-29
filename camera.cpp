#include "camera.h"

Vec Camera::convertToCameraCoords(Vec p) const {
    return Vec((p.x - pos.x)*zoom + dim.x/2, (pos.y - p.y)*zoom + dim.y/2);
}

Vec Camera::convertToWorldCoords(Vec p) const {
    return Vec((p.x - dim.x/2)/zoom + pos.x, pos.y - (p.y - dim.y/2)/zoom);
}

Vec Camera::mouseWorldCoords()
{
    return convertToWorldCoords(mousePos);
}

void Camera::renderScene(QPainter &painter, std::vector<Renderable*>& renderables)
{
    if (focus) {
            pos = focus->pos;
    }

    for (Renderable* obj : renderables)
    {
        obj->render(painter, *this);
    }
}
