#ifndef CAMERA_H
#define CAMERA_H

#include <QPainter>
#include <vector>
#include "vec.h"
#include "phys.h"
#include "renderable.h"

class Phys;
class Renderable;

class Camera
{
public:
    Camera() {}
    Camera(int W, int H):
        dim(W, H) { }

    void renderScene(QPainter&, std::vector<Renderable*>& renderables);
    Vec convertToCameraCoords(Vec) const;
    Vec convertToWorldCoords(Vec) const;
    Vec mouseWorldCoords();

    Vec pos = Vec(0, 0), dim = Vec(800, 600);
    double zoom = 1.0;

    bool grabbed = false;
    Phys* held_object = nullptr;
    Vec grab_coords = Vec(0,0), pos_when_grabbed = Vec(0,0);
    Vec mousePos = Vec(0,0);
    Vec mouseVel = Vec(0,0);


    Phys* focus = nullptr;
};

#endif // CAMERA_H

