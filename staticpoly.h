#ifndef STATICPOLY
#define STATICPOLY

#include "polygon.h"
#include "vec.h"
#include "wpos.h"

class StaticPoly: public Polygon
{
public:
    StaticPoly() {}
    StaticPoly(Vec p, std::vector<WPos> points):
        Polygon(p, points) {}

    void applyForce(Vec, Vec) { return; }
};

#endif // STATICPOLY

