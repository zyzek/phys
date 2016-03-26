#ifndef WORLD_H
#define WORLD_H

#include "renderable.h"
#include "phys.h"
#include "spring.h"
#include "wpos.h"

class World : public Renderable
{
public:
    World();
    virtual ~World();

    static World* get_world();

    void apply_gravity();
    void apply_springs();

    Phys* object_at(WPos);

    void render(QPainter &, const Camera&);

    std::vector<Phys*> objects;
    std::vector<Spring*> springs;
    double time_speed = 10.0;
    Phys* cam_focus;
    double dt;
};

#endif // WORLD_H
