#include "gravvector.h"

#include "constants.h"

using namespace std;

GravVector::GravVector(Vec pos)
    : cam_pos(pos) { }

void GravVector::render(QPainter &painter, const Camera& cam) {
    painter.setPen(stroke_color);
    Vec cam_grav = Vec(grav_vector.x, -grav_vector.y)*40;

    if (cam_grav.length() > GRAV_VECTOR_MAX_LENGTH) cam_grav = cam_grav.unit()*GRAV_VECTOR_MAX_LENGTH;
    //if (cam_grav.length() < 2) cam_grav = cam_grav.unit()*2;

    painter.drawLine(cam_pos.qpointf(), (cam_pos + cam_grav).qpointf());
}

void GravVector::set_cam_pos(const Vec newpos) {
    cam_pos = newpos;
}

void GravVector::update_world_pos(const Camera &cam) {
    world_pos = cam.to_world_space(cam_pos);
}

void GravVector::update_color() {
    double h = grav_vector.angle()/TWOPI;
    stroke_color.setHsvF(h, 1.0, 1.0);
}

void GravVector::recalc_gravity(const std::vector<Phys*> &objects) {
    grav_vector = Vec(0,0);
    for (Phys *object : objects) {
        Vec cd = object->pos - world_pos;
        grav_vector = grav_vector + cd.unit()*(G*object->mass)/pow(cd.length(), 2);
    }
}

string GravVector::vec_str() {
    string s = "Cam: (" + to_string(cam_pos.x) + ", "  + to_string(cam_pos.y) + ")" +
             "; World: (" + to_string(world_pos.x) + ", " + to_string(world_pos.y) + ")" +
             "; Grav: (" + to_string(grav_vector.x) + ", " + to_string(grav_vector.y) + ")" +
             "; Cam Grav: (" + to_string(grav_vector.x * 40) + ", " + to_string(-grav_vector.y * 40) + ")" +
             "; Hue: " + stroke_color.name().toStdString() +
             "; Grav Angle: " + to_string(grav_vector.angle()/TWOPI) +
             "; Length: " + to_string(grav_vector.length()) + "\n";
    return s;
}
