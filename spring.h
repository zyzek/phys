#ifndef SPRING_H
#define SPRING_H

#include <QPainter>

#include "phys.h"
#include "camera.h"

class Camera;
class Phys;

class Spring : public Renderable
{
public:
    Spring(Phys* A, Vec ap, Phys* B, Vec bp, double L, double S):
        a(A), b(B), aAttach(ap), bAttach(bp), length(L), springiness(S)
    {
        fillColor = Qt::darkRed;
    }

    void apply();
    void render(QPainter &, const Camera&);

    Phys *a, *b;
    Vec aAttach, bAttach;
    double length;
    double springiness;
};

#endif // SPRING_H
