#ifndef CIRCLE_H
#define CIRCLE_H

#define PI 3.14159265

#include <math.h>
#include <QPainter>
#include "vec.h"
#include "renderable.h"
#include "phys.h"

class Circle : public Phys, public Renderable
{
public:
    Circle(Vec p, double r, double m=1.0);
    virtual ~Circle() {}

    void render(QPainter &, const Camera&);

    bool is_internal(WPos) const;

private:
    Circle();

};

#endif // CIRCLE_H
