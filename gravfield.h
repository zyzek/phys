#ifndef GRAVFIELD_H
#define GRAVFIELD_H

#include <vector>
#include <array>

#include "world.h"
#include "renderable.h"
#include "phys.h"
#include "gravvector.h"

#include "constants.h"



class GravField: public Renderable
{
public:
    GravField(std::vector<Phys*>*);
    virtual ~GravField() {}
    void render(QPainter &, const Camera&);

private:
    std::vector<Phys*> *grav_sources;
    std::array<Vec, GRAV_FIELD_POINTS*GRAV_FIELD_POINTS> cam_field;
    std::array<Vec, GRAV_FIELD_POINTS*GRAV_FIELD_POINTS> world_field;
    std::array<Vec, GRAV_FIELD_POINTS*GRAV_FIELD_POINTS> grav_field;
    //std::array<double, GRAV_FIELD_POINTS*GRAV_FIELD_POINTS> grav_colors;
    //std::array<GravVector, GRAV_FIELD_POINTS*GRAV_FIELD_POINTS> grav_vector_field;
    size_t gf_size;
};

#endif // GRAVFIELD_H
