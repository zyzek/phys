#ifndef GRAVVECTOR_H
#define GRAVVECTOR_H

#include "renderable.h"
#include "world.h"
#include "camera.h"

#include <string>
#include <QColor>


class GravVector : public Renderable
{
public:
    GravVector() {}
    GravVector(Vec);

    virtual ~GravVector() {}

    void render(QPainter &, const Camera &);

    void set_cam_pos(const Vec);
    void update_world_pos(const Camera&);
    void update_color();
    void recalc_gravity(const std::vector<Phys*> &objects);

    std::string vec_str();
    QColor stroke_color = Qt::magenta;

private:
    Vec world_pos = Vec(0,0), cam_pos = Vec(0,0), grav_vector = Vec(0,0);
};

#endif // GRAVVECTOR_H
