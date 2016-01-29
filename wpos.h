#ifndef WPOS_H
#define WPOS_H

class Vec;

class WPos
{
public:
    WPos():
        x(0.0), y(0.0), w(0.0) {}
    WPos(double X, double Y, double W=1.0):
        x(X), y(Y), w(W) { }
    WPos(const WPos& p):
        WPos(p.x, p.y, p.w) { }

    WPos operator + (const WPos &);
    WPos operator + (const Vec &);
    WPos operator + (const double &);

    friend Vec operator - (const WPos &, const WPos &);
    WPos operator - (const double &);

    WPos operator * (const double &);
    WPos operator / (const double &);

    WPos& operator = (const WPos &);

    friend bool operator == (WPos&, WPos&);
    friend bool operator != (WPos&, WPos&);

    // Defines a lexicographic ordering among points.
    friend bool operator > (WPos&, WPos&);
    friend bool operator < (WPos&, WPos&);

    double x, y, w;
};

#endif // WPOS_H
