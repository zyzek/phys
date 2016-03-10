#ifndef POLYGON_H
#define POLYGON_H

#include <QPainter>
#include <QPointF>
#include <vector>
#include "renderable.h"
#include "phys.h"
#include "wpos.h"
#include "collision.h"

double orientation(WPos, WPos, WPos);
std::vector<WPos> convexHull(std::vector<WPos>);
WPos CMMI(std::vector<WPos>, double);
double area(const WPos[], const int);

class Polygon : public Phys, public Renderable
{
public:
    Polygon() {}
    Polygon(Vec p, std::vector<WPos> points, double density=1.0):
        Phys(p, PhysType::Polygon)
    {
        verts = convexHull(points);
        mass = density*area();

        WPos cm = CMMI(verts, density);
        mi = cm.w;

        // For a polygon, the radius will represent the major radius; the longest distance from the object's centre of mass to any of its vertices.
        // TODO: Change the intersection check on polygons to the smallest enclosing ellipse? e.g., find the equation x^2/a + y^2/b + z^2/c = 0
        //		 centred at the CoM and any point substituted in which returns a value less than one should be inside the ellipse.
        radius = 0;

        // Adjust our coordinates so that the centre of mass lies at the origin
        // in egocentric coordinates.
        for (WPos &v : verts)
        {
            double dist = (v - cm).length();
            if (dist > radius) radius = dist;

            v.x -= cm.x;
            v.y -= cm.y;
            worldverts.push_back(QPointF(v.x, v.y));
        }

        fillColor = Qt::blue;
    }

    void render(QPainter &, const Camera&);
    double area();
    bool isInternal(WPos);
    bool isOnBoundary(WPos);

    std::vector<WPos> verts;
    std::vector<QPointF> worldverts;

private:
    void egoToCamQPointF(const WPos p, QPointF &qp, const Camera &cam);
};

int quadrantsWound(const Vec&, const Vec&);


#endif // POLYGON_H
