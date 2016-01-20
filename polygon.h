#ifndef POLYGON_H
#define POLYGON_H

#include <QPainter>
#include <QPointF>
#include <vector>
#include "renderable.h"
#include "phys.h"
#include "wpos.h"

double orientation(WPos, WPos, WPos);
std::vector<WPos> convexHull(std::vector<WPos>);
WPos CMMI(std::vector<WPos>, double);
double area(const WPos[], const int);

class Polygon : public Phys, public Renderable
{
public:
    Polygon() {}
    Polygon(Vec p, std::vector<WPos> points, double density=1.0):
        Phys(p)
    {
        verts = convexHull(points);
        mass = density*area();

        WPos cm = CMMI(verts, density);
        mi = cm.w;

        // Adjust our coordinates so that the centre of mass lies at the origin
        // in egocentric coordinates.
        for (WPos &v : verts)
        {
            v.x -= cm.x;
            v.y -= cm.y;
            worldverts.push_back(QPointF(v.x, v.y));
        }

        fillColor = Qt::blue;
    }

    void render(QPainter &, const Camera&);
    double area();
    bool isInternal(WPos);

    std::vector<WPos> verts;
    std::vector<QPointF> worldverts;

private:
    void egoToCamQPointF(const WPos p, QPointF &qp, const Camera &cam);
};

int quadrantsWound(const Vec, const Vec);


#endif // POLYGON_H
