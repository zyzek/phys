#include "gravfield.h"
#include <algorithm>

GravField::GravField(std::vector<Phys*> *sources)
    : grav_sources(sources)
{
    // Set up the individual vectors in the field.
    size_t long_axis = std::max(WINDOW_HEIGHT, WINDOW_WIDTH);
    double spacing = long_axis/GRAV_FIELD_POINTS;
    size_t horiz_points = WINDOW_WIDTH/spacing
         , vert_points = WINDOW_HEIGHT/spacing;
    gf_size = horiz_points*vert_points;

    for (size_t j = 0; j < vert_points; ++j) {
        double y = j*spacing;
        for (size_t i = 0; i < horiz_points; ++i) {
            double x = i*spacing;
            Vec pos = Vec(x, y);
            grav_field[(j*horiz_points) + i].set_cam_pos(pos);
        }
    }
}

void GravField::render(QPainter &painter, const Camera &cam) {
    for (size_t i = 0; i < gf_size; ++i) {
        GravVector &g = grav_field[i];
        g.update_world_pos(cam);
        g.recalc_gravity(*grav_sources);
        g.update_color();
        g.render(painter, cam);
    }
}
