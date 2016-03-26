#ifndef POLYGON_H
#define POLYGON_H

#include <QPainter>
#include <QColor>
#include <QPointF>
#include <vector>
#include "phys.h"
#include "wpos.h"
#include "collision.h"

double orientation(WPos, WPos, WPos);
std::vector<WPos> convexHull(std::vector<WPos>);
WPos CMMI(std::vector<WPos>, double);
double area(const WPos[], const int);

class Polygon : public Phys
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
            world_verts.push_back(QPointF(v.x, v.y));
        }

        fill_color = Qt::blue;
    }

    virtual ~Polygon() {}

    void render(QPainter &, const Camera&);
    double area();
    bool is_internal(WPos) const;
    bool is_on_boundary(WPos);

    std::vector<WPos> verts;
    std::vector<QPointF> world_verts;

    QColor stroke_color = QColor(Qt::black);
    QColor fill_color = QColor(Qt::yellow);

private:
    void ego_to_cam_QPointF(const WPos p, QPointF &qp, const Camera &cam);
};

int quadrants_wound(const Vec&, const Vec&);


#endif // POLYGON_H
