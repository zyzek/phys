#ifndef GRAVFIELD_H
#define GRAVFIELD_H

#include <vector>
#include <array>

#include "world.h"
#include "renderable.h"
#include "phys.h"
#include "gravvector.h"



class GravField: public Renderable
{
public:
    GravField(std::vector<Phys*>*);
    virtual ~GravField() {};
    virtual void render(QPainter &, const Camera&);

private:
    std::vector<Phys*> *grav_sources;
    std::array<GravVector, GRAV_FIELD_POINTS*GRAV_FIELD_POINTS> grav_field;
    size_t gf_size;
};

#endif // GRAVFIELD_H
