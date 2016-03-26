#include "vec.h"

#include <cmath>

#include "constants.h"

Vec& Vec::operator= (const Vec &v)
{
    x = v.x;
    y = v.y;
    return *this;
}

Vec Vec::operator+ (Vec v)
{
    Vec temp(x + v.x, y + v.y);
    return temp;
}

Vec& Vec::operator+= (const Vec &v)
{
    x += v.x;
    y += v.y;
    return *this;
}



Vec Vec::operator- (Vec v)
{
    Vec temp(x - v.x, y - v.y);
    return temp;
}

Vec Vec::operator* (double c)
{
    Vec temp(x * c, y * c);
    return temp;
}

Vec Vec::operator/ (double c)
{
    Vec temp(x / c, y / c);
    return temp;
}

bool operator == (Vec &v1, Vec&v2)
{
    return v1.x == v2.x && v1.y == v2.y;
}

bool operator != (Vec &v1, Vec&v2)
{
    return !(v1 == v2);
}



void Vec::set(double X, double Y)
{
    x = X;
    y = Y;
}

double Vec::length() const
{
    return sqrt(x*x + y*y);
}

double Vec::length_squared() const
{
    return x*x + y*y;
}

// Returns an angle between 0 and 2PI radians
double Vec::angle() const
{
    if (x == 0 && y == 0) return 0;
    double a = atan2(y, x);
    return a > 0 ? a : TWOPI + a;
}

double Vec::angle(const Vec& v) const {
    double a =  v.angle() - this->angle();
    return a > 0 ? a : TWOPI + a;
}

Vec Vec::unit()
{
    Vec u(*this);
    u.normalise();
    return u;
}

void Vec::normalise()
{
    double l = length();
    x = x/l;
    y = y/l;
}

Vec Vec::perpendicular()
{
    return Vec(-y, x);
}

Vec Vec::rotated(double angle)
{
       double c = cos(angle);
       double s = sin(angle);
       return Vec(x*c - y*s, x*s + y*c);
}

double Vec::dot(Vec v)
{
    return x*v.x + y*v.y;
}

double Vec::perp_dot(Vec v)
{
    return x*v.y - y*v.x;
}

QPointF Vec::qpointf()
{
    return QPointF(x, y);
}

WPos Vec::wpos()
{
    return WPos(x, y);
}
