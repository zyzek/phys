#ifndef CIRCLE_H
#define CIRCLE_H

#include <math.h>
#include <QPainter>
#include <QColor>
#include "vec.h"
#include "phys.h"
#include "constants.h"

class Circle : public Phys
{
public:
    Circle(Vec p, double r, double m = 1.0);
    virtual ~Circle() {}

    void render(QPainter &, const Camera&);

    bool is_internal(WPos) const;

    QColor stroke_color = QColor(Qt::black);
    QColor fill_color = QColor(Qt::yellow);

private:
    Circle();

};

#endif // CIRCLE_H
