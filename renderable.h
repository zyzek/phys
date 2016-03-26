#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "camera.h"

class Camera;

class Renderable
{
public:
    virtual ~Renderable() {}
    virtual void render(QPainter &, const Camera&) = 0;
};

#endif // RENDERABLE_H
