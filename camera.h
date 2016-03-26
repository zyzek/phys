#ifndef CAMERA_H
#define CAMERA_H

#include <QPainter>
#include <vector>
#include "vec.h"

class Phys;
class Renderable;

class Camera
{
public:
    Camera();
    Camera(int W, int H);
    virtual ~Camera() {}

    Vec to_screen_space(Vec) const;
    Vec to_world_space(Vec) const;
    Vec mouse_world_coords() const;

    void encompass_points(std::vector<Vec>);
    void render_scene(QPainter&, std::vector<Renderable*>& renderables) const;
    void move_to_focus();
    double scale_by_zoom(double) const;

    // pos : the camera's current world position
    Vec pos = Vec(0, 0);

    // dim : the dimensions of the window in pixels.
    Vec dim = Vec(800, 600);

    // At 1.0 zoom, 1px = 1m; at 0.5, 1px = 2m.
    double zoom = 1.0;
    // Determines the degree to which to augment objects when zooming out.
    double zoom_rescale = 0.0;
    // The log size of the smallest object considered when performing rescaling.
    double min_scale_log = 1.0;

    // Variable facilitating mouse interaction.
    bool grabbed = false;
    Phys* held_object = nullptr;
    Vec grab_coords = Vec(0,0), pos_when_grabbed = Vec(0,0);
    Vec mouse_pos = Vec(0,0);
    Vec mouse_vel = Vec(0,0);

    // Current object the camera is focussed upon; null if no such object.
    Phys* focus = nullptr;
};

#endif // CAMERA_H

