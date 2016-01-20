#ifndef CIRCLE_H
#define CIRCLE_H

#define PI 3.14159265

#include <QPainter>
#include "vec.h"
#include "renderable.h"
#include "phys.h"

class Circle : public Phys, public Renderable
{
public:
    Circle(Vec p, double r, double density=1.0):
        Phys(p), radius(r)
    {
        mass = density*PI*r*r;
        mi = mass*r*r/2.0;
    }

    void render(QPainter &, const Camera&);

private:
    Circle();

    double radius = 1.0;
};

#endif // CIRCLE_H