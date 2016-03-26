#ifndef VEC_H
#define VEC_H

#include <string>
#include <QPointF>
#include "wpos.h"

class Vec
{
public:
    Vec():
        x(0.0), y(0.0) {}
    Vec(double X, double Y):
        x(X), y(Y) { }
    Vec(const Vec& v):
        x(v.x), y(v.y) {}
    Vec(const WPos& p):
        Vec(p.x, p.y) {}

    virtual ~Vec() {}

    Vec& operator = (const Vec&);
    Vec operator + (Vec);
    Vec& operator += (const Vec&);
    Vec operator - (Vec);
    Vec operator * (double);
    Vec operator / (double);

    friend bool operator == (Vec&, Vec&);
    friend bool operator != (Vec&, Vec&);

    operator std::string () const {
            return "<" + std::to_string(x) + ", " + std::to_string(y) + ">";
    }

    void set(double, double);
    double length() const;
    double length_squared() const;
    double angle() const;
    double angle(const Vec&) const;
    Vec unit();
    void normalise();
    Vec perpendicular();
    Vec rotated(double angle);

    double dot(Vec);
    double perp_dot(Vec);

    QPointF qpointf();
    WPos wpos();

    double x, y;
};

#endif // VEC_H
