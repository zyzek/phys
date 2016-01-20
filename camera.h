#ifndef CAMERA_H
#define CAMERA_H

#include <QPainter>
#include <vector>
#include "vec.h"
#include "renderable.h"

class Renderable;

class Camera
{
public:
    Camera() {}
    Camera(int W, int H):
        w(W), h(H) { }

    void renderScene(QPainter&, std::vector<Renderable*>& renderables);
    Vec convertToCameraCoords(Vec) const;
    Vec convertToWorldCoords(Vec) const;
    int x = 0, y = 0, w = 800, h = 600;
    double zoom = 1.0;
};

#endif // CAMERA_H
