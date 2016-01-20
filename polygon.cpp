#include "polygon.h"

#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>

#include <QPointF>
#include <QPainter>

#define PI 3.141592654

void Polygon::render(QPainter &painter, const Camera &cam)
{
    for (auto i = 0; i < verts.size(); ++i)
    {
        egoToCamQPointF(verts[i], worldverts[i], cam);
    }

    painter.setPen(strokeColor);
    painter.setBrush(QBrush(fillColor));
    painter.drawConvexPolygon(&worldverts[0], worldverts.size());
}

// Calculate screen space coordinates given egocentric coordinates.
void Polygon::egoToCamQPointF(const WPos p, QPointF &qp, const Camera &cam)
{
    Vec transformed = cam.convertToCameraCoords(egoToWorld(p));
    qp.setX(transformed.x);
    qp.setY(transformed.y);
}

double Polygon::area()
{
    return ::area(&verts[0], verts.size());
}

// Returns the signed area of a convex polygon.
// Verts should be given in anticlockwise order.
double area(const WPos verts[], const int count)
{
    double area = 0.0;

    for (auto i = 0; i < count; ++i)
    {
        WPos v1 = verts[i], v2 = verts[(i+1)%count];
        area += v1.x*v2.y - v2.x*v1.y;
    }

    return area/2.0;
}

// Calculate a weighted point whose coordinates are the centre of mass, and whose weight
// Is the moment of inertia of the convex polygon defined by the given set of vertices.
// Weighted vertices will shift the mass distribution.
// The approach here is to triangulate the convex hull and take the weighted average
// of the centers of mass of the resulting triangles, where a triangle's mass depends
// upon its area.
WPos CMMI(std::vector<WPos> verts, double density)
{
    if (verts.size() < 3) return WPos(0,0);

    WPos weighted_centroid, pivot = verts.front();
    std::vector<WPos> centroids;

    double tri_mass, weighted_mass = 0.0;

    for (auto vert = verts.begin() + 1; vert != verts.end() - 1; ++vert)
    {
        WPos tri_points[3] = {pivot, *vert, *(vert + 1)};
        tri_mass = area(tri_points, 3)*density;
        weighted_centroid = (pivot + *vert + *(vert + 1)) * tri_mass;
        weighted_mass += weighted_centroid.w;
        centroids.push_back(weighted_centroid);
    }

    double ratio = (density*area(&verts[0], verts.size())) / weighted_mass;

    for (WPos &c : centroids)
    {
        c.w *= ratio;
    }

    WPos CMMI = std::accumulate(centroids.begin(), centroids.end(), WPos(0,0,0));
    CMMI.w = 0.0;

    for (WPos &c : centroids)
    {
        Vec rad = (CMMI - c);
        CMMI.w += rad.dot(rad) * c.w;
    }

    return CMMI;
}


// Determine the direction a triangle's ordered vertices are oriented
// negative if clockwise,
// positive if anti-clockwise
// zero if the triangle is degenerate.
double orientation(WPos p0, WPos p1, WPos p2)
{
    return ( (p1.x - p0.x) * (p2.y - p0.y)
           - (p2.x - p0.x) * (p1.y - p0.y) );
}

struct AngleComp {
    AngleComp(WPos p):
        m(p) { }

    bool operator() (WPos i, WPos j)
    {
        return (i - m).angle() < (j - m).angle();
    }

    WPos m;
};

std::vector<WPos> convexHull(std::vector<WPos> points)
{
    // Remove duplicated elements
    std::sort(points.begin(), points.end());
    points.erase(std::unique(points.begin(), points.end()), points.end());

    if (points.size() < 3)
    {
        return std::vector<WPos>({WPos(0,0), WPos(1,0), WPos(0,1)});
    }

    // The point, m, with the lowest lexicographic coordinates must be on the hull
    std::vector<WPos> hull;
    auto im = std::min_element(points.begin(), points.end());
    WPos m = *im;
    hull.push_back(m);
    points.erase(im);

    // Sort by the angle of the vector from m to each point, producing P
    AngleComp comp(m);
    std::sort(points.begin(), points.end(), comp);

    // for p in P (sweeping through the set of points anticlockwise)
    //   while p is oriented clockwise wrt the last line segment on the hull,
    //      remove hull.last, as it forms a concavity between p and (hull.last - 1)
    //   add p to the hull.
    hull.push_back(*points.begin());
    for (auto iter = points.begin() + 1; iter != points.end(); ++iter)
    {
        // TODO: Make this also discard points that don't change edge angle appreciably.
        while (orientation(*(hull.end() - 2), *(hull.end() - 1), *iter) < 0)
        {
            hull.pop_back();
        }

        hull.push_back(*iter);
    }

    // We need not check if the last point, e, added produces a concavity.
    // If it did, then we could produce a triangle, external to the hull, with vertices
    // m, e, and p (p being the previous vertex on the hull to e)
    // In this case we have a nonzero angle, epsilon, between me and mp.
    // Then the mp's reference angle, arg(mp) = (arg(me) + epsilon).
    // Note that 0 <= arg(me) <= pi, as m was chosen to minimise y ordinate.
    // But if arg(me) < arg(mp), then e must have been added to the hull before p.
    // That e was taken to be the last point added contradicts this,
    // so e cannot produce a concavity in the hull.
    return hull;
}


bool Polygon::isInternal(WPos p)
{
    // Check the angle around our test point that the boundary of the polygon subtends.
    // We know the point is internal if that angle is a full revolution.

    int winding = quadrantsWound(verts.back() - p, verts.front() - p);

    Vec curr, last = verts.front() - p;

    for (const WPos& b : verts)
    {
        curr = b - p;
        winding += quadrantsWound(last, curr);
        last = curr;
    }

    return winding == 4 || winding == -4;
}

int quadrantsWound(const Vec p1, const Vec p2)
{
    if (p1.x > 0) {
        if (p2.x > 0) {
            if (p1.y > 0) {
                if (p2.y > 0) {
                    // Q1 -> Q1
                    return 0;
                }
                else {
                    // Q1 -> Q4
                    return -1;
                }
            }
            else {
                if (p2.y > 0) {
                    // Q4 -> Q1
                    return 1;
                }
                else {
                    // Q4 -> Q4
                    return 0;
                }
            }
        }
        else {
            if (p1.y > 0) {
                if (p2.y > 0) {
                    // Q1 -> Q2
                    return 1;
                }
                else {
                    // Q1 -> Q3
                    if (p1.angle(p2) < PI) {
                        return 2;
                    }

                    return -2;
                }
            }
            else {
                if (p2.y > 0) {
                    // Q4 -> Q2
                    if (p1.angle(p2) < PI) {
                        return 2;
                    }

                    return -2;
                }
                else {
                    // Q4 -> Q3
                    return -1;
                }
            }
        }
    }
    else
    {
        if (p2.x > 0) {
            if (p1.y > 0) {
                if (p2.y > 0) {
                    // Q2 -> Q1
                    return -1;
                }
                else {
                    // Q2 -> Q4
                    if (p1.angle(p2) < PI) {
                        return 2;
                    }

                    return -2;
                }
            }
            else {
                if (p2.y > 0) {
                    // Q3 -> Q1
                    if (p1.angle(p2) < PI) {
                        return 2;
                    }

                    return -2;
                }
                else {
                    // Q3 -> Q4
                    return 1;
                }
            }
        }
        else {
            if (p1.y > 0) {
                if (p2.y > 0) {
                    // Q2 -> Q2
                    return 0;
                }
                else {
                    // Q2 -> Q3
                    return 1;
                }
            }
            else {
                if (p2.y > 0) {
                    // Q3 -> Q2
                    return -1;
                }
                else {
                    // Q3 -> Q3
                    return 0;
                }
            }
        }
    }
}





