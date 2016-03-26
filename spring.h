#ifndef SPRING_H
#define SPRING_H

#include <QPainter>
#include <QColor>

#include "phys.h"
#include "camera.h"

class Phys;

class Spring : public Renderable
{
public:
    Spring(Phys* A, Vec ap, Phys* B, Vec bp, double L, double S, QColor c = Qt::darkRed):
        a(A), b(B), a_attach(ap), b_attach(bp), length(L), elasticity(S), stroke_color(c) {}

    virtual ~Spring() {}

    void apply();
    void render(QPainter &, const Camera&);

    Phys *a, *b;
    Vec a_attach, b_attach;
    double length;
    double elasticity;

    QColor stroke_color = Qt::darkRed;
};

#endif // SPRING_H
