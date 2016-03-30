/* Camera
 *
 * The camera holds the current view position, zoom level, window size.
 * In addition, it contains methods to handle conversion between screen and world space.
 * When rendering, a list of all objects to be rendered is passed to the camera,
 * which calls each object's individual render method.
 */

#include "camera.h"

#include <algorithm>
#include <cmath>
#include <limits>

#include "phys.h"
#include "renderable.h"


Camera::Camera() {}
Camera::Camera(int w, int h): dim(w, h) {}

// Given a world coordinate, returns a coordinate in screen space.
Vec Camera::to_screen_space(Vec p) const {
    return Vec((p.x - pos.x)*zoom + dim.x/2, (pos.y - p.y)*zoom + dim.y/2);
}

// Given a screen space coordinate, returns the corresponding world coordinate.
Vec Camera::to_world_space(Vec p) const {
    return Vec((p.x - dim.x/2)/zoom + pos.x, pos.y - (p.y - dim.y/2)/zoom);
}

// Obtain world position the mouse is pointing to.
Vec Camera::mouse_world_coords() const {
    return to_world_space(mouse_pos);
}

// Move the camera and zoom so that its view just contains the given set of points.
void Camera::encompass_points(std::vector<Vec> points) {
    if (points.empty()) {
        return;
    }
    else if (points.size() == 1) {
        pos = points[0];
        return;
    }

    // Find the extreme points.
    Vec minv, maxv = points[0];
    for (Vec p : points) {
        if (p.x < minv.x) minv.x = p.x;
        if (p.y < minv.y) minv.y = p.y;
        if (p.x > maxv.x) maxv.x = p.x;
        if (p.y > maxv.y) maxv.y = p.y;
    }

    // Move to the middle of the extreme points, zoom so they just fit in the frame.
    pos = Vec((minv.x + maxv.x)/2, (minv.y + maxv.y)/2);
    double cand_zoom = 0.95*std::min(dim.x / (maxv.x - minv.x),
                                     dim.y / (maxv.y - minv.y));

    // This check in case points are colocated on either axis.
    if (std::isfinite(cand_zoom)) zoom = cand_zoom;
}

// Render all objects in the scene.
void Camera::render_scene(QPainter &painter, std::vector<Renderable*>& renderables) const
{
    for (Renderable* obj : renderables)
    {
        obj->render(painter, *this);
    }
}

// If the camera is focused on some object, move the camera to its position.
void Camera::move_to_focus() {
    if (focus) {
            pos = focus->pos;
    }
}

double Camera::scale_by_zoom(double r) const {
    // One wishes to simulate the solar system at a realistic scale,
    // but when zoomed out far enough to see the entire thing, only
    // the sun (and perhaps jupiter) is actually visible.
    // Thus the idea of this function is to scale the apparent size of things up
    // as we zoom out, while maintaining some idea of their relative actual sizes.
    //
    // We then define a function f: R+xR+ -> R+ to rescale the apparent radii of objects.
    // It is desirable that such a function satisfies the following conditions:
    // 1. r1 > r2 -> f(r1, z) > f(r2, z)
    // 2. z1 > z2 -> f(r, z1) > f(r, z2)
    // 3. (lim z -> 0 |f(r1, z) - f(r2, z)|) = 0
    // 4. (lim z -> 0 f(r, z)) = c, c > 0
    //
    // In english:
    // 1. if one object is actually bigger than another, it should appear larger at any zoom level
    // 2. An object at a closer zoom level should appear larger than one at a more distant zoom level
    // 3. As we zoom out, the apparent difference between the sizes of objects shrinks.
    // 4. As one zooms out, the size of objects approaches some nonzero constant.
    //
    // zoom_rescale can be tweaked to change the power of this scaling;
    // by setting it to 0, this feature can be turned off.
    //
    // min_scale_log allows the differences in the log-size of the various objects
    // to become apparent, by subtracting the greatest lower bound of the log-sizes
    // of the objects under observation.

    return zoom*r + zoom_rescale*(0.5 + log(r) - min_scale_log);
}

Vec Camera::get_pos(void) const
{
	return pos;
}

void Camera::set_pos(const Vec &new_pos)
{
	pos = new_pos;
}

Vec Camera::get_dimensions(void) const
{
	return dim;
}

Vec Camera::set_dimensions(const Vec &new_dim)
{
	return dim = new_dim;
}

double Camera::get_zoom(void) const
{
	return zoom;
}

void Camera::set_zoom(const double new_zoom)
{
	zoom = new_zoom;
}

double Camera::get_zoom_rescale(void) const
{
	return zoom_rescale;
}

void Camera::set_zoom_rescale(const double new_rescale)
{
	zoom_rescale = new_rescale;
}

double Camera::get_min_scale_log(void) const
{
	return min_scale_log;
}

void Camera::set_min_scale_log(const double new_min)
{
	min_scale_log = new_min;
}
