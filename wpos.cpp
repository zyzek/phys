#include "wpos.h"
#include "vec.h"

WPos& WPos::operator = (const WPos &p)
{
    x = p.x;
    y = p.y;
    w = p.w;
    return *this;
}

WPos WPos::operator + (const WPos &p)
{
    WPos temp((w*x + p.w*p.x)/(w + p.w), (w*y + p.w*p.y)/(w + p.w), w + p.w);
    return temp;
}

WPos WPos::operator + (const double &z)
{
    WPos temp(x, y, w+z);
    return temp;
}

WPos WPos::operator + (const Vec &v)
{
    WPos temp(x + v.x, y + v.y, w);
    return temp;
}

Vec operator - (const WPos &p, const WPos &q)
{
    Vec temp(p.x - q.x, p.y - q.y);
    return temp;
}

WPos WPos::operator - (const double &z)
{
    WPos temp(x, y, w-z);
    return temp;
}

WPos WPos::operator * (const double &z)
{
    WPos temp(x, y, w*z);
    return temp;
}

WPos WPos::operator / (const double &z)
{
    WPos temp(x, y, w/z);
    return temp;
}

bool operator == (WPos &p1, WPos&p2)
{
    return p1.x == p2.x && p1.y == p2.y && p1.w == p2.w;
}

bool operator != (WPos &p1, WPos&p2)
{
    return !(p1 == p2);
}

bool operator < (WPos &p1, WPos&p2)
{
    if (p1.y < p2.y)
    {
        return true;
    }
    else if (p1.y > p2.y)
    {
        return false;
    }
    else
    {
        return p1.x < p2.x;
    }
}

bool operator > (WPos &p1, WPos&p2)
{
    if (p1.y > p2.y)
    {
        return true;
    }
    else if (p1.y < p2.y)
    {
        return false;
    }
    else
    {
        return p1.x > p2.x;
    }
}
