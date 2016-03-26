#include "gravfield.h"
#include <algorithm>
#include <QColor>

/*GravField::GravField(std::vector<Phys*> *sources)
    : grav_sources(sources)
{
    if (!GRAV_FIELD_POINTS) {
        gf_size = 0;
    }
    else {
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
            grav_vector_field[(j*horiz_points) + i].set_cam_pos(pos);
        }
        }
    }
}

void GravField::render(QPainter &painter, const Camera &cam) {
    for (size_t i = 0; i < gf_size; ++i) {
        GravVector &g = grav_vector_field[i];
        g.update_world_pos(cam);
        g.recalc_gravity(*grav_sources);
        g.update_color();
        g.render(painter, cam);
    }
}*/

GravField::GravField(std::vector<Phys*> *sources)
    : grav_sources(sources)
{
    if (!GRAV_FIELD_POINTS) {
        gf_size = 0;
    }
    else {
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
                cam_field[(j*horiz_points) + i] = pos;
            }
        }
    }
}

void GravField::render(QPainter &painter, const Camera &cam) {
    for (size_t i = 0; i < gf_size; ++i) {
        grav_field[i] = Vec(0, 0);
        world_field[i] = cam.to_world_space(cam_field[i]);
    }

    for (Phys *&object : *grav_sources) {
        Vec o_pos = object->pos;
        double o_mass = object->mass;

        for (size_t i = 0; i < gf_size; ++i) {
            Vec g_vec = o_pos - world_field[i];
            grav_field[i] += g_vec.unit()*(G*o_mass) / g_vec.length_squared();
        }
    }

    for (size_t i = 0; i < gf_size; ++i) {
        Vec &cam_pos = cam_field[i];
        Vec cam_grav = Vec(grav_field[i].x, -grav_field[i].y)*40;

        double length = cam_grav.length();
        if (length > GRAV_VECTOR_MAX_LENGTH) cam_grav = cam_grav.unit()*GRAV_VECTOR_MAX_LENGTH;
        else if (length < 0.5) cam_grav = cam_grav.unit()*0.5;//continue;
        //else if (length < 3.0 && length > 0.5) cam_grav = cam_grav.unit()*3.0;

        double value;

        if (fmod(length, 0.1) < 0.01) {
            value = 1.0;
        }
        else {
            value = std::max(std::min(sqrt(length), 1.0), 0.3);
            //value = 0.0;
        }

        painter.setPen(QColor::fromHsvF(cam_grav.angle()/TWOPI, 1.0, value));
        // Vec(1,1) added here because it seems as if Qt is off by a pixel in either axis.
        painter.drawLine(cam_pos.qpointf(), (cam_pos + cam_grav /*+ Vec(-1, 1)*/).qpointf());
    }
}
